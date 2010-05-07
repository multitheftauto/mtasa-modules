addEvent("onIRCConnect")
addEvent("onIRCDisconnect")
addEvent("onIRCMessage")
addEvent("onIRCAction")
addEvent("onIRCJoin")
addEvent("onIRCPart")
addEvent("onIRCQuit")
addEvent("onIRCMode")
addEvent("onIRCNick")
addEvent("onIRCNotice")
addEvent("onIRCTopicReceive")
addEvent("onIRCTopicDataReceive")
addEvent("onIRCUserListReceive")

local log = fileCreate("raw.log")

local getSockTableBySock

local connectedSockets  = {}
local connectingSockets = {}

function ircConnect(host, port, user, nick)
	if host and type(host)=="string" and port and type(port)=="number" and user and type(user)=="string" and nick and type(nick)=="string" then
		local sock = sockOpen(host,port)
		
		if sock then
			table.insert(connectingSockets,{sock=sock,host=host,port=port,user=user,nick=nick})
		end
		
		return sock
	end
	
	return false
end

function ircDisconnect(sock,quitmsg)
	if sock and ircIsConnected(sock) then
		quitmsg = type(quitmsg)=="string" and quitmsg or "Socketbot"
		sockWrite(sock,"QUIT :"..quitmsg.."\r\n")
		sockClose(sock)
		
		return true
	end
	
	return false
end

function ircIsConnected(theSock)
	for index,sock in ipairs(connectedSockets) do
		if sock.sock == theSock then
			return true
		end
	end
	
	return false
end

function ircIsConnecting(theSock)
	for index,sock in ipairs(connectingSockets) do
		if sock.sock == theSock then
			return true
		end
	end
	
	return false
end

function ircGetHost(sock)
	if ircIsConnected(sock) or ircIsConnecting(sock) then
		local data = getSockTableBySock(sock)
		
		return data and data.host
	end
	
	return false
end

function ircGetPort(sock)
	if ircIsConnected(sock) or ircIsConnecting(sock) then
		local data = getSockTableBySock(sock)
		
		return data and data.port
	end
	
	return false
end

function ircGetUser(sock)
	if ircIsConnected(sock) then
		local data = getSockTableBySock(sock)
		
		return data and data.user
	end
	
	return false
end

function ircGetNick(sock)
	if ircIsConnected(sock) then
		local data = getSockTableBySock(sock)
		
		return data and data.nick
	end
	
	return false
end

function ircJoin(sock, channel)
	if sock and ircIsConnected(sock) and channel and type(channel)=="string" then
		return sockWrite(sock,"JOIN "..channel.."\r\n")
	end
	
	return false
end

function ircSay(sock, channel, msg)
	if sock and ircIsConnected(sock) and channel and type(channel)=="string" and msg and type(msg)=="string" then
		return sockWrite(sock,"PRIVMSG "..channel.." :"..msg.."\r\n")
	end
	
	return false
end

function ircAction(sock, channel, action)
	if sock and ircIsConnected(sock) and channel and type(channel)=="string" and action and type(action)=="string" then
		return sockWrite(sock,"PRIVMSG "..channel.." :ACTION "..action.."\r\n")
	end
	
	return false
end

function ircGetUserNick(user)
	if user and type(user) == "string" and string.find(user,"!") then
		return gettok(user,1,string.byte("!"))
	end
	
	return false
end

function ircGetUserIdent(user)
	if user and type(user) == "string" then
		return gettok(gettok(user,2,string.byte("!")),1,string.byte("@"))
	end
	
	return false
end

function ircGetUserAddress(user)
	if user and type(user) == "string" then
		return gettok(user,2,string.byte("@"))
	end
	
	return false
end

function getSockTableBySock(theSock)
	for index,sock in ipairs(connectingSockets) do
		if sock.sock == theSock then
			return sock
		end
	end
	
	for index,sock in ipairs(connectedSockets) do
		if sock.sock == theSock then
			return sock
		end
	end
end

addEventHandler("onResourceStop",getResourceRootElement(),
	function()
		for index,sock in ipairs(connectingSockets) do
			ircDisconnect(sock.sock,"Resource stopping")
		end
		
		for index,sock in ipairs(connectedSockets) do
			ircDisconnect(sock.sock,"Resource stopping")
		end
		
		fileFlush(log)
		fileClose(log)
	end
)

addEventHandler("onSockOpened",getRootElement(),
	function(theSock)
		if ircIsConnecting(theSock) then
			for index,sock in ipairs(connectingSockets) do
				if sock.sock == theSock then
					sockWrite(sock.sock,"USER "..sock.user.." Bot * :SocketBot for MTA:SA 1.0\r\n")
					sockWrite(sock.sock,"NICK "..sock.nick.."\r\n")
					
					table.insert(connectedSockets,table.remove(connectingSockets,index))
					
					break
				end
			end
			
			triggerEvent("onIRCConnect",getRootElement(),theSock)
		end
	end
)

