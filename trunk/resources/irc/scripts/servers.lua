---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.3
-- Date: 31.10.2010
---------------------------------------------------------------------

-- everything is saved here
servers = {} -- syntax: [server] = {element socket,string name,string host,string nick,string password,number port,bool secure,string nickservpass,string authident, string authpass,number lastping,timer timeoutguard,number reconnectattempts, table channels,bool connected,table buffer}

------------------------------------
-- Servers
------------------------------------
function func_ircRaw (server,data)
	if servers[server][15] then
		writeLog("-> "..data)
		return sockWrite(servers[server][1],data.."\r\n")
	end
	table.insert(servers[server][16],data)
	return true
end
registerFunction("ircRaw","func_ircRaw","irc-server","string")

function func_ircHop (channel,reason)
	local name = channels[channel][1]
	local password = channels[channel][6]
	if ircPart(channel,reason) then
		return ircJoin(ircGetChannelServer(channel),name,password)
	end
	return false
end
registerFunction("ircHop","func_ircHop","irc-channel","(string)")

function func_ircSay (target,message)
	if #message > 400 then
		for i=1,math.ceil(#message/400) do
			ircSay(target,string.sub(message,(i-1)*400,i*400))
		end
		return true
	end
	local server = getElementParent(target)
	if not server then return false end
	local localuser = ircGetUserFromNick(ircGetServerNick(server))
	if not localuser then return false end
	if type(target) == "irc-channel" then
		triggerEvent("onIRCMessage",localuser,target,message)
		return ircRaw(server,"PRIVMSG "..ircGetChannelName(target).." :"..message)
	elseif type(target) == "irc-user" then
		triggerEvent("onIRCPrivateMessage",target,message)
		return ircRaw(server,"PRIVMSG "..ircGetUserNick(target).." :"..message)
	end
	return false
end
registerFunction("ircSay","func_ircSay","irc-channel/irc-user","string")

function func_ircPart (channel,reason)
	local channelName = ircGetChannelName(channel)
	if channelName then
		if reason then
			return ircRaw(ircGetChannelServer(channel),"PART "..channelName.." :"..reason)
		else
			return ircRaw(ircGetChannelServer(channel),"PART "..channelName)
		end
	end
	return false
end
registerFunction("ircPart","func_ircPart","irc-channel","string")

function func_ircJoin (server,channel,password)
	table.insert(servers[server][14],channel)
	local chan = createElement("irc-channel")
	setElementParent(chan,server)
	if #getElementsByType("irc-channel") == 1 then
		channels[chan] = {channel,"+nst","Unknown",{},password,false,true}
	else
		channels[chan] = {channel,"+nst","Unknown",{},password,false,false}
	end
	if password then
		ircRaw(server,"JOIN "..channel.." :"..password)
	else
		ircRaw(server,"JOIN "..channel)
	end
	ircRaw(server,"NAMES "..channel)
	return chan
end
registerFunction("ircJoin","func_ircJoin","irc-server","string","(string)")

function func_ircAction (channel,message)
	if #message > 400 then
		for i=1,math.ceil(#message/400) do
			ircAction(channel,string.sub(message,(i-1)*400,i*400))
		end
		return true
	end
	local server = getElementParent(channel,0)
	local channelName = ircGetChannelName(channel)
	if server and channelName then
		return ircRaw(server,"PRIVMSG "..channelName.." :ACTION "..message.."")
	end
	return false
end
registerFunction("ircAction","func_ircAction","irc-channel","string")

function func_ircNotice (target,message)
	if #message > 400 then
		for i=1,math.ceil(#message/400) do
			ircNotice(target,string.sub(message,(i-1)*400,i*400))
		end
		return true
	end
	local server = getElementParent(target,0)
	if not server then return false end
	if type(target) == "irc-channel" then
		return ircRaw(server,"NOTICE "..ircGetChannelName(target).." :"..message)
	elseif type(target) == "irc-user" then
		return ircRaw(server,"NOTICE "..ircGetUserNick(target).." :"..message)
	end
	return false
end
registerFunction("ircNotice","func_ircNotice","irc-channel/irc-user","string")

function func_outputIRC (message)
	if #message > 400 then
		for i=1,math.ceil(#message/400) do
			outputIRC(string.sub(message,(i-1)*400,i*400))
		end
		return true
	end
	if get("*irc-stripcolourcodes") == "true" then
		message = string.gsub(message,"#%x%x%x%x%x%x","")
	end
	for i,channel in pairs (ircGetChannels()) do
		if ircIsEchoChannel(channel) then
			local server = getElementParent(channel)
			local localuser = ircGetUserFromNick(ircGetServerNick(server))
			if server then
				if localuser then
					triggerEvent("onIRCMessage",localuser,channel,message)
				end
				return ircRaw(server,"PRIVMSG "..ircGetChannelName(channel).." :"..(message or "<no message>"))
			end
		end
	end
	return false
end
registerFunction("outputIRC","func_outputIRC","string")

function func_ircIdentify (server,password)
	servers[server][8] = password
	return ircRaw(server,"PRIVMSG NickServ :IDENTIFY "..(password or ""))
end
registerFunction("ircIdentify","func_ircIdentify","irc-server","string")

function func_ircConnect (host,nick,port,password,secure)
	local server = createElement("irc-server")
	local socket,sockError = sockOpen(host,(port or 6667),secure)
	if server and socket then
		servers[server] = {socket,host,host,nick,password,port,secure,false,false,false,getTickCount(),nil,0,{},false,{}}
		triggerEvent("onIRCConnecting",server)
		return server
	end
	if sockError then
		outputDebugString("irc failed to connect to "..tostring(host).." reason: "..getSocketErrorString(sockError))
	end
	return false
end
registerFunction("ircConnect","func_ircConnect","string","string","(number)","(string)","(boolean)")

function func_ircReconnect (server,reason)
	if type(reason) ~= "string" then
		reason = "reconnecting..."
	end
	triggerEvent("onIRCDisconnect",server,reason)
	ircRaw(server,"QUIT :"..reason)
	sockClose(servers[server][1])
	triggerEvent("onIRCConnecting",server)
	servers[server][15] = false
	servers[server][1] = sockOpen(ircGetServerHost(server),ircGetServerPort(server),ircIsServerSecure(server))
	if servers[server][8] then
		ircRaw(server,"PRIVMSG NickServ :IDENTIFY "..tostring(servers[server][8]))
	end
	for i,channel in ipairs (servers[server][14]) do
		ircRaw(server,"JOIN "..tostring(channel))
	end
	return true
end
registerFunction("ircReconnect","func_ircReconnect","irc-server","(string)")

function func_ircDisconnect (server,reason)
	ircRaw(server,"QUIT :"..(reason or "Disconnect"))
	sockClose(servers[server][1])
	servers[server] = nil
	return destroyElement(server)
end
registerFunction("ircDisconnect","func_ircDisconnect","irc-server","(string)")

function func_ircChangeNick (server,newnick)
	servers[server][4] = newnick
	return ircRaw(server,"NICK :"..newnick)
end
registerFunction("ircChangeNick","func_ircChangeNick","irc-server","string")

function ircGetServers ()
	return getElementsByType("irc-server")
end
	
function func_ircGetServerName (server)
	return servers[server][2]
end
registerFunction("ircGetServerName","func_ircGetServerName","irc-server")

function func_ircGetServerHost (server)
	return servers[server][3]
end
registerFunction("ircGetServerHost","func_ircGetServerHost","irc-server")

function func_ircGetServerPort (server)
	return servers[server][6]
end
registerFunction("ircGetServerPort","func_ircGetServerPort","irc-server")

function func_ircGetServerPass (server)
	return servers[server][5]
end
registerFunction("ircGetServerPass","func_ircGetServerPass","irc-server")

function func_ircGetServerNick (server)
	return servers[server][4]
end
registerFunction("ircGetServerNick","func_ircGetServerNick","irc-server")

function func_ircIsServerSecure (server)
	return servers[server][7]
end
registerFunction("ircIsServerSecure","func_ircIsServerSecure","irc-server")

function func_ircGetServerChannels (server)
	return servers[server][14]
end
registerFunction("ircGetServerChannels","func_ircGetServerChannels","irc-server")