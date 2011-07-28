---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.0
-- Date: 31.10.2010
---------------------------------------------------------------------

------------------------------------
-- Echo
------------------------------------
local messages = {}

addEventHandler("onResourceStart",root,
	function (resource)
		if getResourceInfo(resource,"type") ~= "map" then
			outputIRC("07* Resource '"..getResourceName(resource).."' started!")
		end
		if resource == getThisResource() then
			for i,player in ipairs (getElementsByType("player")) do
				messages[player] = 0
			end
		end
	end
)

addEventHandler("onResourceStop",root,
	function (resource)
		if getResourceInfo(resource,"type") ~= "map" then
			outputIRC("07* Resource '"..(getResourceName(resource) or "?").."' stopped!")
		end
	end
)

addEventHandler("onPlayerJoin",root,
	function ()
		messages[source] = 0
		outputIRC("03*** "..getPlayerName(source).." joined the game.")
	end
)

addEventHandler("onPlayerQuit",root,
	function (quit,reason,element)
		messages[source] = nil
		if reason then
			outputIRC("02*** "..getPlayerName(source).." was "..quit.." from the game by "..getPlayerName(element).." ("..reason..")")
		else
			outputIRC("02*** "..getPlayerName(source).." left the game ("..quit..")")
		end
	end
)

addEventHandler("onPlayerChangeNick",root,
	function (oldNick,newNick)
		setTimer(function (player,oldNick)
			local newNick = getPlayerName(player)
			if newNick ~= oldNick then
				outputIRC("13* "..oldNick.." is now known as "..newNick)
			end
		end,100,1,source,oldNick)
	end
)

addEventHandler("onPlayerMute",root,
	function ()
		outputIRC("12* "..getPlayerName(source).." has been muted")
	end
)

addEventHandler("onPlayerUnmute",root,
	function ()
		outputIRC("12* "..getPlayerName(source).." has been unmuted")
	end
)

addEventHandler("onPlayerChat",root,
	function (message,type)
		messages[source] = messages[source] + 1
		if type == 0 then
			outputIRC("07"..getPlayerName(source)..": "..message)
		elseif type == 1 then
			outputIRC("06* "..getPlayerName(source).." "..message)
		elseif type == 2 then
			outputIRC("07(TEAM)"..getPlayerName(source)..": "..message)
		end
	end
)

-- anti-spam
setTimer(function ()
	for player,number in pairs (messages) do
		if number >= 2 then
			muteSerial(getPlayerSerial(player),"Antispam triggered",180000)
			setPlayerMuted(player,true)
			outputChatBox("* "..getPlayerName(player).." was muted by irc (Antispam triggered)",root,0,0,255)
			ircSay(channel,"12* "..getPlayerName(player).." was muted by irc (Antispam triggered)")
		end
		messages[player] = 0
	end
end,3000,0)

addEventHandler("onSettingChange",root,
	function (setting,oldValue,newValue)
		outputIRC("6Setting '"..tostring(setting).."' changed: "..tostring(oldValue).." -> "..tostring(newValue))
	end
)

local bodyparts = {nil,nil,"Torso","Ass","Left Arm","Right Arm","Left Leg","Right Leg","Head"}
local weapons = {}
weapons[19] = "Rockets"
weapons[88] = "Fire"
addEventHandler("onPlayerWasted",root,
	function (ammo,killer,weapon,bodypart)
		if killer then
			if getElementType(killer) == "vehicle" then
				local driver = getVehicleController(killer)
				if driver then
					outputIRC("04* "..getPlayerName(source).." was killed by "..getPlayerName(driver).." in a "..getVehicleName(killer))
				else
					outputIRC("04* "..getPlayerName(source).." was killed by an "..getVehicleName(killer))
				end
			elseif getElementType(killer) == "player" then
				if weapon == 37 then
					if getPedWeapon(killer) ~= 37 then
						weapon = 88
					end
				end
				outputIRC("04* "..getPlayerName(source).." was killed by "..getPlayerName(killer).." ("..(getWeaponNameFromID(weapon) or weapons[weapon] or "?")..")("..bodyparts[bodypart]..")")
			else
				outputIRC("04* "..getPlayerName(source).." died")
			end
		else
			outputIRC("04* "..getPlayerName(source).." died")
		end
	end
)
		
addEvent("onPlayerFinish",true)
addEventHandler("onPlayerFinish",root,
	function (rank,time)
		outputIRC("12* "..getPlayerName(source).." finished (rank: "..rank.." time: "..msToTimeStr(time)..")")
	end
)

