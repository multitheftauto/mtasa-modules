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
			fileWrite(logfile,"\n------ new log ------\n")
		else
			logfile = fileCreate("irc.log")
			fileWrite(logfile,"\n------ new log ------\n")
		end
		fileFlush(logfile)
	end
)

addEvent("onIRCRaw")
addEventHandler("onIRCRaw",root,
	function (data)
		fileWrite(logfile,data.."\n")
		fileFlush(logfile)
	end
)

addEventHandler("onResourceStop",resourceRoot,
	function ()
		fileFlush(logfile)
		fileClose(logfile)
	end
)

addEvent("onIRCConnecting")
addEventHandler("onIRCConnecting",root,
	function ()
		outputServerLog("IRC: connecting to '"..ircGetServerHost(source).."' on port "..ircGetServerPort(source).."...")
	end
)

addEvent("onIRCConnect")
addEventHandler("onIRCConnect",root,
	function ()
		outputServerLog(ircGetServerName(source)..": Connected as "..ircGetServerNick(source).."!")
	end
)

addEvent("onIRCFailConnect")
addEventHandler("onIRCFailConnect",root,
	function (reason)
		outputServerLog("IRC: could not connect to '"..ircGetServerName(source).."' ("..(reason or "unknown")..")")
	end
)

addEvent("onIRCUserJoin")
addEventHandler("onIRCUserJoin",root,
	function (channel,vhost)
		outputServerLog(ircGetServerName(ircGetUserServer(source))..": "..ircGetUserNick(source).." ("..vhost..") joined "..ircGetChannelName(channel))
	end
)

addEvent("onIRCUserPart")
addEventHandler("onIRCUserPart",root,
	function(channel,reason)
		if reason then
			outputServerLog(ircGetServerName(ircGetUserServer(source))..": "..ircGetUserNick(source).." has parted "..ircGetChannelName(channel).." ("..reason..")")
		else
			outputServerLog(ircGetServerName(ircGetUserServer(source))..": "..ircGetUserNick(source).." has parted "..ircGetChannelName(channel))
		end
	end
)

addEvent("onIRCUserChangeNick")
addEventHandler("onIRCUserChangeNick",root,
	function (oldnick,newnick)
		outputServerLog(ircGetServerName(ircGetUserServer(source))..": "..oldnick.." is now known as "..newnick)
	end
)

addEvent("onIRCPrivateMessage")
addEventHandler("onIRCPrivateMessage",root,
	function (message)
		outputServerLog(ircGetServerName(ircGetUserServer(source))..": PM from "..ircGetUserNick(source)..": "..message)
	end
)

addEvent("onIRCMessage")
addEventHandler("onIRCMessage",root,
	function (channel,message)
		outputServerLog(ircGetServerName(ircGetUserServer(source))..": ["..ircGetChannelName(channel).."] "..ircGetUserNick(source)..": "..message)
	end
)

addEvent("onIRCPrivateNotice")
addEventHandler("onIRCPrivateNotice",root,
	function (message)
		outputServerLog(ircGetServerName(ircGetUserServer(source))..": notice from "..ircGetUserNick(source)..": "..message)
	end
)

addEvent("onIRCNotice")
addEventHandler("onIRCNotice",root,
	function (channel,message)
		outputServerLog(ircGetServerName(ircGetUserServer(source))..": NOTICE ["..ircGetChannelName(channel).."] "..ircGetUserNick(source)..": "..message)
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
			outputServerLog(ircGetServerName(ircGetUserServer(source))..": ["..ircGetChannelName(channel).."] "..setter.." sets mode: +"..mode.." "..ircGetUserNick(source))
		else	
			outputServerLog(ircGetServerName(ircGetUserServer(source))..": ["..ircGetChannelName(channel).."] "..setter.." sets mode: -"..mode.." "..ircGetUserNick(source))
		end
	end
)

addEvent("onIRCChannelMode")
addEventHandler("onIRCChannelMode",root,
	function (positive,mode,setter)
		if positive then
			outputServerLog(ircGetServerName(ircGetChannelServer(source))..": ["..ircGetChannelName(source).."] "..(ircGetUserNick(setter) or "Server").." sets mode: +"..mode)
		else	
			outputServerLog(ircGetServerName(ircGetChannelServer(source))..": ["..ircGetChannelName(source).."] "..(ircGetUserNick(setter) or "Server").." sets mode: -"..mode)
		end
	end
)

addEvent("onIRCUserQuit")
addEventHandler("onIRCUserQuit",root,
	function (reason)
		if reason then
			outputServerLog(ircGetServerName(ircGetUserServer(source))..": "..ircGetUserNick(source).." quit ("..reason..")")
		else
			outputServerLog(ircGetServerName(ircGetUserServer(source))..": "..ircGetUserNick(source).." quit")
		end
	end
)

addEvent("aMessage",true)
addEventHandler("aMessage",root,
	function (type,t)
		if type == "new" then
			outputServerLog("ADMIN MESSAGE: new "..(t.category).." from "..getPlayerName(source)..". Subject: "..(t.subject))
		end
	end
)