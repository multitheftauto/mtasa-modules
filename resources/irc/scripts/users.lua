---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.6
-- Date: 31.10.2010
---------------------------------------------------------------------

-- everything is saved here
users = {} -- syntax: [user] = {name=string, vhost=string, email=string, realname=string}
	
------------------------------------
-- Users
------------------------------------
function func_ircSetUserMode (user,channel,mode)
	return ircRaw(getElementParent(user),"MODE "..channels[channel].name.." "..mode.." :"..users[user].name)
end
registerFunction("ircSetUserMode","func_ircSetUserMode","irc-user","irc-channel","string")

function func_ircGetUserChannels (user)
	local t = {}
	for i,channel in pairs (getElementsByType("irc-channel")) do
		if channels[channel]["users"][user] then
			table.insert(t,channel)
		end
	end
	return t
end
registerFunction("ircGetUserChannels","func_ircGetUserChannels","irc-user")

function func_ircGetUserNick (user)
	return users[user].name
end
registerFunction("ircGetUserNick","func_ircGetUserNick","irc-user")

function func_ircGetUserServer (user)
	return getElementParent(user)
end
registerFunction("ircGetUserServer","func_ircGetUserServer","irc-user")

function func_ircGetUsers (server)
	if server then
		local users = {}
		for i,user in ipairs (getElementsByType("irc-user")) do
			if getElementParent(user) == server then
				table.insert(users,user)
			end
		end
		return users
	else
		return getElementsByType("irc-user")
	end
end
registerFunction("ircGetUsers","func_ircGetUsers","(irc-server)")

function func_ircGetUserFromNick (nick,server)
	for i,user in ipairs (ircGetUsers()) do
		if string.lower(ircGetUserNick(user)) == string.lower(nick) and (getElementParent(user) == server or not server) then
			return user
		end
	end
	return false
end
registerFunction("ircGetUserFromNick","func_ircGetUserFromNick","string","(irc-server)")

function func_ircGetUserVhost (user)
	return users[user].vhost
end
registerFunction("ircGetUserVhost","func_ircGetUserVhost","irc-user")

function func_ircGetUserLevel (user,channel)
	return channels[channel]["users"][user] or 0
end
registerFunction("ircGetUserLevel","func_ircGetUserLevel","irc-user","irc-channel")