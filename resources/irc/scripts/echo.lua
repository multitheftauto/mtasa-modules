---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.6
-- Date: 31.10.2010
---------------------------------------------------------------------

------------------------------------
-- Echo
------------------------------------
local messages = {}

addEventHandler("onResourceStart",root,
	function (resource)
		if get("*irc-onResourceStart") ~= "true" then return end
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
		if get("*irc-onResourceStop") ~= "true" then return end
		if getResourceInfo(resource,"type") ~= "map" then
			outputIRC("07* Resource '"..(getResourceName(resource) or "?").."' stopped!")
		end
	end
)

addEventHandler("onPlayerJoin",root,
	function ()
		if get("*irc-onPlayerJoin") ~= "true" then return end
		messages[source] = 0
		outputIRC("03*** "..getPlayerName(source).." joined the game.")
	end
)

addEventHandler("onPlayerQuit",root,
	function (quit,reason,element)
		if get("*irc-onPlayerQuit") ~= "true" then return end
		messages[source] = nil
		if reason then
			if element then
				outputIRC("02*** "..getPlayerName(source).." was "..quit.." from the game by "..getPlayerName(element).." ("..reason..")")
			else
				outputIRC("02*** "..getPlayerName(source).." was "..quit.." from the game by Console ("..reason..")")
			end
		else
			outputIRC("02*** "..getPlayerName(source).." left the game ("..quit..")")
		end
	end
)

addEventHandler("onPlayerChangeNick",root,
	function (oldNick,newNick)
		if get("*irc-onPlayerChangeNick") ~= "true" then return end
		setTimer(function (player,oldNick)
			local newNick = getPlayerName(player)
			if newNick ~= oldNick then
				outputIRC("13* "..oldNick.." is now known as "..newNick)
			end
		end,100,1,source,oldNick)
	end
)

addEventHandler("onPlayerMute",root,
	function (arg)
		if get("*irc-onPlayerMute") ~= "true" then return end
		if type(arg) ~= "nil" then return end
		local result = executeSQLSelect("ircmutes","serial,reason","serial = '"..getPlayerSerial(source).."'")
		if result and result[1] then
			local admin = result[1]["admin"] or "console"
			if result[1]["reason"] then
				outputIRC("12* "..getPlayerName(source).." has been muted by "..admin.." ("..result[1]["reason"]..")")
			else
				outputIRC("12* "..getPlayerName(source).." has been muted by "..admin)
			end
		else
			outputIRC("12* "..getPlayerName(source).." has been muted")
		end
	end
)

addEventHandler("onPlayerUnmute",root,
	function ()
		if get("*irc-onPlayerUnmute") ~= "true" then return end
		outputIRC("12* "..getPlayerName(source).." has been unmuted")
	end
)

addEventHandler("onPlayerChat",root,
	function (message,type)
		if get("*irc-onPlayerChat") ~= "true" then return end
		messages[source] = messages[source] + 1
		if type == 0 then
			outputIRC("07"..getPlayerName(source)..": "..message)
		elseif type == 1 then
			outputIRC("06* "..getPlayerName(source).." "..message)
		elseif type == 2 then
			local team = getPlayerTeam(source)
			if not team then return end
			if get("*irc-logteammessages") == "/" then return end
			if get("*irc-logteammessages") == "*" then
				outputIRC("07("..getTeamName(team)..")"..getPlayerName(source)..": "..message)
			else
				for i,channel in pairs (ircGetChannels()) do
					if ircIsEchoChannel(channel) then
						local server = getElementParent(channel)
						ircRaw(server,"PRIVMSG "..tostring(get("*irc-logteammessages"))..ircGetChannelName(channel).." :07("..getTeamName(team)..")"..getPlayerName(source)..": "..message)
					end
				end
			end
		end
	end
)

addEventHandler("onSettingChange",root,
	function (setting,oldValue,newValue)
		if get("*irc-onSettingChange") ~= "true" then return end
		outputIRC("6Setting '"..tostring(setting).."' changed: "..tostring(oldValue).." -> "..tostring(newValue))
	end
)

