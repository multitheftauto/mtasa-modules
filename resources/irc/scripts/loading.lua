---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.0
-- Date: 31.10.2010
---------------------------------------------------------------------

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
		
		-- Parse rights file.
		local rightsFile = fileOpen("scripts/rights.txt",true)
		if rightsFile then
			local missingRights = {}
			for i,line in ipairs (split(fileRead(rightsFile,fileGetSize(rightsFile)),44)) do
				line = string.sub(line,2)
				if not hasObjectPermissionTo(getThisResource(),"function."..line,true) then
					table.insert(missingRights,"function."..line)
				end
			end
			if #missingRights ~= 0 then
				outputServerLog("IRC: "..#missingRights.." missing rights: ")
				for i,missingRight in ipairs (missingRights) do
					outputServerLog("	- "..missingRight)
				end
				outputServerLog("Please give the irc resource these rights or it will not work properly!")
			end
		else
			outputServerLog("IRC: could not start resource, the rights file can't be loaded!")
			outputServerLog("IRC: restart the resource to retry")
			return
		end
		
		-- Is the resource up-to-date?
		function checkVersion (res,version)
			if res ~= "ERROR" and version then
				if getNumberFromVersion(version) > getNumberFromVersion(getResourceInfo(getThisResource(),"version")) then
					outputServerLog("IRC: resource is outdated, newest version: "..version)
					outputIRC("The irc resource is outdated, newest version: "..version)
				end
			end
		end
		callRemote("http://community.mtasa.com/mta/resources.php",checkVersion,"version","irc")
		
		-- Parse functions file.
		local functionsFile = fileOpen("scripts/functions.txt",true)
		if functionsFile then
			for i,line in ipairs (split(fileRead(functionsFile,fileGetSize(functionsFile)),44)) do
				if gettok(line,1,21) ~= "--" then
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
						if _G[("func_"..functionName)] then
							return _G[("func_"..functionName)](...)
						else
							outputServerLog("Function '"..functionName.."' does not exist")
							return false
						end
					end
				end
			end
			fileClose(functionsFile)
		else
			outputServerLog("IRC: could not start resource, the functions file can't be loaded!")
			outputServerLog("IRC: restart the resource to retry")
			return
		end
		
		--[[ parse events file
		local eventsFile = fileOpen("scripts/events.txt",true)
		if eventsFile then
			for i,line in ipairs (split(fileRead(eventsFile,fileGetSize(eventsFile)),44)) do
				if gettok(line,1,32) ~= "--" then
					addEvent(tostring(gettok(line,1,32)))
				end
			end
			fileClose(eventsFile)
		else
			outputServerLog("IRC: could not start resource, the events file can't be loaded!")
			outputServerLog("IRC: restart the resource to retry")
			return
		end
		]]
		
		local aclFile = xmlLoadFile("acl.xml")
		if  aclFile then
			local i = 0
			-- for each command we find
			while(xmlFindChild(aclFile,"command",i)) do
				local child = xmlFindChild(aclFile,"command",i)
				local attrs = xmlNodeGetAttributes(child)
				acl[attrs.name] = attrs
				i = i + 1
			end
			xmlUnloadFile(aclFile)
		else
			outputServerLog("IRC: could not start resource, the acl file can't be loaded!")
			outputServerLog("IRC: restart the resource to retry")
			return
		end

		addIRCCommands()
		internalConnect()
	end
)

addEventHandler("onResourceStop",resourceRoot,
	function ()
		for i,server in ipairs (ircGetServers()) do
			ircDisconnect(server,"resource stopped")
		end
		servers = {}
		channels = {}
		users = {}
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