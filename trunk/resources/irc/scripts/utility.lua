---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.0
-- Date: 31.10.2010
---------------------------------------------------------------------

------------------------------------
-- Utility
------------------------------------
-- function copied from: http://lua-users.org/wiki/StringRecipes
function Split(str, delim, maxNb)
    -- Eliminate bad cases...
    if string.find(str, delim) == nil then
        return { str }
    end
    if maxNb == nil or maxNb < 1 then
        maxNb = 0    -- No limit
    end
    local result = {}
    local pat = "(.-)" .. delim .. "()"
    local nb = 0
    local lastPos
    for part, pos in string.gfind(str, pat) do
        nb = nb + 1
        result[nb] = part
        lastPos = pos
        if nb == maxNb then break end
    end
    -- Handle the last field
    if nb ~= maxNb then
        result[nb + 1] = string.sub(str, lastPos)
    end
    return result
end

function checkForTimeout (server)
	if not servers[server][15] then
		return ircReconnect(server)
	end
	if (getTickCount() - servers[server][11]) > 240000 then
		return ircReconnect(server)
	end
	ircRaw(server,"PING "..servers[server][3])
end

function getAdminMessage (time,author)
	outputServerLog("Time: "..time..", Author: "..author)
	return "Hello, world!"
end

function getNickFromRaw (raw)
	return string.sub(gettok(raw,1,33),2)
end

function getMessageFromRaw (raw)
	local t = split(string.sub(raw,2,-2),58)
	table.remove(t,1)
	return table.concat(t,":")
end

local chars = {"+","%","@","&","~"}
function getNickAndLevel (nick)
	for i,char in ipairs (chars) do
		if string.sub(nick,1,1) == char then
			nick = string.sub(nick,2)
			return nick,i
		end
	end
	return nick
end

function toletters (string)
	local t = {}
	for i=1,string.len(string) do
		table.insert(t,string.sub(string,1,1))
		string = string.sub(string,2)
	end
	return t
end

function getPlayerFromPartialName (name)
	local matches = {}
	for i,player in ipairs (getElementsByType("player")) do
		if string.find(string.lower(getPlayerName(player)),string.lower(name),0,false) then
			table.insert(matches,player)
		end
	end
	if #matches == 1 then
		return matches[1]
	end
	return false
end

function getResourceFromPartialName (name)
	local matches = {}
	for i,resource in ipairs (getElementsByType("resource")) do
		if string.find(string.lower(getResourceName(resource)),string.lower(name),0,false) then
			table.insert(matches,resource)
		end
	end
	if #matches == 1 then
		return matches[1]
	end
	return false
end

--[[_tostring = tostring
function tostring (string)
	local t = Split(_tostring(string),"\n")
	if #t > 1 then
		return t[2]
	end
	return _tostring(string)
end]]