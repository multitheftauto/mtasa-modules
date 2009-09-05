--[[
	Info:      IRC Module basic script.
	Link:      http://development.mtasa.com/index.php?title=Modules/SebasIRC
	Author:    Sebas Lamers <sebasdevelopment@gmx.com>
]]--

local root = getRootElement()
local irc = {}

-- Vars (Edit these).............................
irc["server"] = "irc.mtasa.com"	-- IRC Adress
irc["port"] = 6667	-- IRC Port
irc["nickname"] = "MTABot"	-- Bot nickname
irc["channel"] = "#mta.test"	-- Default IRC log channel
irc["password"] = ""	-- NickServ password, or let it blanco!
irc["consolelog"] = true	-- True for logging things in server console or false for not

-- Don't edit here (But you can).................

--[[
	Add events
]]--
addEvent("onIRCJoin")
addEvent("onIRCPart")
addEvent("onIRCQuit")
addEvent("onIRCMessage")
addEvent("onIRCAction")

--[[
	IRC Events
]]--
addEventHandler("onIRCJoin", root,
	function(user, channel)
		if user == irc["nickname"] then return end
		local logmessage = "[IRC] * "..tostring(user).." has joined "..tostring(channel)
		
		outputChatBox(logmessage)
		
		if irc["consolelog"] then
			outputServerLog(logmessage)
		end
	end
)

addEventHandler("onIRCPart", root,
	function(user, channel)
		if user == irc["nickname"] then return end
		local logmessage = "[IRC] * "..tostring(user).." has left "..tostring(channel)
		
		outputChatBox(logmessage)
		
		if irc["consolelog"] then
			outputServerLog(logmessage)
		end
	end
)

addEventHandler("onIRCAction", root,
	function(channel, user, action)
		local logmessage = "[IRC] * "..tostring(user).." "..tostring(action)
		
		outputChatBox(logmessage)
		
		if irc["consolelog"] then
			outputServerLog(logmessage)
		end
	end
)

addEventHandler("onIRCMessage", root,
    function(channel, user, message)
        if string.find(message, "!boo") then
            ircSay(irc["channel"], "7Boo!")
        end
		
		if string.find(message, "!say") then
			outputChatBox("[IRC] "..tostring(channel).." <"..tostring(user).."> "..tostring(string.sub(message, 5)))
		end
		
		if irc["consolelog"] then
			outputServerLog("[IRC] "..tostring(channel).." <"..tostring(user).."> "..tostring(message))
		end
    end
)

--[[
	Resource events
]]--
addEventHandler("onResourceStart", root,
	function(res)
		if res == getThisResource() then
			ircDisconnect()
			if ircConnect(irc["server"], tonumber(irc["port"]), irc["nickname"]) then
				outputServerLog("Connected to irc!")
				ircSetMode("+B")
				ircSay("NickServ", "identify "..tostring(irc["password"]))
				ircJoin(irc["channel"])
			else
				outputServerLog("Cannot connect!")
			end
		end
	end
)

addEventHandler("onGamemodeStart",getRootElement(),
	function(res)
		local resName = getResourceInfo(res, "name") or getResourceName(res)

		ircSay(irc["channel"], "* Gamemode '"..resName.."' started.")
	end
)

addEventHandler("onGamemodeStop",getRootElement(),
	function(res)
		local resName = getResourceInfo(res, "name") or getResourceName(res)
		
		ircSay(irc["channel"], "* Gamemode '"..resName.."' stopped.")
	end
)

addEventHandler("onGamemodeMapStart",getRootElement(),
	function(res)
		local resName = getResourceInfo(res, "name") or getResourceName(res)
		
		ircSay(irc["channel"], "* Map '"..resName.."' started.")
	end
)

addEventHandler("onGamemodeMapStop",getRootElement(),
	function(res)
		local resName = getResourceInfo(res, "name") or getResourceName(res)
		
		ircSay(irc["channel"], "* Map '"..resName.."' stopped.")
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