---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.0
-- Date: 31.10.2010
---------------------------------------------------------------------

-- everything is saved here
servers = {} -- syntax: [server] = {element socket,string name,string host,string nick,string password,number port,bool secure,string nickservpass,string authident, string authpass,number lastping,timer timeoutguard,number reconnectattempts, table channels,bool connected,table buffer}
channels = {} -- syntax: [channel] = {string name,string mode,string topic,table users,string password,bool joined,bool echo}
users = {} -- syntax: [user] = {string name,string mode,string vhost,string email,string realname,table channels,table channelmodes}

------------------------------------
-- Loading
------------------------------------
addEventHandler("onResourceStart",resourceRoot,
	function ()
		-- Is the sockets module loaded?
		if not sockOpen then
			outputServerLog("IRC: could not start resource, the sockets module isn't loaded!")
			outputServerLog("IRC: restart the resource to retry")
			return
		end
		
		-- Parse functions file.
		local functionsFile = fileOpen("scripts/functions.txt",true)
		if functionsFile then
			for i,line in ipairs (split(fileRead(functionsFile,fileGetSize(functionsFile)),44)) do
				local functionName = gettok(line,2,32)
				_G[(functionName)] = function (...)
					local args = {...}
					for i,arg in ipairs (args) do
						local expectedArgType = gettok(line,(2+i),32)
						if type(arg) ~= expectedArgType and not string.find(expectedArgType,")") then
							outputServerLog("IRC: Bad argument #"..i.." @ '"..gettok(line,2,32).."' "..expectedArgType.." expected, got "..type(arg))
							return
						end
					end
					return _G[("func_"..functionName)](...)
				end
			end
			fileClose(functionsFile)
		else
			outputServerLog("IRC: could not start resource, the functions file can't be loaded!")
			outputServerLog("IRC: restart the resource to retry")
			return
		end
		
		-- Connect when needed.
		if get("irc-autoconnect") == "true" then
			internalConnect()
		end
	end
)

addEventHandler("onResourceStop",resourceRoot,
	function ()
		for i,server in ipairs (ircGetServers()) do
			ircDisconnect(server,"resource stopped")
		end
	end
)

function internalConnect ()
	-- Parse settings file.
	local settingsFile = xmlLoadFile("settings.xml")
	if settingsFile then
		for i,server in ipairs (xmlNodeGetChildren(settingsFile)) do
			local host = xmlNodeGetAttribute(server,"host")
			local nick = xmlNodeGetAttribute(server,"nick")
			local channels = xmlNodeGetAttribute(server,"channels")
			local port = tonumber(xmlNodeGetAttribute(server,"port")) or 6667
			local password = xmlNodeGetAttribute(server,"password") or false
			local secure = xmlNodeGetAttribute(server,"secure") or false
			local nspass = xmlNodeGetAttribute(server,"nickservpass") or false
			if not host then
				outputServerLog("IRC: problem with server #"..i..", no host given!")
			elseif not nick then
				outputServerLog("IRC: problem with server #"..i..", no nick given!")
			elseif not channels then
				outputServerLog("IRC: problem with server #"..i..", no channels given!")
			else
				local server = ircConnect(host,nick,port,password,secure)
				if nspass then
					ircIdentify(server,nspass)
				end
				for i,channel in ipairs (split(channels,44)) do
					ircJoin(server,gettok(channel,1,32),gettok(channel,2,32))
				end
			end
		end
		xmlUnloadFile(settingsFile)
	else
		outputServerLog("IRC: could not start resource, the settings.xml can't be parsed!")
		outputServerLog("IRC: restart the resource to retry")
		return
	end
end

------------------------------------
-- Socket events
------------------------------------
addEventHandler("onSockOpened",root,
	function (socket)
		for server,info in pairs (servers) do
			if info[1] == socket then
				servers[server][15] = true
				ircRaw(server,"USER echobot MCvarial MCv :Echobot by MCvarial");
				ircRaw(server,"NICK "..info[4])
				return
			end
		end
	end
)

addEventHandler("onSockData",root,
	function (socket,data)
		for server,info in pairs (servers) do
			if info[1] == socket then
				for i,line in ipairs (Split(data,"\n")) do
					if line ~= "" then
						triggerEvent("onIRCRaw",server,line)
					end
				end
				return
			end
		end
	end
)

addEventHandler("onSockClosed",root,
	function (socket)
		for server,info in pairs (servers) do
			if info[1] == socket then
				ircDisconnect(server,"Socket closed")
				return
			end
		end
	end
)

