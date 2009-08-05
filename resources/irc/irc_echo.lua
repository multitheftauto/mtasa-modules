--[[
	Info:      IRC Module basic script.
	Link:      http://development.mtasa.com/index.php?title=Modules/SebasIRC
	Author:    Sebas Lamers <baslamers@home.nl>
]]--

local root =  root
local irc = {}
irc["server"] = "irc.mtasa.com"
irc["port"] = 6667
irc["nickname"] = "MTABot"
irc["channel"] = "#mta.modules"
irc["password"] = ""

addEventHandler("onResourceStart", root,
	function(res)
		if res == getThisResource() then
			ircDisconnect()
			if ircConnect(irc["server"], tonumber(irc["port"]), irc["nickname"]) then
				outputServerLog("Connected to irc!")
				ircSetMode("+B")
				ircSay("NickServ", "identify "..tostring(irc["password"]))
				ircJoin(irc["channel"])
--				ircSay(irc["channel"], "Hello, I started server ["..getServerName().."], max players: "..getMaxPlayers())
			else
				outputServerLog("Cannot connect!")
			end
--		else
--			ircSay(irc["channel"], "* "..getResourceName(res).." loaded!")
		end
	end
)

--[[addEventHandler("onResourceStop", root,
	function(res)
		if res == getThisResource() then
			ircDisconnect()
		else
			ircSay(irc["channel"], "* "..getResourceName(res).." was stopped!")
		end
	end
)]]

addEventHandler("onGamemodeStart", root,
	function(res)
		local resName = getResourceInfo(res, "name") or getResourceName(res)

		ircSay(irc["channel"], "* Gamemode '"..resName.."' started.")
	end
)

addEventHandler("onGamemodeStop", root,
	function(res)
		local resName = getResourceInfo(res, "name") or getResourceName(res)
		
		ircSay(irc["channel"], "* Gamemode '"..resName.."' stopped.")
	end
)

addEventHandler("onGamemodeMapStart", root,
	function(res)
		local resName = getResourceInfo(res, "name") or getResourceName(res)
		
		ircSay(irc["channel"], "* Map '"..resName.."' started.")
	end
)

addEventHandler("onGamemodeMapStop", root,
	function(res)
		local resName = getResourceInfo(res, "name") or getResourceName(res)
		
		ircSay(irc["channel"], "* Map '"..resName.."' stopped")
	end
)

addEventHandler("onPlayerJoin", root,
	function()
		ircSay(irc["channel"], "* "..getPlayerName(source).." has joined the server!")
	end
)

addEventHandler("onPlayerQuit", root,
	function(reason)		
		ircSay(irc["channel"], "* "..getPlayerName(source).." has left the server ["..reason.."]!")
	end
)

addEventHandler("onPlayerChangeNick", root,
	function(oldNick, newNick)
		ircSay(irc["channel"], "* "..oldNick.." is known as "..newNick)
	end
)

addEventHandler("onPlayerSpawn", root,
	function(x, y, z)
		ircSay(irc["channel"], "* "..getPlayerName(source).." has spawned at "..getZoneName(x, y, z))
	end
)

addEventHandler("onPlayerWasted", root,
	function(totalAmmo, killer, killerWeapon, bodypart)
		local player = source
		
		local weapon
		
		if killerWeapon then
			weapon = getWeaponNameFromID(killerWeapon)
		else
			weapon = ""
		end
		
		if killer then
			local killerName = getPlayerName(killer)
			
			if killer == player then
				ircSay(irc["channel"], "* "..killerName.." died ("..weapon..")!")
			else
				ircSay(irc["channel"], "* "..killerName.." killed "..getPlayerName(player).." ("..weapon..")!")
			end
		else
			ircSay(irc["channel"], "* "..getPlayerName(player).." has killed himself!")
		end
	end
)

addEventHandler("onPlayerChat", root,
	function(chatMessage, chatType)
		local player = source
	
		if chatType == 0 then -- Normal message
			ircSay(irc["channel"], getPlayerName(player)..": "..tostring(chatMessage))
		elseif chatType == 1 then -- /me
			ircSay(irc["channel"], "* "..getPlayerName(player).." "..tostring(chatMessage))
		end
	end
)