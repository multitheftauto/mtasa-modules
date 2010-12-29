---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.0
-- Date: 31.10.2010
---------------------------------------------------------------------

------------------------------------
-- Handling
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
		end
		if t[2] == "376" then
			triggerEvent("onIRCConnect",source)
		end
		if t[2] == "001" then
			servers[source][2] = t[7]
		end
		if t[2] == "002" then
			servers[source][3] = t[7]
		end
		if t[2] == "JOIN" then
			local nick = getNickFromRaw(data)
			local user = ircGetUserFromNick(nick)
			local channel = ircGetChannelFromName(getMessageFromRaw(data))
			local vhost = gettok(gettok(data,1,32),2,33)
			if nick == ircGetServerNick(source) then
				channels[channel][6] = true
			end
			if user then
				table.insert(users[user][6],channel)
			else
				user = createElement("irc-user")
				users[user] = {nick,"+iwxz",vhost,"?","?",{channel}}
				setElementParent(user,source)
			end
			triggerEvent("onIRCUserJoin",user,channel,vhost)
		end
		if t[2] == "NICK" then
			local oldnick = getNickFromRaw(data)
			local newnick = getMessageFromRaw(data)
			local user = ircGetUserFromNick(oldnick)
			users[user][1] = newnick
			triggerEvent("onIRCUserChangeNick",user,oldnick,newnick)
		end
		if t[2] == "PART" then
			local user = ircGetUserFromNick(getNickFromRaw(data))
			local reason = getMessageFromRaw(data)
			local channel = ircGetChannelFromName(string.sub(t[3],1,-2))
			for i,chan in ipairs (users[user][6]) do
				if channel == chan then
					table.remove(users[user][6],i)
					break
				end
			end
			triggerEvent("onIRCUserPart",user,channel,reason)
		end
		if t[2] == "353" then
			local nicks = split(getMessageFromRaw(data),32)
			local channel = ircGetChannelFromName(t[5])
			for i,nick in ipairs (nicks) do
				local nick,level = getNickAndLevel(nick)
				local user = ircGetUserFromNick(nick)
				if user then
					table.insert(users[user][6],channel)
				else
					user = createElement("irc-user")
					users[user] = {nick,"+iwxz","?","?","?",{channel}}
					setElementParent(user,source)
				end
				if not userlevels[channel] then
					userlevels[channel] = {}
				end
				userlevels[channel][user] = level
			end
		end
		if t[2] == "PRIVMSG" then
			local user = ircGetUserFromNick(getNickFromRaw(data))
			local message = getMessageFromRaw(data)
			local channel = ircGetChannelFromName(t[3])
			if t[3] == ircGetServerNick(source) then
				triggerEvent("onIRCPrivateMessage",user,message)
			else
				triggerEvent("onIRCMessage",user,channel,message)
			end
		end
		if t[2] == "NOTICE" then
			local user = ircGetUserFromNick(getNickFromRaw(data))
			if user then
				if t[3] == ircGetServerNick(source) then
					triggerEvent("onIRCPrivateNotice",user,getMessageFromRaw(data))
				else
					triggerEvent("onIRCNotice",user,ircGetChannelFromName(t[3]),getMessageFromRaw(data))
				end
			end
		end
		if t[2] == "QUIT" then
			local user = ircGetUserFromNick(getNickFromRaw(data))
			triggerEvent("onIRCUserQuit",user,getMessageFromRaw(data))
			users[user] = nil
			destroyElement(user)
		end
		if t[1] == "ERROR" then
			triggerEvent("onIRCFailConnect",source,getMessageFromRaw(data))
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