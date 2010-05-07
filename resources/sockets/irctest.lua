local botData = {
	host = "irc.gtanet.com",
	port = 6667,
	user = "Socketbot",
	nick = "Socketbot",
	channel = "#mta.dutch",
--	joinmsg = "Kiekeboe! :D",
	quitmsg = "/disconnect command used",
}

local root = getRootElement()

local IRCCommandHandlers = {}

local sock

function connect()
	if not ircIsConnected(sock) then
		sock = ircConnect(botData.host,botData.port,botData.user,botData.nick)
	end
end
addCommandHandler("connect",connect)

function disconnect()
	if ircIsConnected(sock) then
		ircDisconnect(sock,botData.quitmsg)
		sock = nil
	end
end
addCommandHandler("disconnect",disconnect)

function say(p,c,...)
	if ircIsConnected(sock) then
		ircSay(sock,botData.channel,table.concat({...}," "))
	end
end
addCommandHandler("irc",say)

function join(p,c,chan)
	if ircIsConnected(sock) and chan then
		outputDebugString("ircjoin "..tostring(chan))
		ircJoin(sock,chan)
	end
end
addCommandHandler("ircjoin",join)

function handleConnect(connectedSock)
	if connectedSock == sock then
		ircJoin(sock,botData.channel)
		
		if botData.joinmsg then
			ircSay(sock,botData.channel,botData.joinmsg)
		end
	end
end
addEventHandler("onIRCConnect",getRootElement(),handleConnect)

function addIRCCommandHandler(command,func,caseSens)
	if command and type(command)=="string" and func and type(func)=="function" then
		local handler
		
		if not IRCCommandHandlers[command] then
			handler={}
			IRCCommandHandlers[command]=handler
		else
			handler=IRCCommandHandlers[command]
		end
		
		if caseSens==nil then
			caseSens=true
		end
		
		handler[#handler+1]={func=func,caseSens=caseSens}
		
		return true
	end
	return false
end

addEventHandler("onIRCMessage", root,
    function(socket, user, channel, message)
		if socket == sock then
			local nick = ircGetUserNick(user)
			outputServerLog("IRC: "..nick.."("..channel.."): "..message)
			
			local argTable = split(message,32)
			
			for command,funcTable in pairs(IRCCommandHandlers) do
				local caseSensitive
				local continue
				
				if argTable[1] == command then
					continue      = true
					caseSensitive = false
				elseif string.lower(argTable[1])==string.lower(command) then
					continue      = true
					caseSensitive = true
				end
				
				if continue then
					table.remove(argTable,1)
					
					for index,func in ipairs(funcTable) do
						if not caseSensitive or not func.caseSens then
							func.func(socket,user,channel,command,unpack(argTable))
						end
					end
					
					break
				end
			end
		end
    end
)

addEventHandler("onIRCAction", root,
	function(socket, user, channel, action)
		if socket == sock then
			user = ircGetUserNick(user)
			outputServerLog("IRC: *"..user.."("..channel..") "..action)
		end
	end
)

addEventHandler("onIRCJoin", root,
	function(socket, user, channel)
		if socket == sock then
			user = ircGetUserNick(user)
			outputServerLog("IRC: "..user.." joined channel "..channel)
		end
	end
)

addEventHandler("onIRCPart", root,
	function(socket, user, channel)
		if socket == sock then
			user = ircGetUserNick(user)
			outputServerLog("IRC: "..user.." left channel "..channel)
		end
	end
)

addEventHandler("onIRCQuit", root,
	function(socket, user, reason)
		if socket == sock then
			user = ircGetUserNick(user)
			outputServerLog("IRC: "..user.." quit ("..reason..")")
		end
	end
)

addEventHandler("onIRCMode", root,
	function(socket, user, channel, modes, targets)
		if socket == sock then
--			user = ircGetUserNick(user)
			outputServerLog("IRC: "..user.." set mode "..modes.." for "..table.concat(targets,", ").." on channel "..channel)
		end
	end
)

addEventHandler("onIRCNick", root,
	function(socket, user, newNick)
		if socket == sock then
			user = ircGetUserNick(user)
			outputServerLog("IRC: "..user.." changed his nick to "..newNick)
		end
	end
)

addEventHandler("onIRCNotice", root,
	function(socket, user, target, notice)
		if socket == sock then
			user = ircGetUserNick(user)
			outputServerLog("IRC: NOTICE ("..user.." -> "..target.."): "..notice)
		end
	end
)

addEventHandler("onIRCUserListReceive", root,
	function(socket, channel, users)
		if socket == sock then
			outputServerLog("IRC: Users currently on channel "..channel..": "..table.concat(users," "))
		end
	end
)

addIRCCommandHandler("!hello",
	function(socket, user, channel, command, target)
		if target == "Testbot" then
--			sockWrite(socket,"PRIVMSG "..channel.." :Hi "..ircGetUserNick(user).."! :)\r\n")
			ircSay(socket,channel,"Hi "..ircGetUserNick(user).."! :)")
		end
	end
)

addIRCCommandHandler("!slap",
	function(socket, user, channel, command, target)
--		sockWrite(socket,"PRIVMSG "..channel.." :ACTION slaps "..target.."\r\n")
		ircAction(socket,channel,"slaps "..target)
	end
)

function runString (commandstring, source, outputTo)
	local sourceName=source
	ircSay(sock,outputTo,sourceName.." executed command: "..commandstring)
	local notReturned
	--First we test with return
	local commandFunction,errorMsg = loadstring("return "..commandstring)
	if errorMsg then
		--It failed.  Lets try without "return"
		notReturned = true
		commandFunction, errorMsg = loadstring(commandstring)
	end
	if errorMsg then
		--It still failed.  Print the error message and stop the function
--		ircSay(sock,outputTo,"Fout: "..errorMsg, outputTo)
		ircSay(sock,outputTo,"Error: "..errorMsg, outputTo)
		return
	end
	--Finally, lets execute our function
	results = { pcall(commandFunction) }
	if not results[1] then
		--It failed.
--		ircSay(sock,outputTo,"Fout: "..results[2], outputTo)
		ircSay(sock,outputTo,"Error: "..results[2], outputTo)
		return
	end
	if not notReturned then
		local resultsString = ""
		local first = true
		for i = 2, #results do
			if first then
				first = false
			else
				resultsString = resultsString..", "
			end
			local resultType = type(results[i])
			if isElement(results[i]) then
				resultType = "element:"..getElementType(results[i])
			end
			resultsString = resultsString..tostring(results[i]).." ["..resultType.."]"
		end
--		ircSay(sock,outputTo,"Commando resultaten: "..resultsString, outputTo)
		ircSay(sock,outputTo,"Command results: "..resultsString, outputTo)
	elseif not errorMsg then
--		ircSay(sock,outputTo,"Commando uitgevoerd!", outputTo)
		ircSay(sock,outputTo,"Command executed!", outputTo)
	end
end

-- run command
addIRCCommandHandler("!run",
	function (socket,user,channel,command,...)
		user = ircGetUserNick(user)
		local msg=table.concat({...}," ")
		return runString(msg, user, channel)
	end
)

-- silent run command
addIRCCommandHandler("!srun",
	function (socket,user,channel,command,...)
		user = ircGetUserNick(user)
		local msg = table.concat({...}, " ")
		return runString(msg, user, user)
	end
)