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
	return nick,0
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
		if getPlayerName(player) == name then
			return player
		end
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
	for i,resource in ipairs (getResources()) do
		if getResourceName(resource) == name then
			return resource
		end
		if string.find(string.lower(getResourceName(resource)),string.lower(name),0,false) then
			table.insert(matches,resource)
		end
	end
	if #matches == 1 then
		return matches[1]
	end
	return false
end

function getTimeStamp ()
	local time = getRealTime()
	return "["..(time.year + 1900).."-"..(time.month+1).."-"..time.monthday.." "..time.hour..":"..time.minute..":"..time.second.."]"
end

local mutes = {}
function muteSerial (serial,reason,time)
	mutes[serial] = reason
	setTimer(unmuteSerial,time,1,serial)
	return true
end

function unmuteSerial (serial)
	for i,player in ipairs (getElementsByType("player")) do
		if getPlayerSerial(player) == serial then
			setPlayerMuted(player,false)
			outputChatBox("* "..getPlayerName(player).." was unmuted",root,0,0,255)
			outputIRC("12* "..getPlayerName(player).." was unmuted")
		end
	end
	mutes[serial] = nil
end

addEventHandler("onPlayerJoin",root,
	function ()
		local reason = mutes[(getPlayerSerial(source))]
		if reason then
			setPlayerMuted(source,true)
			outputChatBox("* "..getPlayerName(source).." was muted ("..reason..")",root,0,0,255)
		end
	end
)

