local ircConnections = {}

function getPlayerIRCConnectionByHost(player,host)
	if ircConnections[player] then
		for index,sock in ipairs(ircConnections[player]) do
			if sock.host == host then
				return sock
			end
		end
	end
	
	return false
end

function handleConnectionRequest(host,port,nick)
	local clientConnections = ircConnections[client]
	
	if not clientConnections then
		clientConnections = {}
		ircConnections[client] = clientConnections
	end
	
	for index,sock in ipairs(clientConnections) do
		if sock.host == host then
			return
		end
	end
	
	local sock = ircConnect(host,port,nick,nick)
	if sock then
		table.insert(clientConnections,{sock=sock,connected=false,host=host,port=port,nick=nick,channels={}})
	end
end
addEvent("onIRCConnectionRequest",true)
addEventHandler("onIRCConnectionRequest",getRootElement(),handleConnectionRequest)

function handleJoinRequest(host,channel)
	local conn = getPlayerIRCConnectionByHost(client,host)
	
	if conn then
--[[		for index,chan in ipairs(conn.channels) do
			if chan == channel then
				return
			end
		end]]
		
		ircJoin(conn.sock,channel)
	end
end
addEvent("onIRCJoinRequest",true)
addEventHandler("onIRCJoinRequest",getRootElement(),handleJoinRequest)

function handleSayRequest(host,channel,msg)
	local conn = getPlayerIRCConnectionByHost(client,host)
	
	if conn then
		ircSay(conn.sock,channel,msg)
	end
end
addEvent("onIRCSayRequest",true)
addEventHandler("onIRCSayRequest",getRootElement(),handleSayRequest)

function sendIRCDataToOwner(socket,event,...)
	for owner,sockTable in pairs(ircConnections) do
		for index,sockData in ipairs(sockTable) do
			if sockData.sock == socket then
				sockData.connected = true
				triggerClientEvent(owner,event,getRootElement(),sockData.host,...)
				return
			end
		end
	end
end

--[[function destroyAllConnections()        HANDLED BY IRC SCRIPT
	for player,sockTable in pairs(ircConnections) do
		for index,sock in ipairs(sockTable) do
			ircDisconnect(sock.sock,"Resource stopping...")
		end
	end
end
addEventHandler("onResourceStop",getRootElement(),destroyAllConnections)]]

-- Message relays

addEventHandler("onIRCConnect",getRootElement(),
	function(theSock)
		for player,sockTable in pairs(ircConnections) do
			for index,sock in ipairs(sockTable) do
				if sock.sock == theSock then
					sock.connected = true
					triggerClientEvent(player,"onIRCConnect",getRootElement(),sock.host,sock.nick)
					return
				end
			end
		end
	end
)

addEventHandler("onIRCMessage", root,
    function(socket, user, channel, message)
		sendIRCDataToOwner(socket,"onIRCMessage",user,channel,message)
    end
)

addEventHandler("onIRCAction", root,
	function(socket, user, channel, action)
		sendIRCDataToOwner(socket,"onIRCAction",user,channel,action)
	end
)

addEventHandler("onIRCJoin", root,
	function(socket, user, channel)
		sendIRCDataToOwner(socket,"onIRCJoin",user,channel)
	end
)

addEventHandler("onIRCPart", root,
	function(socket, user, channel)
		sendIRCDataToOwner(socket,"onIRCPart",user,channel)
	end
)

addEventHandler("onIRCQuit", root,
	function(socket, user, reason)
		sendIRCDataToOwner(socket,"onIRCQuit",user,reason)
	end
)

addEventHandler("onIRCMode", root,
	function(socket, user, channel, modes, targets)
		sendIRCDataToOwner(socket,"onIRCMode",user,channel,modes,targets)
	end
)

addEventHandler("onIRCNick", root,
	function(socket, user, newNick)
		sendIRCDataToOwner(socket,"onIRCNick",user,newNick)
	end
)

addEventHandler("onIRCNotice", root,
	function(socket, user, target, notice)
		sendIRCDataToOwner(socket,"onIRCNotice",user,target,notice)
	end
)

addEventHandler("onIRCUserListReceive", root,
	function(socket, channel, users)
		sendIRCDataToOwner(socket,"onIRCUserListReceive",channel,users)
	end
)

function destroyPlayerConnections(quitReason)
	if ircConnections[source] then
		for index,sock in ipairs(ircConnections[source]) do
			ircDisconnect(sock.sock,"Player quit ("..quitReason..")")
		end
		
		ircConnections[source] = nil
	end
end
addEventHandler("onPlayerQuit",getRootElement(),destroyPlayerConnections)