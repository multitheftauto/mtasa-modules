local SCREEN_WIDTH, SCREEN_HEIGHT = guiGetScreenSize()

local connectionWindow = {}
local IRCWindow        = {}
local IRCConnections   = {}

addEvent("onIRCConnect",true)
addEvent("onIRCDisconnect",true)
addEvent("onIRCMessage",true)
addEvent("onIRCAction",true)
addEvent("onIRCJoin",true)
addEvent("onIRCPart",true)
addEvent("onIRCQuit",true)
addEvent("onIRCMode",true)
addEvent("onIRCNick",true)
addEvent("onIRCNotice",true)
addEvent("onIRCTopicReceive",true)
addEvent("onIRCTopicDataReceive",true)
addEvent("onIRCUserListReceive",true)

function handleResourceStart()
	createIRCWindow()
	createConnectionWindow()
end
addEventHandler("onClientResourceStart",getResourceRootElement(),handleResourceStart)

function handleResourceStop()
	showCursor(false)
	guiSetInputEnabled(false)
end
addEventHandler("onClientResourceStop",getResourceRootElement(),handleResourceStop)

function createIRCWindow()
	IRCWindow.window = guiCreateWindow(SCREEN_WIDTH / 2 - 400,SCREEN_HEIGHT / 2 - 300,800,600,"IRC",false)
	IRCWindow.hosttabs = guiCreateTabPanel(10,20,780,530,false,IRCWindow.window)
	IRCWindow.hide = guiCreateButton(680,565,110,25,"Hide IRC",false,IRCWindow.window)
	IRCWindow.connect = guiCreateButton(10,565,110,25,"Connect...",false,IRCWindow.window)
	
	addEventHandler("onClientGUIClick",IRCWindow.hide,toggleIRCWindow,false)
	addEventHandler("onClientGUIClick",IRCWindow.connect,toggleConnectionWindow,false)
	
	guiSetVisible(IRCWindow.window,false)
end

function createConnectionWindow()
	connectionWindow.window = guiCreateWindow(SCREEN_WIDTH / 2 - 117.5,SCREEN_HEIGHT / 2 - 65,235,130,"IRC Connection",false)
	connectionWindow.connect = guiCreateButton(12,100,80,20,"Connect",false,connectionWindow.window)
	connectionWindow.cancel = guiCreateButton(145,100,80,20,"Cancel",false,connectionWindow.window)
	connectionWindow.host = guiCreateEdit(45,25,180,20,"irc.gtanet.com",false,connectionWindow.window)
	connectionWindow.port = guiCreateEdit(45,45,180,20,"6667",false,connectionWindow.window)
	connectionWindow.nick = guiCreateEdit(45,65,180,20,"",false,connectionWindow.window)
	guiCreateLabel(10,27,35,16,"Host:",false,connectionWindow.window)
	guiCreateLabel(10,47,35,16,"Port:",false,connectionWindow.window)
	guiCreateLabel(10,67,35,16,"Nick:",false,connectionWindow.window)
	
	guiWindowSetSizable(connectionWindow.window,false)
	
	guiSetVisible(connectionWindow.window,false)
	
	addEventHandler("onClientGUIClick",connectionWindow.connect,handleConnectPressed,false)
	addEventHandler("onClientGUIClick",connectionWindow.cancel,toggleConnectionWindow,false)
end

function createHostTab(hostName)
	local tab         = guiCreateTab(hostName,IRCWindow.hosttabs)
	local channelTabs = guiCreateTabPanel(0,0,780,505,false,tab)
	
	local channelManagerTab = guiCreateTab("Manage channels...",channelTabs)
	
	local joinButton  = guiCreateButton(14,79,80,28,"Join",false,channelManagerTab)
	local channelEdit = guiCreateEdit(62,36,135,25,"",false,channelManagerTab)
	local joinLabel   = guiCreateLabel(23,4,175,25,"Join",false,channelManagerTab)
		guiSetFont(joinLabel,"default-bold-small")
	guiCreateLabel(7,39,75,18,"Channel:",false,channelManagerTab)
	
	addEventHandler("onClientGUIClick",joinButton,requestJoinChannel,false)
	
	return {tab=tab, channelTabs=channelTabs, channelManagerTab=channelManagerTab, joinButton=joinButton, channelEdit=channelEdit}
