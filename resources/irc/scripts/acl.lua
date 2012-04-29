---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.2
-- Date: 31.10.2010
---------------------------------------------------------------------

commands = {}

------------------------------------
-- Acl
------------------------------------
function func_addIRCCommandHandler (cmd,fn,level,echoChannelOnly)
	if not level then level = 0 end
	if not echoChannelOnly then echoChannelOnly = true end
	if commands[string.lower(cmd)] and commands[string.lower(cmd)].fn then return false end
	if commands[string.lower(cmd)] then
		commands[string.lower(cmd)].fn = fn
	else
		commands[string.lower(cmd)] = {fn=fn,level=level,echoChannelOnly=echoChannelOnly,sourceResource=sourceResource}
	end
	return true
end
registerFunction("addIRCCommandHandler","func_addIRCCommandHandler","string","function/string","(number)","(boolean)")

function func_ircGetCommands ()
	local cmds = {}
	for cmd,_ in pairs (commands) do
		table.insert(cmds,cmd)
	end
	return cmds
end
registerFunction("ircGetCommands","func_ircGetCommands")

function func_ircGetCommandLevel (cmd)
	if commands[cmd] then
		return tonumber(commands[cmd].level)
	end
	return false
end
registerFunction("ircGetCommandLevel","func_ircGetCommandLevel","string")

function func_ircIsCommandEchoChannelOnly (cmd)
	if commands[cmd] then
		return commands[cmd].echoChannelOnly
	end
	return false
end
registerFunction("ircIsCommandEchoChannelOnly","func_ircIsCommandEchoChannelOnly","string")
	
addEvent("onIRCMessage")
addEventHandler("onIRCMessage",root,
	function (channel,message)
		local cmd = string.lower(gettok(message,1,32))
		local args = split(message,32)
		if commands[cmd] and commands[cmd].level <= (tonumber(ircGetUserLevel(source,channel)) or 0) then
			if commands[cmd].echoChannelOnly and not ircIsEchoChannel(channel) then return end
			if type(commands[cmd].fn) == "function" then
				commands[cmd].fn(ircGetChannelServer(channel),channel,source,unpack(args))
			else
				call(commands[cmd].sourceResource,commands[cmd].fn,ircGetChannelServer(channel),channel,source,unpack(args))
			end
		end
	end
)