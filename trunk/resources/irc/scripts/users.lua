---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.2
-- Date: 31.10.2010
---------------------------------------------------------------------

-- everything is saved here
users = {} -- syntax: [user] = {string name,string mode,string vhost,string email,string realname,table channels} (2nd,3rd,4th,5th argument not used)
	
------------------------------------
-- Users
------------------------------------
function func_ircSetUserMode (user,channel,mode)
	return ircRaw(getElementParent(user),"MODE "..channels[channel][1].." "..mode.." :"..users[user][1])
end
registerFunction("ircSetUserMode","func_ircSetUserMode","irc-user","irc-channel","string")

function func_ircGetUserMode (user)
	return users[user][2]
end
registerFunction("ircGetUserMode","func_ircGetUserMode","irc-user")

function func_ircGetUserNick (user)
	return users[user][1]
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
			if ircGetUserServer(user) == server then
				table.insert(users,user)
			end
		end
		return users
	else
		return getElementsByType("irc-user")
	end
end
registerFunction("ircGetUsers","func_ircGetUsers","(irc-server)")

function func_ircGetUserFromNick (nick)
	for i,user in ipairs (ircGetUsers()) do
		if string.lower(ircGetUserNick(user)) == string.lower(nick) then
			return user
		end
	end
	return false
end
registerFunction("ircGetUserFromNick","func_ircGetUserFromNick","string")

function func_ircGetUserVhost (user)
	return users[user][3]
end
registerFunction("ircGetUserVhost","func_ircGetUserVhost","irc-user")