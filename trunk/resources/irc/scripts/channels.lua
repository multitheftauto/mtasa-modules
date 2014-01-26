---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.6
-- Date: 31.10.2010
---------------------------------------------------------------------

-- everything is saved here
channels = {} -- syntax: [channel] = {name=string, topic=string, users=table, password=string, echochannel=bool}

------------------------------------
-- Channels
------------------------------------
function func_ircGetChannelFromName (channel,server)
	for i,chan in ipairs (ircGetChannels()) do
		if string.lower(ircGetChannelName(chan)) == string.lower(channel) and (getElementParent(chan) == server or not server) then
			return chan
		end
	end
	return false
end
registerFunction("ircGetChannelFromName","func_ircGetChannelFromName","string","(irc-server)")

function func_ircGetEchoChannels ()
	local channels = {}
	for i,channel in ipairs (ircGetChannels()) do
		if ircIsEchoChannel(channel) then
			table.insert(channels,channel)
		end
	end
	return channels
end
registerFunction("ircGetEchoChannels","func_ircGetEchoChannels")

function func_ircGetChannelServer (channel)
	return getElementParent(channel)
end
registerFunction("ircGetChannelServer","func_ircGetChannelServer","irc-channel")

function func_ircGetChannels (server)
	if servers[server] then
		local channels = {}
		for i,channel in ipairs (ircGetChannels()) do
			if ircGetChannelServer(channel) == server then
				table.insert(channels,channel)
			end
		end
		return channels
	end
	return getElementsByType("irc-channel")
end
registerFunction("ircGetChannels","func_ircGetChannels","(irc-server)")

function func_ircSetChannelMode (channel,mode)
	return ircRaw(getElementParent(channel),"MODE "..channels[channel][1].." :"..mode)
end
registerFunction("ircSetChannelMode","func_ircSetChannelMode","irc-channel","string")

function func_ircGetChannelName (channel)
	return channels[channel].name
end
registerFunction("ircGetChannelName","func_ircGetChannelName","irc-channel")

function func_ircGetChannelUsers (channel)
	local t = {}
	for user,level in pairs (channels[channel].users) do
		table.insert(t,user)
	end
	return t
end
registerFunction("ircGetChannelUsers","func_ircGetChannelUsers","irc-channel")

function func_ircGetChannelTopic (channel)
	return channels[channel].topic
end
registerFunction("ircGetChannelTopic","func_ircGetChannelTopic","irc-channel")

function func_ircGetChannelPassword (channel)
	return channels[channel].password
end
registerFunction("ircGetChannelPassword","func_ircGetChannelPassword","irc-channel")

function func_ircIsEchoChannel (channel)
	return channels[channel].echochannel
end
registerFunction("ircIsEchoChannel","func_ircIsEchoChannel","irc-channel")