local ignoreraws = {}
ignoreraws["003"] = true
ignoreraws["004"] = true
ignoreraws["005"] = true
ignoreraws["006"] = true
ignoreraws["007"] = true
ignoreraws["008"] = true

ignoreraws["211"] = true
ignoreraws["212"] = true
ignoreraws["213"] = true
ignoreraws["214"] = true
ignoreraws["215"] = true
ignoreraws["216"] = true
ignoreraws["217"] = true -- might be used for ping freq

addEvent("onIRCRaw")
addEventHandler("onIRCRaw",root,
	function (data)
		local t = split(data,32)
		if t[1] == "PING" then
			resetTimer(servers[source][12])
			if t[2] then
				ircRaw(source,"PONG "..string.sub(t[2],2))
			else
				ircRaw(source,"PONG :REPLY")
			end
		elseif t[2] == "376" then
			triggerEvent("onIRCConnect",source)
		elseif t[2] == "001" then
			servers[source][2] = t[7]
		elseif t[2] == "002" then
			servers[source][3] = t[7]
		else
			outputServerLog("IRC: UKNOWN RAW: '"..data.."'")
		end
	end
)

addEvent("onIRCConnect")
addEventHandler("onIRCConnect",root,
	function ()
		for i,raw in ipairs (servers[source][16]) do
			ircRaw(source,raw)
		end
		servers[source][16] = {}
	end
)

------------------------------------
-- Functions
------------------------------------
function func_ircRaw (server,data)
	if servers[server] and servers[server][1] then
		if servers[server][15] then
			return sockWrite(servers[server][1],data.."\r\n")
		end
		table.insert(servers[server][16],data)
		return true
	end
	return false
end

function func_ircHop (channel,reason)
	if channels[channel] then
		local name = channels[channel][1]
		local password = channels[channel][6]
		if ircPart(channel) then
			return ircJoin(name,password)
		end
	end
	return false
end

function func_ircSay (target,message)
	local server = getElementParent(channel,0)
	local targetName = ircGetChannelName(target)
	if not targetName then
		targetName = ircGetUserName(target)
	end
	if server and targetName then
		return ircRaw(server,"PRIVMSG "..targetName.." :"..(message or "<no message>"))
	end
	return false
end

function func_ircPart (server,channel,reason)
	if servers[server] and channels[channel] then
		if getElementType(server) == "irc-server" then
			local channelName = ircGetChannelName(channel)
			if channelName then
				if reason then
					return ircRaw(server,"PART "..channelName.." :"..reason)
				else
					return ircRaw(server,"PART "..channelName)
				end
			end
		end
	end
	return false
end

function func_ircJoin (server,channel,password)
	if servers[server] then
		local channel = createElement("irc-channel")
		setElementParent(channel,server)
		if #getElementsByType("irc-channel") == 1 then
			channels[channel] = {channel,"+nst","Unknown",{},password,false,true}
		else
			channels[channel] = {channel,"+nst","Unknown",{},password,false,false}
		end
		if password then
			ircRaw(server,"JOIN "..channel.." :"..password)
		else
			ircRaw(server,"JOIN "..channel)
		end
		return channel
	else
		return false
	end
end

function func_ircAction (channel,message)
	local server = getElementParent(channel,0)
	local channelName = ircGetChannelName(channel)
	if server and channelName then
		return ircRaw(server,"ACTION "..channelName.." :"..(message or "<no message>"))
	end
	return false
end

function func_ircNotice (target,message)
	local server = getElementParent(channel,0)
	local targetName = ircGetChannelName(target)
	if not targetName then
		targetName = ircGetUserName(target)
	end
	if server and targetName then
		return ircRaw(server,"NOTICE "..targetName.." :"..(message or "<no message>"))
	end
	return false
end

function func_outputIRC (message)
	for channel,info in pairs (channels) do
		if info[8] then
			return ircRaw(getElementParent(channel,"PRIVMSG "..info[1].." :"..(message or "<no message>")))
		end
	end
	return false
end

function func_ircIdentify (server,password)
	return ircRaw(server,"PRIVMSG NickServ :IDENTIFY "..(password or ""))
end

function func_ircConnect (host,nick,port,password,secure)
	local server = createElement("irc-server")
	local socket = sockOpen(host,port,secure)
	local timer = setTimer(checkForTimeout,120000,0,server)
	if server and socket then
		triggerEvent("onIRCConnecting",server)
		servers[server] = {socket,host,host,nick,password,port,secure,false,false,false,getTickCount(),timer,0,{},false,{}}
		return server
	end
	return false
end

