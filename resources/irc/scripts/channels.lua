---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.2
-- Date: 31.10.2010
---------------------------------------------------------------------

-- everything is saved here
channels = {} -- syntax: [channel] = {string name,string mode,string topic,table users,string password,bool joined,bool echo}

------------------------------------
-- Channels
------------------------------------
function func_ircGetChannelFromName (channel)
	for i,chan in ipairs (ircGetChannels()) do
		if string.lower(ircGetChannelName(chan)) == string.lower(channel) then
			return chan
		end
	end
	return false
end
registerFunction("ircGetChannelFromName","func_ircGetChannelFromName","string")

function ircGetEchoChannels ()
	local channels = {}
	for i,channel in ipairs (ircGetChannels()) do
		if ircIsEchoChannel(channel) then
			table.insert(channels,channel)
		end
	end
	return channels
end

function func_ircGetChannelServer (channel)
	return getElementParent(channel)
end
registerFunction("ircGetChannelServer","func_ircGetChannelServer","irc-channel")

function ircGetChannels (server)
	if servers[server] then
		local channels = {}
		for i,channels in ipairs (ircGetChannels()) do
			if ircGetChannelServer(channel) == server then
				table.insert(channels,channel)
			end
		end
		return channels
	end
	return getElementsByType("irc-channel")
end

function func_ircSetChannelMode (channel,mode)
	return ircRaw(getElementParent(channel),"MODE "..channels[channel][1].." :"..mode)
end
registerFunction("ircSetChannelMode","func_ircSetChannelMode","irc-channel","string")

function func_ircGetChannelName (channel)
	return channels[channel][1]
end
registerFunction("ircGetChannelName","func_ircGetChannelName","irc-channel")

function func_ircGetChannelMode (channel)
	return channels[channel][2]
end
registerFunction("ircGetChannelMode","func_ircGetChannelMode","irc-channel")

function func_ircGetChannelUsers (channel)
	return channels[channel][4]
end
registerFunction("ircGetChannelUsers","func_ircGetChannelUsers","irc-channel")

function func_ircGetChannelTopic (channel)
	return channels[channel][3]
end
registerFunction("ircGetChannelTopic","func_ircGetChannelTopic","irc-channel")

function func_ircIsEchoChannel (channel)
	return channels[channel][7]
end
registerFunction("ircIsEchoChannel","func_ircIsEchoChannel","irc-channel")