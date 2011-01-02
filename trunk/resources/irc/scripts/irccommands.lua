--------------------------------------------------------
-- Name: irc                                          --
-- Author: MCvarial <MCvarial@gmail.com>              --
-- Date: 02-05-2010                                   --
--------------------------------------------------------

------------------------------------
-- Echo
------------------------------------
function addIRCCommands ()

function say (server,channel,user,command,...)
	local message = table.concat({...}," ")
	if not message then ircNotice(user,"syntax is !s <message>") return end
	outputChatBox("* "..ircGetUserNick(user).." on irc: "..message,root,255,168,0)
	outputIRC("07* "..ircGetUserNick(user).." on irc: "..message)
end
addIRCCommandHandler("!say",say)
addIRCCommandHandler("!s",say)
addIRCCommandHandler("!m",say)

addIRCCommandHandler("!pm",
	function (server,channel,user,command,name,...)
		local message = table.concat({...}," ")
		if not name then ircNotice(user,"syntax is !pm <name> <message>") return end
		if not message then ircNotice(user,"syntax is !pm <name> <message>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			outputChatBox("* PM from "..ircGetUserNick(user).." on irc: "..message,player,255,168,0)
			ircNotice(user,"Your pm has been send to "..getPlayerName(player))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!kick",
	function (server,channel,user,command,name,...)
		if not name then ircNotice(user,"syntax is !kick <name> <reason>") return end
		local reason = table.concat({...}," ") or ""
		local player = getPlayerFromPartialName(name)
		if player then
			local nick = getPlayerName(player)
			kickPlayer(player,reason)
			outputChatBox("* "..nick.." was kicked from the game by "..ircGetUserNick(user).." ("..reason..")",root,255,100,100)
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!ban",
	function (server,channel,user,command,name,...)
		if not name then ircNotice(user,"syntax is !ban <name> [reason] (time)") return end
		local reason = table.concat({...}," ") or ""
		local t = split(reason,40)
		local time
		if #t > 1 then
			time = "("..t[#t]
		end
		local player = getPlayerFromPartialName(name)
		if player then
			if time then
				addBan(getPlayerIP(player),nil,getPlayerSerial(player),ircGetUserNick(user),reason,toMs(time)/1000)
			else
				addBan(getPlayerIP(player),nil,getPlayerSerial(player),ircGetUserNick(user),reason)
			end
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!mute",
	function (server,channel,user,command,name,...)
		if not name then ircNotice(user,"syntax is !mute <name> [reason] [(time)]") return end
		local reason = table.concat({...}," ") or ""
		local t = split(reason,40)
		local time
		if #t > 1 then
			time = "("..t[#t]
		end
		local player = getPlayerFromPartialName(name)
		if player then
			muteSerial(getPlayerSerial(player),reason,toMs(time))
			if reason then
				outputChatBox("* "..getPlayerName(player).." was muted by "..ircGetUserNick(user).." ("..reason..")",root,0,0,255)
				ircSay(channel,"12* "..getPlayerName(player).." was muted by "..ircGetUserNick(user).." ("..reason..")")
			else
				outputChatBox("* "..getPlayerName(player).." was muted by "..ircGetUserNick(user),root,0,0,255)
				ircSay(channel,"12* "..getPlayerName(player).." was muted by "..ircGetUserNick(user))
			end
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)
addIRCCommandHandler("!kill",
	function (server,channel,user,command,name,...)
		if not name then ircNotice(user,"syntax is !kill <name> [reason]") return end
		local reason = table.concat({...}," ") or ""
		local player = getPlayerFromPartialName(name)
		if player then
			killPed(player)
			outputChatBox("* "..getPlayerName(player).." was killed by "..ircGetUserNick(user).." ("..reason..")",root,0,0,255)
			ircSay(channel,"12* "..getPlayerName(player).." was killed by "..ircGetUserNick(user).." ("..reason..")")
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!unmute",
	function (server,channel,user,command,name,...)
		if not name then ircNotice(user,"syntax is !unmute <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			setPlayerMuted(player,false)
			outputChatBox("* "..getPlayerName(player).." was unmuted by "..ircGetUserNick(user),root,0,0,255)
			ircSay(channel,"12* "..getPlayerName(player).." was unmuted by "..ircGetUserNick(user))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!freeze",
	function (server,channel,user,command,name,...)
		if not name then ircNotice(user,"syntax is !freeze <name> [reason]") return end
		local reason = table.concat({...}," ") or ""
		local reason = table.concat({...}," ") or ""
		local t = split(reason,40)
		local time
		if #t > 1 then
			time = "("..t[#t]
		end
		local player = getPlayerFromPartialName(name)
		if player then
			if isPedInVehicle(player) then
				setVehicleFrozen(getPedOccupiedVehicle(player),true)
				setTimer(setVehicleFrozen,time,1,getPedOccupiedVehicle(player),false)
			end
			setPedFrozen(player,true)
			setTimer(setPedFrozen,time,1,player,false)
			outputChatBox("* "..getPlayerName(player).." was frozen by "..ircGetUserNick(user).." ("..reason..")",root,0,0,255)
			ircSay(channel,"12* "..getPlayerName(player).." was frozen by "..ircGetUserNick(user).." ("..reason..")")
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!unfreeze",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !unfreeze <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			if isPedInVehicle(player) then
				setVehicleFrozen(getPedOccupiedVehicle(player),false)
			end
			setPedFrozen(player,false)
			outputChatBox("* "..getPlayerName(player).." was unfrozen by "..ircGetUserNick(user),root,0,0,255)
			ircSay(channel,"12* "..getPlayerName(player).." was unfrozen by "..ircGetUserNick(user))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!slap",
	function (server,channel,user,command,name,hp,...)
		if not name then ircNotice(user,"syntax is !slap <name> <hp> [reason]") return end
		if not hp then ircNotice(user,"syntax is !slap <name> <hp> [reason]") return end
		local reason = table.concat({...}," ") or ""
		local player = getPlayerFromPartialName(name)
		if player then
			setElementVelocity((getPedOccupiedVehicle(player) or player),0,0,hp*0.01)
			setElementHealth((getPedOccupiedVehicle(player) or player),(getElementHealth((getPedOccupiedVehicle(player) or player)) - hp))
			outputChatBox("* "..getPlayerName(player).." was slaped by "..ircGetUserNick(user).." ("..reason..")("..hp.."HP)",root,0,0,255)
			ircSay(channel,"12* "..getPlayerName(player).." was slaped by "..ircGetUserNick(user).." ("..reason..")("..hp.."HP)")
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!getip",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !getip <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			ircNotice(user,getPlayerName(player).."'s IP: "..getPlayerIP(player))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!getserial",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !getserial <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			ircNotice(user,getPlayerName(player).."'s Serial: "..getPlayerSerial(player))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!unban",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !unban <name>") return end
		for i,ban in ipairs (getBans()) do
			if getBanNick(ban) == name then
				removeBan(ban)
			end
		end
	end
)

addIRCCommandHandler("!unbanip",
	function (server,channel,user,command,ip)
		if not ip then ircNotice(user,"syntax is !unbanip <ip>") return end
		for i,ban in ipairs (getBans()) do
			if getBanIP(ban) == ip then
				removeBan(ban)
			end
		end
	end
)

addIRCCommandHandler("!unbanserial",
	function (server,channel,user,command,serial)
		if not serial then ircNotice(user,"syntax is !unbanserial <serial>") return end
		for i,ban in ipairs (getBans()) do
			if getBanSerial(ban) == serial then
				removeBan(ban)
			end
		end
	end
)

addIRCCommandHandler("!banname",
	function (server,channel,user,command,name,...)
		if not name then ircNotice(user,"syntax is !banname <name> (<reason>)") return end
		local reason = table.concat({...}," ") or ""
		local t = split(reason,40)
		local time
		if #t > 1 then
			time = "("..t[#t]
		end
		if time then
			addBan(nil,name,nil,ircGetUserNick(user),reason,toMs(time)/1000)
		else
			addBan(nil,name,nil,ircGetUserNick(user),reason)
		end
	end
)

addIRCCommandHandler("!banserial",
	function (server,channel,user,command,serial,...)
		if not serial then ircNotice(user,"syntax is !banserial <name> (<reason>)") return end
		local reason = table.concat({...}," ") or ""
		local t = split(reason,40)
		local time
		if #t > 1 then
			time = "("..t[#t]
		end
		if time then
			addBan(nil,nil,serial,ircGetUserNick(user),reason,toMs(time)/1000)
		else
			addBan(nil,nil,serial,ircGetUserNick(user),reason)
		end
	end
)

addIRCCommandHandler("!banip",
	function (server,channel,user,command,ip,...)
		if not ip then ircNotice(user,"syntax is !banname <name> (<reason>)") return end
		local reason = table.concat({...}," ") or ""
		local t = split(reason,40)
		local time
		if #t > 1 then
			time = "("..t[#t]
		end
		if time then
			addBan(ip,nil,nil,ircGetUserNick(user),reason,toMs(time)/1000)
		else
			addBan(ip,nil,nil,ircGetUserNick(user),reason)
		end
	end
)

addIRCCommandHandler("!bans",
	function (server,channel)
		ircSay(channel,"There are "..#getBans().." bans on the server!")
	end
)

addIRCCommandHandler("!uptime",
	function (server,channel,user,command,...)
		ircNotice(user,"Hi "..ircGetUserNick(user)..", my uptime is: "..getTimeString(getTickCount()))
	end
)

addIRCCommandHandler("!players",
	function (server,channel)
		if getPlayerCount() == 0 then
			ircSay(channel,"There are no players ingame")
		else
			local players = getElementsByType("player")
			for i,player in ipairs (players) do
				players[i] = getPlayerName(player)
			end
			ircSay(channel,"There are "..getPlayerCount().." players ingame: "..table.concat(players," "))
		end
	end
)

addIRCCommandHandler("!run",
	function (server,channel,user,command,...)
		local str = table.concat({...}," ")
		if not str then ircNotice(user,"syntax is !run <string>") return end
		runString(str,root,ircGetUserNick(user))
	end
)

addIRCCommandHandler("!crun",
	function (server,channel,user,command,...)
		local t = {...}
		local str = table.concat(t," ")
		if not str then ircNotice(user,"syntax is !crun <string>") return end
		if #getElementsByType("player") == 0 then
			ircNotice(user,"No player ingame!")
		end
		for i,player in ipairs (getElementsByType("player")) do
			if i == 1 then
				triggerClientEvent(player,"doCrun",root,str,false)
			else
				triggerClientEvent(player,"doCrun",root,str,false)
			end
		end
	end
)

addIRCCommandHandler("!resources",
	function (server,channel,user,command)
		local resources = getResources()
		for i,resource in ipairs (resources) do
			if getResourceState(resource) == "running" then
				resources[i] = "03"..getResourceName(resource).."01"
			elseif getResourceState(resource) == "failed to load" then
				resources[i] = "04"..getResourceName(resource).." ("..getResourceLoadFailureReason(resource)..")01"
			else
				resources[i] = "07"..getResourceName(resource).."01"
			end
		end
		ircSay(channel,"07Resources: "..table.concat(resources,", "))
	end
)

addIRCCommandHandler("!start",
	function (server,channel,user,command,resName)
		if not resName then ircNotice(user,"syntax is !start <resourcename>") return end
		local resource = getResourceFromPartialName(resName)
		if resource then
			if not startResource(resource) then
				ircNotice(user,"Failed to start '"..getResourceName(resource).."'")
			end
		else
			ircNotice(user,"Resource '"..resName.."' not found!")
		end
	end
)

addIRCCommandHandler("!restart",
	function (server,channel,user,command,resName)
		if not resName then ircNotice(user,"syntax is !restart <resourcename>") return end
		local resource = getResourceFromPartialName(resName)
		if resource then
			if not restartResource(resource) then
				ircNotice(user,"Failed to restart '"..getResourceName(resource).."'")
			end
		else
			ircNotice(user,"Resource '"..resName.."' not found!")
		end
	end
)

addIRCCommandHandler("!stop",
	function (server,channel,user,command,resName)
		if not resName then ircNotice(user,"syntax is !stop <resourcename>") return end
		local resource = getResourceFromPartialName(resName)
		if resource then
			if not stopResource(resource) then
				ircNotice(user,"Failed to stop '"..getResourceName(resource).."'")
			end
		else
			ircNotice(user,"Resource '"..resName.."' not found!")
		end
	end
)
function outputCommands (server,channel,user,command)
	local cmds = {}
	for i,cmd in ipairs (ircGetCommands()) do
		if ircIsCommandEchoChannelOnly(cmd) then
			if ircIsEchoChannel(channel) then
				if ircGetCommandLevel(cmd) <= ircGetUserLevel(user,channel) then
					table.insert(cmds,cmd)
				end
			end
		else
			if ircGetCommandLevel(cmd) <= ircGetUserLevel(user,channel) then
				table.insert(cmds,cmd)
			end
		end
	end
	ircSay(channel,ircGetUserNick(user)..", you can use these commands: "..table.concat(cmds,", "))
end
addIRCCommandHandler("!commands",outputCommands)
addIRCCommandHandler("!cmds",outputCommands)

addIRCCommandHandler("!account",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !account <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			ircNotice(user,getPlayerName(player).."'s account name: "..(getAccountName(getPlayerAccount(player)) or "Guest Account"))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!money",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !money <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			ircNotice(user,getPlayerName(player).."'s money: "..tostring(getPlayerMoney(player)))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!health",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !health <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			ircNotice(user,getPlayerName(player).."'s health: "..tostring(getPlayerHealth(player)))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!wantedlevel",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !wantedlevel <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			outputIRC(getPlayerName(player).."'s wanted level: "..tostring(getPlayerWantedLevel(player)))
		else
			outputIRC("'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!team",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !team <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			local team = getPlayerTeam(player)
			if team then
				outputIRC(getPlayerName(player).."'s team: "..getTeamName(team))
			else
				outputIRC(getPlayerName(player).." is in no team")
			end
		else
			outputIRC("'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!ping",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !ping <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			outputIRC(getPlayerName(player).."'s ping: "..getPlayerPing(player))
		else
			outputIRC("'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!community",
	function (server,channel,user,command,name)
		if not name then ircNotice(user,"syntax is !community <name>") return end
		local player = getPlayerFromPartialName(name)
		if player then
			ircNotice(user,getPlayerName(player).."'s community account name: "..(getPlayerUserName(player) or "None"))
		else
			ircNotice(user,"'"..name.."' no such player")
		end
	end
)

addIRCCommandHandler("!changemap",
	function (server,channel,user,command,name,...)
		if not name then ircNotice(user,"syntax is !changemap <name>") return end
		local map = table.concat({...}," ")
		local resource = getResourceFromPartialName(map)
		if resource then
			exports.mapmanager:changeGameModeMap(resource)
		end
	end
)

addIRCCommandHandler("!modules",
	function (server,channel,user,command)
		ircSay(channel,"07Loaded modules: "..table.concat(getLoadedModules(),", "))
	end
)

end	