function func_ircReconnect (server)
	if servers[server] then
		servers[server][15] = false
		ircRaw(server,"QUIT :Reconnect")
		sockClose(servers[server][1])
		servers[server][1] = sockOpen(servers[server][2],servers[server][6],servers[server][7])
		return true
	end
	return false
end

function func_ircDisconnect (server,reason)
	if servers[server] then
		ircRaw(server,"QUIT :"..(reason or "Disconnect"))
		sockClose(servers[server][1])
		servers[server] = nil
		return destroyElement(server)
	end
	return false
end

function func_ircChangeNick (server,newnick)
	if servers[server] and type(newnick) == "string" then
		servers[server][4] = newnick
		return ircRaw(server,"NICK :"..newnick)
	end
	return false
end

function func_ircGetServers ()
	return getElementsByType("irc-server")
end
	
function func_ircGetServerName (server)
	if servers[server] then
		return servers[server][2]
	end
	return false
end

function func_ircGetServerHost (server)
	if servers[server] then
		return servers[server][3]
	end
	return false
end

function func_ircGetServerPort (server)
	if servers[server] then
		return servers[server][6]
	end
	return false
end

function func_ircGetServerPass (server)
	if servers[server] then
		return servers[server][5]
	end
	return false
end

function func_ircGetServerNick (server)
	if servers[server] then
		return servers[server][4]
	end
	return false
end

function func_ircIsServerSecure (server)
	if servers[server] then
		return servers[server][7]
	end
	return false
end

function func_ircGetServerChannels (server)
	if servers[server] then
		return servers[server][14]
	end
	return false
end

function func_ircSetChannelMode (channel,mode)
	if channels[channel] and type(mode) == "string" then
		return ircRaw(getElementParent(channel),"MODE "..channels[channel][1].." :"..mode)
	end
	return false
end

function func_ircGetChannelName (channel)
	if channels[channel] then
		return channels[channel][1]
	end
	return false
end

function func_ircGetChannelMode (channel)
	if channels[channel] then
		return channels[channel][2]
	end
	return false
end

function func_ircGetChannelUsers (channel)
	if channels[channel] then
		return channels[channel][4]
	end
	return false
end

function func_ircGetChannelTopic (channel)
	if channels[channel] then
		return channels[channel][3]
	end
	return false
end
	
function func_ircSetUserMode (user,channel,mode)
	if users[user] and channels[channel] and type(mode) == "string" then
		return ircRaw(getElementParent(user),"MODE "..channels[channel][1].." "..mode.." :"..users[user][1])
	end
	return  false
end

function func_ircIsUserBot (user)
	if users[user] then
		if string.find(users[user][2],"B") then
			return true
		end
	end
	return false
end

function func_ircGetUserMode (user)
	if users[user] then
		return users[user][2]
	end
	return false
end
-- syntax: [user] = {string name,string mode,string vhost,string email,string realname,table channels,table channelmodes}
function func_ircGetUserNick (user)
	if users[user] then
		return users[user][1]
	end
	return false
end

-- TODO
function func_ircGetUserLevel (user,channel)
	return 0
end

function func_ircGetUserVhost (user)
	if users[user] then
		return users[user][3]
	end
	return false
end

-- TODO
function func_ircIsUserSecure (user)
	return false
end

function func_ircGetUserRealName (user)
	if users[user] then
		return users[user][5]
	end
	return false
end

-- TODO
function func_ircGetUserChannels (user)
	return false
end

-- TODO
function func_ircGetUserChannelMode (user)
	return false
end

------------------------------------
-- Utility functions
------------------------------------
-- function copied from: http://lua-users.org/wiki/StringRecipes
function Split(str, delim, maxNb)
    -- Eliminate bad cases...
    if string.find(str, delim) == nil then
        return { str }
    end
    if maxNb == nil or maxNb < 1 then
        maxNb = 0    -- No limit
    end
    local result = {}
    local pat = "(.-)" .. delim .. "()"
    local nb = 0
    local lastPos
    for part, pos in string.gfind(str, pat) do
        nb = nb + 1
        result[nb] = part
        lastPos = pos
        if nb == maxNb then break end
    end
    -- Handle the last field
    if nb ~= maxNb then
        result[nb + 1] = string.sub(str, lastPos)
    end
    return result
end

function checkForTimeout (server)
	if not servers[server][15] then
		return ircReconnect(server)
	end
	if (getTickCount() - servers[server][11]) > 240000 then
		return ircReconnect(server)
	end
	ircRaw(server,"PING "..servers[server][3])
	--outputServerLog("Checking for timeout on server '"..tostring(server).."'")
end

function getAdminMessage (time,author)
	outputServerLog("Time: "..time..", Author: "..author)
	return "Hello, world!"
end