addEventHandler("onResourceStop",resourceRoot,
	function ()
		for serial,reason in pairs (mutes) do
			unmuteSerial(serial)
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
function toMs (time)
	time = string.sub(time,2,-2)
	local t = split(time,32)
	local factor = 0
	local ms = 0
	for i,v in ipairs (t) do
		if tonumber(v) then
			factor = tonumber(v)
		elseif times[v] then
			ms = ms + (factor * times[v])
		end
	end
	return ms
end

function msToTimeStr(ms)
	if not ms then
		return ''
	end
	local centiseconds = tostring(math.floor(math.fmod(ms, 1000)/10))
	if #centiseconds == 1 then
		centiseconds = '0' .. centiseconds
	end
	local s = math.floor(ms / 1000)
	local seconds = tostring(math.fmod(s, 60))
	if #seconds == 1 then
		seconds = '0' .. seconds
	end
	local minutes = tostring(math.floor(s / 60))
	return minutes .. ':' .. seconds .. ':' .. centiseconds
end

function getTimeString (timeMs)
	local weeks = math.floor(timeMs/604800000)
	timeMs = timeMs - weeks * 604800000
	
	local days = math.floor(timeMs/86400000)
	timeMs = timeMs - days * 86400000
	
	local hours = math.floor(timeMs/3600000)
	timeMs = timeMs - hours * 3600000
	
	local minutes = math.floor(timeMs/60000)
	timeMs = timeMs - minutes * 60000
	
	local seconds = math.floor(timeMs/1000)
	
	return string.format("%dwks %ddays %dhrs %dmins %dsecs ",weeks,days,hours,minutes,seconds)
end

function getNumberFromVersion (version)
	local p1,p2,p3 = unpack(split(version,46))
	return tonumber((100*tonumber(p1))+(10*tonumber(p2))+(tonumber(p3)))
end

_addBan = addBan
function addBan (...)
	if getVersion().number < 260 then
		local t = {...}
		t[4] = nil
		return _addBan(unpack(t))
	else
		return _addBan(...)
	end
end

function getSocketErrorString (integer)
	if sockets[integer] then
		return sockets[integer]
	else
		return "Unknown error"
	end
end

-- linux error codes	
local socketErrors = {}	
socketErrors[001] = "Operation not permitted"
socketErrors[002] = "No such file or directory"
socketErrors[003] = "No such process"
socketErrors[004] = "Interrupted system call"
socketErrors[005] = "I/O error"
socketErrors[006] = "No such device or address"
socketErrors[007] = "Argument list too long"
socketErrors[008] = "Exec format error"
socketErrors[009] = "Bad file number"
socketErrors[010] = "No child processes"
socketErrors[011] = "Try again"
socketErrors[012] = "Out of memory"
socketErrors[013] = "Permission denied"
socketErrors[014] = "Bad address"
socketErrors[015] = "Block device required"
socketErrors[016] = "Device or resource busy"
socketErrors[017] = "File exists"
socketErrors[018] = "Cross-device link"
socketErrors[019] = "No such device"
socketErrors[020] = "Not a directory"
socketErrors[021] = "Is a directory"
socketErrors[022] = "Invalid argument"
socketErrors[023] = "File table overflow"
socketErrors[024] = "Too many open files"
socketErrors[025] = "Not a typewriter"
socketErrors[026] = "Text file busy"
socketErrors[027] = "File too large"
socketErrors[028] = "No space left on device"
socketErrors[029] = "Illegal seek"
socketErrors[030] = "Read-only file system"
socketErrors[031] = "Too many links"
socketErrors[032] = "Broken pipe"
socketErrors[033] = "Math argument out of domain of func"
socketErrors[034] = "Math result not representable"
socketErrors[035] = "Resource deadlock would occur"
socketErrors[036] = "File name too long"
socketErrors[037] = "No record locks available"
socketErrors[038] = "Function not implemented"
socketErrors[039] = "Directory not empty"
socketErrors[040] = "Too many symbolic links encountered"
socketErrors[042] = "No message of desired type"
socketErrors[043] = "Identifier removed"
socketErrors[044] = "Channel number out of range"
socketErrors[045] = "Level 2 not synchronized"
socketErrors[046] = "Level 3 halted"
socketErrors[047] = "Level 3 reset"
socketErrors[048] = "Link number out of range"
socketErrors[049] = "Protocol driver not attached"
socketErrors[050] = "No CSI structure available"
socketErrors[051] = "Level 2 halted"
socketErrors[052] = "Invalid exchange"
socketErrors[053] = "Invalid request descriptor"
socketErrors[054] = "Exchange full"
socketErrors[055] = "No anode"
socketErrors[056] = "Invalid request code"
socketErrors[057] = "Invalid slot"
socketErrors[059] = "Bad font file format"
socketErrors[060] = "Device not a stream"
socketErrors[061] = "No data available"
socketErrors[062] = "Timer expired"
socketErrors[063] = "Out of streams resources"
socketErrors[064] = "Machine is not on the network"
socketErrors[065] = "Package not installed"
socketErrors[066] = "Object is remote"
socketErrors[067] = "Link has been severed"
socketErrors[068] = "Advertise error"
socketErrors[069] = "Srmount error"
socketErrors[070] = "Communication error on send"
socketErrors[071] = "Protocol error"
socketErrors[072] = "Multihop attempted"
socketErrors[073] = "RFS specific error"
socketErrors[074] = "Not a data message"
socketErrors[075] = "Value too large for defined data type"
socketErrors[076] = "Name not unique on network"
socketErrors[077] = "File descriptor in bad state"
socketErrors[078] = "Remote address changed"
socketErrors[079] = "Can not access a needed shared library"
socketErrors[080] = "Accessing a corrupted shared library"
socketErrors[081] = ".lib section in a.out corrupted"
socketErrors[082] = "Attempting to link in too many shared libraries"
socketErrors[083] = "Cannot exec a shared library directly"
socketErrors[084] = "Illegal byte sequence"
socketErrors[085] = "Interrupted system call should be restarted"
socketErrors[086] = "Streams pipe error"
socketErrors[087] = "Too many users"
socketErrors[088] = "Socket operation on non-socket"
socketErrors[089] = "Destination address required"
socketErrors[090] = "Message too long"
socketErrors[091] = "Protocol wrong type for socket"
socketErrors[092] = "Protocol not available"
socketErrors[093] = "Protocol not supported"
socketErrors[094] = "Socket type not supported"
socketErrors[095] = "Operation not supported on transport endpoint"
socketErrors[096] = "Protocol family not supported"
socketErrors[097] = "Address family not supported by protocol"
socketErrors[098] = "Address already in use"
socketErrors[099] = "Cannot assign requested address"
socketErrors[100] = "Network is down"
socketErrors[101] = "Network is unreachable"
socketErrors[102] = "Network dropped connection because of reset"
socketErrors[103] = "Software caused connection abort"
socketErrors[104] = "Connection reset by peer"
socketErrors[105] = "No buffer space available"
socketErrors[106] = "Transport endpoint is already connected"
socketErrors[107] = "Transport endpoint is not connected"
socketErrors[108] = "Cannot send after transport endpoint shutdown"
socketErrors[109] = "Too many references: cannot splice"
socketErrors[110] = "Connection timed out"
socketErrors[111] = "Connection refused"
socketErrors[112] = "Host is down"
socketErrors[113] = "No route to host"
socketErrors[114] = "Operation already in progress"
socketErrors[115] = "Operation now in progress"
socketErrors[116] = "Stale NFS file handle"
socketErrors[117] = "Structure needs cleaning"
socketErrors[118] = "Not a XENIX named type file"
socketErrors[119] = "No XENIX semaphores available"
socketErrors[120] = "Is a named type file"
socketErrors[121] = "Remote I/O error"
socketErrors[122] = "Quota exceeded"
socketErrors[123] = "No medium found"
socketErrors[124] = "Wrong medium type"