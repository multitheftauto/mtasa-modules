---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.6
-- Date: 31.10.2010
---------------------------------------------------------------------

------------------------------------
-- Handling
------------------------------------
addEventHandler("onSockOpened",root,
	function (socket)
		for i,server in ipairs (getElementsByType("irc-server")) do
			if servers[server].socket == socket then
				servers[server].connected = true
				if servers[server].password then
					ircRaw(server,"PASS "..servers[server].password)
				end
				ircRaw(server,"USER echobot MCvarial MCv :Echobot by MCvarial")
				ircRaw(server,"NICK "..servers[server].nick)
				return
			end
		end
	end
)

addEventHandler("onSockData",root,
	function (socket,data)
		if string.find(data,"Could not resolve your hostname: Domain name not found; using your IP address",0) then
			localIP = string.sub(gettok(data,18,32),2,-2)
		end
		for server,info in pairs (servers) do
			if info.socket == socket then
				info.inputbuffer = info.inputbuffer..data
				local start = string.find(info.inputbuffer,"\n",0,true)
				while start do
					triggerEvent("onIRCRaw",server,string.sub(info.inputbuffer,1,start-2))
					info.inputbuffer = string.sub(info.inputbuffer,start+1)
					start = string.find(info.inputbuffer,"\n",0,true)
				end
				return
			end
		end
	end
)

