---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.3
-- Date: 31.10.2010
---------------------------------------------------------------------

local numberOfPlayers = 0
local players = {}
local eventsAdded = false
local events = {
"onIRCMessage",
"onIRCUserJoin",
"onIRCUserPart",
"onIRCUserQuit",
"onIRCLevelChange",
"onIRCUserChangeNick"
}

------------------------------------
-- Irc client
------------------------------------
addCommandHandler("irc",
	function (player)
		if players[player] then return end
		if get("irc-client") == "true" then
			if not eventsAdded then addEvents() end
			local channels = {}
			for i,channel in ipairs (ircGetChannels()) do
				local users = {}
				for i,user in ipairs (ircGetChannelUsers(channel)) do
					table.insert(users,{name=ircGetUserNick(user),level=ircGetUserLevel(user,channel)})
				end
				table.insert(channels,{name=ircGetChannelName(channel),users=users,topic=ircGetChannelTopic(channel)})
			end
			players[player] = true
			numberOfPlayers = numberOfPlayers+1
			triggerClientEvent(player,"ircStartClient",resourceRoot,channels)
		end
	end,false,false
)

addEvent("ircStopClient",true)
addEventHandler("ircStopClient",root,
	function (player)
		if players[player] then
			numberOfPlayers = numberOfPlayers-1
			players[player] = nil
		end
	end
)

addEvent("ircSendMessage",true)
addEventHandler("ircSendMessage",root,
	function (player,channel,message)
		local channel = ircGetChannelFromName(channel)
		if channel then
			ircSay(channel,getPlayerName(player)..": "..tostring(message))
		end
	end
)

addEventHandler("onPlayerQuit",root,
	function ()
		if players[source] then
			players[source] = nil
		end
	end
)

function triggerClientIRCEvent (event,...)
	local args = {...}
	for i,arg in ipairs (args) do
		if isElement(arg) then
			if getElementType(arg) == "irc-user" then args[i] = ircGetUserNick(arg) end
			if getElementType(arg) == "irc-channel" then args[i] = ircGetChannelName(arg) end
		end
	end
	for player,_ in pairs (players) do
		triggerClientEvent(player,event,resourceRoot,unpack(args))
	end
end

function addEvents ()
	eventsAdded = true
	for i,event in ipairs (events) do
		addEvent(event)
		addEventHandler(event,root,function (...) if numberOfPlayers > 0 then triggerClientIRCEvent(event,source,...) end end)
	end
end