end

function requestJoinChannel()
	for index,connection in ipairs(IRCConnections) do
		if source == connection.gui.joinButton then
			local host    = connection.host
			local channel = guiGetText(connection.gui.channelEdit)
			
			triggerServerEvent("onIRCJoinRequest",getRootElement(),host,channel)
			
			return
		end
	end
end

function sendMessage()
	for index,connection in ipairs(IRCConnections) do
		for channel,data in pairs(connection.channels) do
			if source == data.typeField or source == data.sayButton then
				local msg = guiGetText(data.typeField)
				addMemoMessage(data.textField,connection.nick..": "..msg)
				triggerServerEvent("onIRCSayRequest",getRootElement(),connection.host,channel,msg)
				guiSetText(data.typeField,"")
				return
			end
		end
	end
end

function createChannelTab(channelTabs,channelName)
	local tab      = guiCreateTab(channelName,channelTabs)
	local userGrid = guiCreateGridList(0,0,115,480,false,tab)
		guiGridListAddColumn(userGrid,"User",0.8)
	local textField = guiCreateMemo(120,0,655,430,"",false,tab)
		guiMemoSetReadOnly(textField,true)
	local typeField = guiCreateEdit(120,450,590,30,"",false,tab)
	local sayButton = guiCreateButton(711,449,68,30,"Say",false,tab)
	
	guiSetSelectedTab(channelTabs,tab)
	
	addEventHandler("onClientGUIClick",sayButton,sendMessage,false)
	addEventHandler("onClientGUIAccepted",typeField,sendMessage,false)
	
	return {tab=tab, userGrid=userGrid, textField=textField, typeField=typeField, sayButton=sayButton}
end

function toggleIRCWindow()
	local toggle = not guiGetVisible(IRCWindow.window)
	
	guiSetVisible(IRCWindow.window,toggle)
	showCursor(toggle)
	guiSetInputEnabled(toggle)
	
	if (not toggle) and guiGetVisible(connectionWindow.window) then
		guiSetVisible(connectionWindow.window,false)
	end
end
addCommandHandler("irc",toggleIRCWindow)

function toggleConnectionWindow()
	local toggle = not guiGetVisible(connectionWindow.window)
	
	guiSetVisible(connectionWindow.window,toggle)
	
	guiBringToFront(connectionWindow.window)
end

function handleConnectPressed()
	guiSetVisible(connectionWindow.window,false)
	
	local host = guiGetText(connectionWindow.host)
	local port = tonumber(guiGetText(connectionWindow.port))
	local nick = guiGetText(connectionWindow.nick)
	
	triggerServerEvent("onIRCConnectionRequest",getRootElement(),host,port,nick)
	
	guiSetText(connectionWindow.host,"")
	guiSetText(connectionWindow.port,"")
	guiSetText(connectionWindow.nick,"")
end

function ircGetUserNick(user)
	if user and type(user) == "string" and string.find(user,"!") then
		return gettok(user,1,string.byte("!"))
	end
	
	return false
end

local rank = {["~"]=5000,["&"]=4000,["@"]=3000,["%"]=2000,["+"]=1000}

function gridListArrangementFunc(a,b)
	a = string.lower(a)
	b = string.lower(b)
	
	local aRank = rank[string.sub(a,1,1)] or 0
	local bRank = rank[string.sub(b,1,1)] or 0
	
	if aRank > bRank then
		return true
	elseif bRank > aRank then
		return false
	else
		return a < b
	end
end

function rearrangeGridList(grid)
	local users = {}
	
	for i=1,guiGridListGetRowCount(grid) do
		table.insert(users,guiGridListGetItemText(grid,0,1))
		guiGridListRemoveRow(grid,0)
	end
	
	table.sort(users,gridListArrangementFunc)
	
	for index,user in ipairs(users) do
		local row = guiGridListAddRow(grid)
		guiGridListSetItemText(grid,row,1,user,false,false)
	end
