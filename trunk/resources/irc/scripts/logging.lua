---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.0
-- Date: 31.10.2010
---------------------------------------------------------------------

local logfile

------------------------------------
-- Logging
------------------------------------
addEventHandler("onResourceStart",resourceRoot,
	function ()
		if fileExists("irc.log") then
			logfile = fileOpen("irc.log")
			fileSetPos(logfile,fileGetSize(logfile))
			writeLog("\n------ new log ------")
		else
			logfile = fileCreate("irc.log")
			writeLog("\n------ new log ------")
		end
	end
)

addEvent("onIRCRaw")
addEventHandler("onIRCRaw",root,
	function (data)
		writeLog(tostring(data))
	end
)

addEventHandler("onResourceStop",resourceRoot,
	function ()
		fileFlush(logfile)
		fileClose(logfile)
	end
)

function writeLog (text)
	fileWrite(logfile,getTimeStamp().." "..text.."\n")
	return fileFlush(logfile)
end

addEvent("onIRCConnecting")
addEventHandler("onIRCConnecting",root,
	function ()
		outputServerLog("IRC: connecting to '"..tostring(ircGetServerHost(source)).."' on port "..tostring(ircGetServerPort(source)).."...")
	end
)

addEvent("onIRCConnect")
addEventHandler("onIRCConnect",root,
	function ()
		outputServerLog(tostring(ircGetServerName(source))..": Connected as "..tostring(ircGetServerNick(source)).."!")
	end
)

addEvent("onIRCFailConnect")
addEventHandler("onIRCFailConnect",root,
	function (reason)
		outputServerLog("IRC: could not connect to '"..tostring(ircGetServerName(source)).."' ("..tostring(reason)..")")
	end
)

addEvent("onIRCDisconnect")
addEventHandler("onIRCDisconnect",root,
	function (reason)
		outputServerLog(tostring(ircGetServerName(source)).." disconnected! ("..tostring(reason)..")")
	end
)

addEvent("onIRCUserJoin")
addEventHandler("onIRCUserJoin",root,
	function (channel,vhost)
		outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": "..tostring(ircGetUserNick(source)).." ("..tostring(vhost)..") joined "..tostring(ircGetChannelName(channel)))
	end
)

addEvent("onIRCUserPart")
addEventHandler("onIRCUserPart",root,
	function(channel,reason)
		if reason then
			outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": "..tostring(ircGetUserNick(source)).." has parted "..tostring(ircGetChannelName(channel)).." ("..tostring(reason)..")")
		else
			outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": "..tostring(ircGetUserNick(source)).." has parted "..tostring(ircGetChannelName(channel)))
		end
	end
)

addEvent("onIRCUserKick")
addEventHandler("onIRCUserKick",root,
	function(channel,reason,kicker)
		if kicker then
			outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": "..tostring(ircGetUserNick(source)).." has been kicked from "..tostring(ircGetChannelName(channel)).." by "..tostring(ircGetUserNick(kicker)).." ("..tostring(reason)..")")
		else
			outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": "..tostring(ircGetUserNick(source)).." has been kicked from "..tostring(ircGetChannelName(channel)).." ("..tostring(reason)..")")
		end
	end
)

addEvent("onIRCUserChangeNick")
addEventHandler("onIRCUserChangeNick",root,
	function (oldnick,newnick)
		outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": "..tostring(oldnick).." is now known as "..tostring(newnick))
	end
)

addEvent("onIRCPrivateMessage")
addEventHandler("onIRCPrivateMessage",root,
	function (message)
		outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": PM from "..tostring(ircGetUserNick(source))..": "..tostring(message))
	end
)

addEvent("onIRCMessage")
addEventHandler("onIRCMessage",root,
	function (channel,message)
		outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": ["..tostring(ircGetChannelName(channel)).."] "..tostring(ircGetUserNick(source))..": "..tostring(message))
	end
)

addEvent("onIRCPrivateNotice")
addEventHandler("onIRCPrivateNotice",root,
	function (message)
		outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": notice from "..tostring(ircGetUserNick(source))..": "..tostring(message))
	end
)

addEvent("onIRCNotice")
addEventHandler("onIRCNotice",root,
	function (channel,message)
		outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": NOTICE ["..tostring(ircGetChannelName(channel)).."] "..tostring(ircGetUserNick(source))..": "..tostring(message))
	end
)

addEvent("onIRCUserMode")
addEventHandler("onIRCUserMode",root,
	function (channel,positive,mode,setter)
		if setter then
			setter = ircGetUserNick(setter)
		else
			setter = "Server"
		end
		if positive then
			outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": ["..tostring(ircGetChannelName(channel)).."] "..tostring(setter).." sets mode: +"..tostring(mode).." "..tostring(ircGetUserNick(source)))
		else	
			outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": ["..tostring(ircGetChannelName(channel)).."] "..tostring(setter).." sets mode: -"..tostring(mode).." "..tostring(ircGetUserNick(source)))
		end
	end
)

addEvent("onIRCChannelMode")
addEventHandler("onIRCChannelMode",root,
	function (positive,mode,setter)
		if positive then
			outputServerLog(tostring(ircGetServerName(ircGetChannelServer(source)))..": ["..tostring(ircGetChannelName(source)).."] "..(ircGetUserNick(setter) or "Server").." sets mode: +"..tostring(mode))
		else	
			outputServerLog(tostring(ircGetServerName(ircGetChannelServer(source)))..": ["..tostring(ircGetChannelName(source)).."] "..(ircGetUserNick(setter) or "Server").." sets mode: -"..tostring(mode))
		end
	end
)

addEvent("onIRCBan")
addEventHandler("onIRCBan",root,
	function (positive,vhost,setter)
		if positive then
			outputServerLog(tostring(ircGetServerName(ircGetChannelServer(source)))..": ["..tostring(ircGetChannelName(source)).."] "..(ircGetUserNick(setter) or "Server").." sets mode: +b "..tostring(vhost))
		else
			outputServerLog(tostring(ircGetServerName(ircGetChannelServer(source)))..": ["..tostring(ircGetChannelName(source)).."] "..(ircGetUserNick(setter) or "Server").." sets mode: -b "..tostring(vhost))
		end
	end
)

addEvent("onIRCUserQuit")
addEventHandler("onIRCUserQuit",root,
	function (reason)
		if reason then
			outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": "..tostring(ircGetUserNick(source)).." quit ("..tostring(reason)..")")
		else
			outputServerLog(tostring(ircGetServerName(ircGetUserServer(source)))..": "..tostring(ircGetUserNick(source)).." quit")
		end
	end
)

addEvent("aMessage",true)
addEventHandler("aMessage",root,
	function (type,t)
		if type == "new" then
			outputServerLog("ADMIN MESSAGE: new "..tostring(t.category).." from "..tostring(getPlayerName(source))..". Subject: "..tostring(t.subject))
		end
	end
)