local bodyparts = {nil,nil,"Torso","Ass","Left Arm","Right Arm","Left Leg","Right Leg","Head"}
local weapons = {}
weapons[19] = "Rockets"
weapons[88] = "Fire"
addEventHandler("onPlayerWasted",root,
	function (ammo,killer,weapon,bodypart)
		if get("*irc-onPlayerWasted") ~= "true" then return end
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

addEvent("onPlayerRaceWasted")
addEventHandler("onPlayerRaceWasted",root,
	function (vehicle)
		if get("*irc-onPlayerRaceWasted") ~= "true" then return end
		if #getAlivePlayers() == 1 and currentmode ~= "Sprint" then
			outputIRC("12* "..getPlayerName(getAlivePlayers()[1]).." won the deathmatch!")
		end
	end
)
		
addEvent("onPlayerFinish",true)
addEventHandler("onPlayerFinish",root,
	function (rank,time)
		if get("*irc-onPlayerFinish") ~= "true" then return end
		outputIRC("12* "..getPlayerName(source).." finished (rank: "..rank.." time: "..msToTimeStr(time)..")")
	end
)

addEvent("onGamemodeMapStart",true)
addEventHandler("onGamemodeMapStart",root,
	function (res)
		if get("*irc-onGamemodeMapStart") ~= "true" then return end
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
		if get("*irc-onPlayerToptimeImprovement") ~= "true" then return end
		if newPos == 1 then
			outputIRC("07* New record: "..msToTimeStr(newTime).." by "..getPlayerName(source).."!")
		end
	end
)

addEventHandler("onBan",root,
	function (ban)
		if get("*irc-onBan") ~= "true" then return end
		outputIRC("12* Ban added by "..(getPlayerName(source) or "Console")..": name: "..(getBanNick(ban) or "/")..", ip: "..(getBanIP(ban) or "/")..", serial: "..(getBanSerial(ban) or "/")..", banned by: "..(getBanAdmin(ban) or "/").." banned for: "..(getBanReason(ban) or "/"))
	end
)

addEventHandler("onUnban",root,
	function (ban)
		if get("*irc-onUnban") ~= "true" then return end
		outputIRC("12* Ban removed by "..(getPlayerName(source) or "Console")..": name: "..(getBanNick(ban) or "/")..", ip: "..(getBanIP(ban) or "/")..", serial: "..(getBanSerial(ban) or "/")..", banned by: "..(getBanAdmin(ban) or "/").." banned for: "..(getBanReason(ban) or "/"))
	end
)

------------------------------------
-- Admin interaction
------------------------------------
addEvent("onPlayerFreeze")
addEventHandler("onPlayerFreeze",root,
	function (state)
		if get("*irc-onPlayerFreeze") ~= "true" then return end
		if state then
			outputIRC("12* "..getPlayerName(source).." was frozen!")
		else
			outputIRC("12* "..getPlayerName(source).." was unfrozen!")
		end
	end
)

addEvent("aMessage",true)
addEventHandler("aMessage",root,
	function (Type,t)
		if get("*irc-adminMessage") ~= "true" then return end
		if Type ~= "new" then return end
		
		for i,channel in ipairs (ircGetEchoChannels()) do
			ircRaw(ircGetChannelServer(channel),"NOTICE %"..tostring(ircGetChannelName(channel)).." :New admin message by "..tostring(getPlayerName(source)))
			ircRaw(ircGetChannelServer(channel),"NOTICE %"..tostring(ircGetChannelName(channel)).." :Category: "..tostring(t.category))
			ircRaw(ircGetChannelServer(channel),"NOTICE %"..tostring(ircGetChannelName(channel)).." :Subject: "..tostring(t.subject))
			ircRaw(ircGetChannelServer(channel),"NOTICE %"..tostring(ircGetChannelName(channel)).." :Message: "..tostring(t.message))
		end
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
		if get("*irc-onPollStuff") ~= "true" then return end
		if pollTitle then
			outputIRC("14* A vote was started ["..tostring(pollTitle).."]")
		end
	end
)

addEvent("onPollStop")
addEventHandler("onPollStop",root,
	function ()
		if get("*irc-onPollStuff") ~= "true" then return end
		if pollTitle then
			pollTitle = nil
			outputIRC("14* Vote stopped!")
		end
	end
)

addEvent("onPollEnd")
addEventHandler("onPollEnd",root,
	function ()
		if get("*irc-onPollStuff") ~= "true" then return end
		if pollTitle then
			pollTitle = nil
			outputIRC("14* Vote ended!")
		end
	end
)

addEvent("onPollDraw")
addEventHandler("onPollDraw",root,
	function ()
		if get("*irc-onPollStuff") ~= "true" then return end
		if pollTitle then
			pollTitle = nil
			outputIRC("14* A draw was reached!")
		end
	end
)