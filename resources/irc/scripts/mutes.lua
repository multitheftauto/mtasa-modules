---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.2
-- Date: 31.10.2010
---------------------------------------------------------------------

executeSQLCreateTable("ircmutes","player TEXT, serial TEXT, reason TEXT, admin TEXT, mute TEXT, duration TEXT")
local mutes = {}

-- admin panel interaction
addEvent("aPlayer",true)
addEventHandler("aPlayer",root,
	function (player,action,reason,time)
		if isPlayerMuted(player) then
			executeSQLInsert("ircmutes","'"..tostring(getPlayerName(player)).."','"..tostring(getPlayerSerial(player)).."','"..tostring(reason).."','"..tostring(getPlayerName(source)).."','"..tostring(getRealTime().timestamp*1000).."','"..((time or 0)*1000).."'")
		else
			executeSQLDelete("ircmutes","serial = '"..tostring(getPlayerSerial(player)).."'")
		end
	end
)

_setPlayerMuted = setPlayerMuted
function setPlayerMuted (player,muted,reason,admin)
	if muted then
		local time = getTimeFromString(reason) or 0
		executeSQLInsert("ircmutes","'"..tostring(getPlayerName(player)).."','"..tostring(getPlayerSerial(player)).."','"..tostring(reason).."','"..tostring(admin).."','"..tostring(getRealTime().timestamp*1000).."','"..time.."'")
		if time > 50 then
			mutes[player] = setTimer(setPlayerMuted,time,1,player,false)
		end
	else
		executeSQLDelete("ircmutes","serial = '"..tostring(getPlayerSerial(player)).."'")
		if mutes[player] then
			mutes[player] = nil
		end
	end
	return _setPlayerMuted(player,muted)
end

addEventHandler("onPlayerUnmute",root,
	function ()
		executeSQLDelete("ircmutes","serial = '"..tostring(getPlayerSerial(source)).."'")
		if mutes[source] then
			mutes[source] = nil
		end
	end
)

addEventHandler("onPlayerJoin",root,
	function ()
		local result = executeSQLSelect("ircmutes","serial,reason,mute,duration","serial = '"..getPlayerSerial(source).."'")
		if result and result[1] then
			local time = (tonumber(result[1]["mute"])+tonumber(result[1]["duration"]))-(getRealTime().timestamp*1000)
			if time < 50 then
				executeSQLDelete("ircmutes","serial = '"..tostring(getPlayerSerial(source)).."'")
			else
				_setPlayerMuted(source,true)
				mutes[source] = setTimer(setPlayerMuted,time,1,source,false)
			end
		end
	end
)

addEventHandler("onPlayerQuit",root,
	function ()
		if mutes[source] then
			killTimer(mutes[source])
			mutes[source] = nil
			local result = executeSQLSelect("ircmutes","serial,reason,mute,duration","serial = '"..getPlayerSerial(source).."'")
			if result and result[1] then
				if result[1].duration == 0 then
					executeSQLDelete("ircmutes","serial = '"..tostring(getPlayerSerial(source)).."'")
				end
			end
		end
	end
)

addEventHandler("onResourceStart",resourceRoot,
	function ()
		local results = executeSQLSelect("ircmutes","serial,mute,duration")
		if type(results) == "table" then
			for i,result in ipairs (results) do
				local mutetime = tonumber(result["mute"])
				local duration = tonumber(result["duration"])
				if mutetime and duration and (mutetime+duration) < (getRealTime().timestamp)*1000 then
					for i,player in ipairs (getElementsByType("player")) do
						if getPlayerSerial(player) == result["serial"] then
							_setPlayerMuted(player,false)
							outputChatBox("* "..getPlayerName(player).." has been unmuted",root,255,0,0)
						end
					end
					executeSQLDelete("ircmutes","serial = '"..tostring(result["serial"]).."'")
				end
			end
		end
	end
)

local times = {}
times["ms"] = 1
times["sec"] = 1000
times["secs"] = 1000
times["second"] = 1000
times["seconds"] = 1000
times["min"] = 60000
times["mins"] = 60000
times["minute"] = 60000
times["minutes"] = 60000
times["hour"] = 3600000
times["hours"] = 3600000
times["day"] = 86400000
times["days"] = 86400000
times["week"] = 604800000
times["weeks"] = 604800000
times["month"] = 2592000000
times["months"] = 2592000000
function getTimeFromString (string)
	if type(string) ~= "string" then return false end
	string = string.gsub(string,"[%(%)]"," ") -- support for old syntax

	local time = 0
	local words = split(string,32)
	for i,word in ipairs (words) do
		if times[word] and words[i-1] then
			time = time + tonumber(words[i-1])*times[word]
		end
	end
	return time
end