end

function getIRCConnectionByHost(host)
	for index,connection in ipairs(IRCConnections) do
		if connection.host == host then
			return connection
		end
	end
end

function gridlistAddUser(grid,user)
	for row=1,guiGridListGetRowCount(grid) do
		if guiGridListGetItemText(grid,row-1,1) == user then
			return
		end
	end
	
	local row = guiGridListAddRow(grid)
	guiGridListSetItemText(grid,row,1,user,false,false)
	
	rearrangeGridList(grid)
end

function gridlistRemoveUser(grid,user)
	for row=1,guiGridListGetRowCount(grid) do
		if guiGridListGetItemText(grid,row-1,1) == user then
			guiGridListRemoveRow(grid,row-1)
			return true
		end
	end
	return false
end

function getCurrentTimeStamp()
	local time = getRealTime()
	
	return "["..string.format("%02i",time.hour)..":"..string.format("%02i",time.minute).."]"
end

function addMemoMessage(memo,msg)
	guiSetText(memo,getCurrentTimeStamp().." "..msg.."\n"..guiGetText(memo))
end

addEventHandler("onIRCConnect",getRootElement(),
	function(host,nick)
		table.insert(IRCConnections,{host=host,nick=nick,gui=createHostTab(host),channels={}})
	end
)

addEventHandler("onIRCJoin",getRootElement(),
	function(host,user,channel)
		local IRCConnection = getIRCConnectionByHost(host)
		
		user = ircGetUserNick(user)
		
		if user == IRCConnection.nick then
			IRCConnection.channels[channel] = createChannelTab(IRCConnection.gui.channelTabs,channel)
		else
			addMemoMessage(IRCConnection.channels[channel].textField,user.." joined the channel")
			gridlistAddUser(IRCConnection.channels[channel].userGrid,user)
		end
	end
)

addEventHandler("onIRCUserListReceive",getRootElement(),
	function(host,channel,users)
		local IRCConnection = getIRCConnectionByHost(host)
		
		for index,user in ipairs(users) do
			gridlistAddUser(IRCConnection.channels[channel].userGrid,user)
		end
	end
)

addEventHandler("onIRCMessage",getRootElement(),
	function(host,user,channel,msg)
		local IRCConnection = getIRCConnectionByHost(host)
		
		user = ircGetUserNick(user)
		
		addMemoMessage(IRCConnection.channels[channel].textField, user..": "..msg)
	end
)

addEventHandler("onIRCAction",getRootElement(),
	function(host,user,channel,action)
		local IRCConnection = getIRCConnectionByHost(host)
		
		user = ircGetUserNick(user)
		
		addMemoMessage(IRCConnection.channels[channel].textField, "*"..user.." "..action)
	end
)

addEventHandler("onIRCPart",getRootElement(),
	function(host,user,channel)
		local IRCConnection = getIRCConnectionByHost(host)
		
		user = ircGetUserNick(user)
		
		addMemoMessage(IRCConnection.channels[channel].textField, user.." left the channel")
		gridlistRemoveUser(IRCConnection.channels[channel].userGrid,user)
	end
)

addEventHandler("onIRCQuit",getRootElement(),
	function(host,user,reason)
		local IRCConnection = getIRCConnectionByHost(host)
		
		user = ircGetUserNick(user)
		
		for channel,data in pairs(IRCConnection.channels) do
			if gridlistRemoveUser(data.userGrid,user) then
				addMemoMessage(data.textField, user.." left "..IRCConnection.host.." ("..reason..")")
			end
		end
	end
)

addEventHandler("onIRCNick",getRootElement(),
	function(host,user,newNick)
		local IRCConnection = getIRCConnectionByHost(host)
		
		user = ircGetUserNick(user)
		
		for channel,data in pairs(IRCConnection.channels) do
			if gridlistRemoveUser(data.userGrid,user) then
				addMemoMessage(data.textField, user.." changed his nick to "..newNick)
				gridlistAddUser(data.userGrid,user)
			end
		end
	end
)