addEvent("onGamemodeMapStart",true)
addEventHandler("onGamemodeMapStart",root,
	function (res)
		outputIRC("12* Map started: "..(getResourceInfo(res, "name") or getResourceName(res)))
		local resource = getResourceFromName("mapratings")
		if resource and getResourceState(resource) == "running" and exports.mapratings:getMapRating(getResourceName(res)) and exports.mapratings:getMapRating(getResourceName(res)).average then
			outputIRC("07* Rating: "..exports.mapratings:getMapRating(getResourceName(res)).average)
		end
	end
)

addEvent("onPlayerToptimeImprovement",true)
addEventHandler("onPlayerToptimeImprovement",root,
	function (newPos,newTime,oldPos,oldTime)
		if newPos == 1 then
			outputIRC("07* New record: "..msToTimeStr(newTime).." by "..getPlayerName(source).."!")
		end
	end
)

addEventHandler("onBan",root,
	function (ban)
		outputIRC("12* Ban added by "..(getPlayerName(source) or "Console")..": name: "..(getBanNick(ban) or "/")..", ip: "..(getBanIP(ban) or "/")..", serial: "..(getBanSerial(ban) or "/")..", banned by: "..(getBanAdmin(ban) or "/").." banned for: "..(getBanReason(ban) or "/"))
	end
)

addEventHandler("onUnban",root,
	function (ban)
		outputIRC("12* Ban removed by "..(getPlayerName(source) or "Console")..": name: "..(getBanNick(ban) or "/")..", ip: "..(getBanIP(ban) or "/")..", serial: "..(getBanSerial(ban) or "/")..", banned by: "..(getBanAdmin(ban) or "/").." banned for: "..(getBanReason(ban) or "/"))
	end
)

addEvent("onPlayerRaceWasted")
addEventHandler("onPlayerRaceWasted",root,
	function (vehicle)
		if #getAlivePlayers() == 1 and currentmode ~= "Sprint" then
			outputIRC("12* "..getPlayerName(getAlivePlayers()[1]).." won the deathmatch!")
		end
	end
)

------------------------------------
-- Admin interaction
------------------------------------
addEvent("onPlayerFreeze")
addEventHandler("onPlayerFreeze",root,
	function (state)
		if state then
			outputIRC("12* "..getPlayerName(source).." was frozen!")
		else
			outputIRC("12* "..getPlayerName(source).." was unfrozen!")
		end
	end
)

addEvent("onPlayerMute")
addEventHandler("onPlayerMute",root,
	function (state)
		if state then
			outputIRC("12* "..getPlayerName(source).." was muted!")
		else
			outputIRC("12* "..getPlayerName(source).." was unmuted!")
		end
	end
)

addEvent("aMessage",true)
addEventHandler("aMessage",root,
	function (Type,t)
		if Type ~= "new" then return end
		
		local channel = ircGetEchoChannel()
		ircRaw(ircGetChannelServer(channel),"NOTICE %"..tostring(ircGetChannelName(channel)).." :New admin message by "..tostring(getPlayerName(source)))
		ircRaw(ircGetChannelServer(channel),"NOTICE %"..tostring(ircGetChannelName(channel)).." :Category: "..tostring(t.category))
		ircRaw(ircGetChannelServer(channel),"NOTICE %"..tostring(ircGetChannelName(channel)).." :Subject: "..tostring(t.subject))
		ircRaw(ircGetChannelServer(channel),"NOTICE %"..tostring(ircGetChannelName(channel)).." :Message: "..tostring(t.message))
	end
)

------------------------------------
-- Votemanager interaction
------------------------------------
local pollTitle

addEvent("onPollStarting")
addEventHandler("onPollStarting",root,
	function (data)
		if data.title then
			pollTitle = tostring(data.title)
		end
	end
)

addEvent("onPollModified")
addEventHandler("onPollModified",root,
	function (data)
		if data.title then
			pollTitle = tostring(data.title)
		end
	end
)

addEvent("onPollStart")
addEventHandler("onPollStart",root,
	function ()
		if pollTitle then
			outputIRC("14* A vote was started ["..tostring(pollTitle).."]")
		end
	end
)

addEvent("onPollStop")
addEventHandler("onPollStop",root,
	function ()
		if pollTitle then
			pollTitle = nil
			outputIRC("14* Vote stopped!")
		end
	end
)

addEvent("onPollEnd")
addEventHandler("onPollEnd",root,
	function ()
		if pollTitle then
			pollTitle = nil
			outputIRC("14* Vote ended!")
		end
	end
)

addEvent("onPollDraw")
addEventHandler("onPollDraw",root,
	function ()
		if pollTitle then
			pollTitle = nil
			outputIRC("14* A draw was reached!")
		end
	end
)