addEvent("onIRCRaw")
addEventHandler("onIRCRaw",root,
	function (data)
		servers[source].lastcontact = getTickCount()
		
		local t = split(data,32)
		if t[1] == "PING" then
			if t[2] then
				ircRaw(source,"PONG "..string.sub(t[2],2))
			else
				ircRaw(source,"PONG :REPLY")
			end
			servers[source].pinginterval = getTickCount()-servers[source].lastping
			servers[source].lastping = getTickCount()
		end
		if t[2] == "376" then
			triggerEvent("onIRCConnect",source)
		end
		if t[2] == "001" then
			servers[source].name = t[7]
			servers[source].connected = true
			if #servers[source].outputbuffer ~= 0 then
				for i,raw in ipairs (servers[source].outputbuffer) do
					ircRaw(source,raw)
				end
				servers[source].outputbuffer = {}
			end
			ircIdentify(source,servers[source].nickservpass,servers[source].nickservname)
		end
		if t[2] == "002" then
			servers[source].host = string.sub(t[7],1,-2)
		end
		if t[2] == "004" then
			servers[source].channelmodeswarg = t[8]
			servers[source].channelmodes = t[7]
			servers[source].usermodes = t[6]
		end
		if t[2] == "471" then
			servers[source].channelmodeswarg = t[8]
			servers[source].channelmodes = t[7]
			servers[source].usermodes = t[6]
		end
		if t[2] == "JOIN" then
			local nick = getNickFromRaw(data)
			local user = ircGetUserFromNick(nick,source)
			local channel = ircGetChannelFromName(t[3],source)
			local vhost = gettok(gettok(data,1,32),2,33)
			if not channel then
				channel = createElement("irc-channel")
				channels[channel] = {name=t[3],topic="Unknown",users={},password=false,echochannel=false}
				setElementParent(channel,source)
			end
			if not user then
				user = createElement("irc-user")
				users[user] = {name=nick,vhost=vhost,email=false,realname=false}
				setElementParent(user,source)
				channels[channel]["users"][user] = 0
			end
			users[user].vhost = vhost
			triggerEvent("onIRCUserJoin",user,channel,vhost)
		end
		if t[2] == "NICK" then
			local oldnick = getNickFromRaw(data)
			local newnick = string.sub(t[3],1,-2)
			local user = ircGetUserFromNick(oldnick,source)
			users[user].name = newnick
			triggerEvent("onIRCUserChangeNick",user,oldnick,newnick)
		end
		if t[2] == "PART" then
			local user = ircGetUserFromNick(getNickFromRaw(data),source)
			local reason = getMessageFromRaw(data)
			local channel = ircGetChannelFromName(t[3],source) or ircGetChannelFromName(string.sub(t[3],1,-2),source)
			channels[channel]["users"][user] = nil
			triggerEvent("onIRCUserPart",user,channel,reason)
			if #ircGetUserChannels(user) == 0 then
				users[user] = nil
				destroyElement(user)
			end
		end
		if t[2] == "KICK" then
			local user = ircGetUserFromNick(t[4],source)
			local reason = getMessageFromRaw(data)
			local channel = ircGetChannelFromName(t[3],source)
			local kicker = ircGetUserFromNick(getNickFromRaw(data),source)
			triggerEvent("onIRCUserKick",user,channel,reason,kicker)
			channels[channel]["users"][user] = nil
			triggerEvent("onIRCUserPart",user,channel,reason)
			if #ircGetUserChannels(user) == 0 then
				users[user] = nil
				destroyElement(user)
			end
			if t[4] == ircGetServerNick(source) then
				ircJoin(source,t[3])
			end
		end
		if t[2] == "353" then
			local nicks = split(getMessageFromRaw(data),32)
			local channel = ircGetChannelFromName(t[5],source)
			for i,nick in ipairs (nicks) do
				local nick,level = getNickAndLevel(nick)
				local user = ircGetUserFromNick(nick,source)
				if not user then
					user = createElement("irc-user")
					users[user] = {name=nick,vhost="Unknown",email=false,realname=false}
					setElementParent(user,source)
				end
				if channels[channel]["users"][user] ~= level then
					triggerEvent("onIRCLevelChange",user,channel,channels[channel]["users"][user],level)
					channels[channel]["users"][user] = level
				end
			end
		end
		if t[2] == "433" then
			triggerEvent("onIRCFailConnect",source,"Nickname already in use")
			ircChangeNick(source,t[4].."_")
			if servers[source].nickservpass then
				ircRaw(source,"PRIVMSG NickServ :GHOST "..t[4].." "..servers[source].nickservpass)
				ircChangeNick(source,t[4])
			end
		end
		if t[2] == "332" then
			local channel = ircGetChannelFromName(t[4],source)
			channels[channel].topic = getMessageFromRaw(data)
		end
		if t[2] == "PRIVMSG" then
			local user = ircGetUserFromNick(getNickFromRaw(data),source)
			local message = getMessageFromRaw(data)
			local channel = ircGetChannelFromName(t[3],source)
			if t[3] == ircGetServerNick(source) then
				triggerEvent("onIRCPrivateMessage",user,message)
			else
				triggerEvent("onIRCMessage",user,channel,message)
			end
		end
		if t[2] == "NOTICE" then
			local user = ircGetUserFromNick(getNickFromRaw(data),source)
			if user then
				if t[3] == ircGetServerNick(source) then
					triggerEvent("onIRCPrivateNotice",user,getMessageFromRaw(data))
				else
					triggerEvent("onIRCNotice",user,ircGetChannelFromName(t[3],source),getMessageFromRaw(data))
				end
			end
		end
		if t[2] == "QUIT" then
			local nick = getNickFromRaw(data)
			local user = ircGetUserFromNick(nick,source)
			triggerEvent("onIRCUserQuit",user,getMessageFromRaw(data))
			for i,channel in ipairs (ircGetUserChannels(user)) do
				channels[channel]["users"][user] = nil
			end
			users[user] = nil
			destroyElement(user)
			if nick == string.sub(ircGetServerNick(source),1,-2) then
				ircChangeNick(source,nick)
			end
		end
		if t[1] == "ERROR" then
			triggerEvent("onIRCFailConnect",source,getMessageFromRaw(data))
		end
		if t[2] == "MODE" then
			ircRaw(source,"NAMES "..t[3])
			if not servers[source].channelmodeswarg then return end
			local channel = ircGetChannelFromName(t[3],source)
			if not channel then return end
			local setter = ircGetUserFromNick(getNickFromRaw(data),source)
			local modes = toletters(string.gsub(t[4],"\r",""))
			local positive = true
			local n = 5
			for _,mode in ipairs (modes) do
				if mode == "-" then
					positive = false
				elseif mode == "+" then
					positive = true
				elseif string.find(servers[source].channelmodeswarg,mode,0,true) then
					local user = ircGetUserFromNick(t[n],source)
					if user then
						triggerEvent("onIRCUserMode",user,channel,positive,mode,setter)
					else
						triggerEvent("onIRCChannelMode",channel,positive,mode,setter,t[n])
					end
					n = n+1
				elseif string.find(servers[source].channelmodes,mode,0,true) then
					triggerEvent("onIRCChannelMode",channel,positive,mode,setter)
				elseif string.find(servers[source].usermodes,mode,0,true) then
					triggerEvent("onIRCUserMode",setter,channel,positive,mode,setter)
					n = n+1
				end
			end
		end
	end
)

setTimer(function ()
	for i,server in ipairs (ircGetServers()) do
		if servers[server].connected then
			local seen = getTickCount()-servers[server].lastcontact
			if seen > servers[server].pinginterval*1.2 then
				ircRaw(server,"PING")
			elseif seen > servers[server].pinginterval*2 then
				ircReconnect(server,"timed out")
			end
		end
	end
end,30000,0)