addEventHandler("onSockClosed",getRootElement(),
	function(theSock)
		for index,sock in ipairs(connectedSockets) do
			if sock.sock == theSock then
				table.remove(connectedSockets,index)
				triggerEvent("onIRCDisconnect",getRootElement(),sock.sock)
				return
			end
		end
		
		for index,sock in ipairs(connectingSockets) do
			if sock.sock == theSock then
				table.remove(connectedSockets,index)
				triggerEvent("onIRCDisconnect",getRootElement(),sock.sock)
				return
			end
		end
	end
)

local ignoreEntries = {"001","002","003","004","005","251","252","254","255","265","266","366","372","375","376"}

local function processData(theSock,procData)
	local time    = getRealTime()
	local logData = "["..string.format("%02i",time.hour)..":"..string.format("%02i",time.minute)..":"..string.format("%02i",time.second).."] "..procData.."\r\n"
	
	fileWrite(log,logData)
	fileFlush(log)
	
	if string.find(procData,"PING") == 1 then
		sockWrite(theSock,"PONG :REPLY\r\n")
	else
		local dataTable = split(procData,32)
		
		local user   = string.sub(table.remove(dataTable,1),2)
		local name   = gettok(user,1,33)
		local action = table.remove(dataTable,1)
		
		if action == "PRIVMSG" then
			local channel = table.remove(dataTable,1)
			local msg     = string.sub(table.concat(dataTable," "),2)
			
			if string.find(msg,"ACTION") == 1 then
				local action = string.sub(msg,9,#msg-1)
				
				triggerEvent("onIRCAction",getRootElement(),theSock,user,channel,action)
			else
				triggerEvent("onIRCMessage",getRootElement(),theSock,user,channel,msg)
			end
		elseif action == "JOIN" then
			local channel = string.sub(dataTable[1],2)
			
			triggerEvent("onIRCJoin",getRootElement(),theSock,user,channel)
		elseif action == "PART" then
			local channel = dataTable[1]
			
			triggerEvent("onIRCPart",getRootElement(),theSock,user,channel)
		elseif action == "QUIT" then
			local reason = string.sub(table.concat(dataTable," "),2)
			
			triggerEvent("onIRCQuit",getRootElement(),theSock,user,reason)
		elseif action == "MODE" then
			local channel     = table.remove(dataTable,1)
			local modeSet     = table.remove(dataTable,1)
			
			triggerEvent("onIRCMode",getRootElement(),theSock,user,channel,modeSet,dataTable)
		elseif action == "NICK" then
			local newNick = string.sub(dataTable[1],2)
			
			triggerEvent("onIRCNick",getRootElement(),theSock,user,newNick)
		elseif action == "NOTICE" then
			local target = table.remove(dataTable,1)
			local notice = string.sub(table.concat(dataTable," "),2)
			
			triggerEvent("onIRCNotice",getRootElement(),theSock,user,target,notice)
		elseif action == "332" then
			local target  = table.remove(dataTable,1) -- Bot nickname; practically useless
			local channel = table.remove(dataTable,1)
			local topic   = string.sub(table.concat(dataTable," "),2)
			
			triggerEvent("onIRCTopicReceive",getRootElement(),theSock,channel,topic)
		elseif action == "333" then
			local target    = table.remove(dataTable,1) -- Bot nickname; practically useless
			local channel   = table.remove(dataTable,1)
			local setter    = table.remove(dataTable,1)
			local timestamp = table.remove(dataTable,1)
			
			triggerEvent("onIRCTopicDataReceive",getRootElement(),theSock,channel,setter,timestamp)
		elseif action == "353" then
			local target    = table.remove(dataTable,1) -- Bot nickname; practically useless
			local equalSign = table.remove(dataTable,1) -- 100% useless
			local channel   = table.remove(dataTable,1)
			local users     = dataTable
			
			if #users > 0 then users[1] = string.sub(users[1],2) end
			
			triggerEvent("onIRCUserListReceive",getRootElement(),theSock,channel,users)
		else
			for index,ignoreValue in ipairs(ignoreEntries) do
				if action == ignoreValue then
					return
				end
			end
			
			outputServerLog("? ("..action.."): "..procData)
		end
	end
end

local procBuffer = ""

addEventHandler("onSockData",getRootElement(),
	function(theSock,data)
		if ircIsConnected(theSock) then
			for i=1, #data do
				local char = string.sub(data,i,i)
				
				if char then
					if char ~= "\n" and char ~= "\r" then
						procBuffer = procBuffer .. char
					elseif char == "\n" then
						processData(theSock,procBuffer)
						procBuffer = ""
					end
				end
			end
		end
	end
)