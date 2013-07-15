---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.3
-- Date: 31.10.2010
---------------------------------------------------------------------

local adTimer
local rights = {
	"function.kickPlayer",
	"function.addBan",
	"function.removeBan",
	"function.getPlayerIP",
	"function.getPlayerSerial",
	"function.setPlayerFrozen",
	"function.setPlayerMuted",
	"function.killPed",
	"function.startResource",
	"function.stopResource",
	"function.restartResource",
	"function.callRemote",
	"function.fileOpen",
	"function.fileRead"
}

------------------------------------
-- Loading
------------------------------------
addEventHandler("onResourceStart",resourceRoot,
	function ()
		-- Is the sockets module loaded?
		if not sockOpen then
			outputDebugString("could not start the irc resource, the sockets module isn't loaded!")
			outputDebugString("try to start the resource again to retry")
			startupCancelled = true
			cancelEvent()
			return
		end
		
		-- Check if the resource has enough rights
		local missingRights = {}
		for i,right in ipairs (rights) do
			if not hasObjectPermissionTo(getThisResource(),right,true) then
				table.insert(missingRights,right)
			end
		end
		if #missingRights ~= 0 then
			outputDebugString("irc is missing "..#missingRights.." rights: ")
			for i,missingRight in ipairs (missingRights) do
				outputDebugString("	- "..missingRight)
			end
			outputDebugString("the irc resource does not have sufficient rights, type 'aclrequest allow irc all'")
			outputDebugString("try to start the resource again to retry")
			startupCancelled = true
			cancelEvent()
			return
		end
		
		-- Is the resource up-to-date?
		function checkVersion (res,version)
			if res ~= "ERROR" and version then
				if getNumberFromVersion(version) > getNumberFromVersion(getResourceInfo(getThisResource(),"version")) then
					outputDebugString("the irc resource is outdated, newest version: "..version)
					setTimer(outputIRC,10000,1,"The irc resource is outdated, newest version: "..version)
				end
			end
		end
		callRemote("http://community.mtasa.com/mta/resources.php",checkVersion,"version","irc")
		
		-- Start the ads.
		addEvent("onIRCPlayerDelayedJoin",true)
		if get("irc-notice") == "true" then
			local timeout = tonumber(get("irc-notice-timeout"))
			if timeout then
				if timeout == 0 then
					addEventHandler("onIRCPlayerDelayedJoin",root,showContinuousAd)
				else
					adTimer = setTimer(showAd,timeout*1000,0)
				end
			end
		end
		
		-- parse acl file
		local aclFile = xmlLoadFile("acl.xml")
		if  aclFile then
			local i = 0
			-- for each command we find
			while(xmlFindChild(aclFile,"command",i)) do
				local child = xmlFindChild(aclFile,"command",i)
				local attrs = xmlNodeGetAttributes(child)
				if commands[string.lower(attrs.name)] then
					commands[string.lower(attrs.name)].level = tonumber(attrs.level)
					commands[string.lower(attrs.name)].echoChannelOnly = attrs.echoChannelOnly
				else
					commands[string.lower(attrs.name)] = {level = tonumber(attrs.level),echoChannelOnly = attrs.echoChannelOnly}
				end
				i = i + 1
			end
			xmlUnloadFile(aclFile)
		else
			outputDebugString("could not start the irc resource, the acl file can't be loaded!")
			outputDebugString("try to start the resource again to retry")
			startupCancelled = true
			cancelEvent()
			return
		end

		triggerEvent("onIRCResourceStart",root)
		internalConnect()
		
		-- start irc addons
		for i,resource in ipairs (getResources()) do
			local info = getResourceInfo(resource,"addon")
			if info and info == "irc" then
				startResource(resource)
			end
		end
	end
)

addEventHandler("onResourceStop",resourceRoot,
	function ()
		for i,server in ipairs (ircGetServers() or {}) do
			ircDisconnect(server,"resource stopped")
		end
		servers = {}
		channels = {}
		users = {}
		
		-- stop irc addons
		for i,resource in ipairs (getResources()) do
			local info = getResourceInfo(resource,"addon")
			if info and info == "irc" then
				stopResource(resource)
			end
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
			local secure = xmlNodeGetAttribute(server,"secure") == "true" or false
			local nspass = xmlNodeGetAttribute(server,"nickservpass") or xmlNodeGetAttribute(server,"nickserverpass") or false
			if not host then
				outputDebugString("problem with irc server #"..i..", no host given!")
			elseif not nick then
				outputDebugString("problem with irc server #"..i..", no nick given!")
			elseif not channels then
				outputDebugString("problem with irc server #"..i..", no channels given!")
			else
				local server = ircConnect(host,nick,port,password,secure)
				if server then
					if nspass then
						ircIdentify(server,nspass)
					end
					for i,channel in ipairs (split(channels,44)) do
						ircJoin(server,gettok(channel,1,32),gettok(channel,2,32))
					end
				else
					outputDebugString("problem with irc server #"..i..", could not connect ("..tostring(getSocketErrorString(sockError)..")"))
				end
			end
		end
		xmlUnloadFile(settingsFile)
	else
		outputDebugString("could not start the irc resource, the settings.xml can't be parsed!")
		outputDebugString("try to start the resource again to retry")
		startupCancelled = true
		cancelEvent()
		return
	end
end

function showAd ()
	for i,channel in ipairs (ircGetChannels()) do
		if ircIsEchoChannel(channel) then
			--outputChatBox("Join us on irc! Server: "..ircGetServerHost(ircGetChannelServer(channel)).." Channel: "..ircGetChannelName(channel),root,0,255,0)
			triggerClientEvent("showAd",root,ircGetServerHost(ircGetChannelServer(channel)),ircGetChannelName(channel),tonumber(get("irc-notice-duration")))
			return
		end
	end
end

function showContinuousAd ()
	for i,channel in ipairs (ircGetChannels()) do
		if ircIsEchoChannel(channel) then
			triggerClientEvent(source,"showAd",root,ircGetServerHost(ircGetChannelServer(channel)),ircGetChannelName(channel),0)
			return
		end
	end
end

------------------------------------
-- Function argument check
------------------------------------

function registerFunction (name,ft,...)
	local arguments = {...}
	for i,arg in ipairs (arguments) do
		if string.sub(arg,1,1) == "(" and string.sub(arg,-1) == ")" then
			arguments[i] = "if args["..i.."] and type(args["..i.."]) ~= '"..string.sub(arg,2,-2).."' then return not outputDebugString('bad argument #"..i.." at "..name.." "..arg.." expected, got '..type(args["..i.."])) end"
		elseif string.find(arg,"/",0,true) then
			local args = split(arg,"/")
			local str = ""
			for k,arg in ipairs (args) do
				str = str.."type(args["..i.."]) ~= '"..arg.."' and "
			end
			str = string.sub(str,1,-5)
			arguments[i] = "if "..str.." then return not outputDebugString('bad argument #"..i.." at "..name.." "..arg.." expected, got '..type(args["..i.."])) end"
		else
			arguments[i] = "if type(args["..i.."]) ~= '"..arg.."' then return not outputDebugString('bad argument #"..i.." at "..name.." "..arg.." expected, got '..type(args["..i.."])) end"
		end
	end
	loadstring("function "..name.." (...) local args = {...} "..table.concat(arguments," ").." return "..ft.."(...) end")()
	--_G[name] = _G[ft]
end

_type = type
function type (element)
	local elementType = _type(element)
	if elementType == "userdata" and isElement(element) then
		return getElementType(element)
	end
	return elementType
end