---------------------------------------------------------------------
-- Project: irc
-- Author: MCvarial
-- Contact: mcvarial@gmail.com
-- Version: 1.0.2
-- Date: 31.10.2010
---------------------------------------------------------------------

local x,y = guiGetScreenSize()
local g_channels = {}
local window

------------------------------------
-- Irc client
------------------------------------
addEvent("ircStartClient",true)
addEventHandler("ircStartClient",root,
	function (channels)
		window = guiCreateWindow((x/2)-320,(y/2)-240,640,480,"Internet Relay Chat",false)
		local tabpanel = guiCreateTabPanel(0.01,0.05,0.98,0.95,true,window)
		for i,channel in ipairs (channels) do
			local tab = guiCreateTab(channel.name,tabpanel)
			local memo = guiCreateMemo(0.01,0.01,0.75,0.90,"* Topic is: "..channel.topic.."\n",true,tab)
			local edit = guiCreateEdit(0.01,0.92,0.75,0.07,"",true,tab)
			local gridlist = guiCreateGridList(0.76,0.01,0.23,0.90,true,tab)
			local hidebtn = guiCreateButton(0.76,0.92,0.11,0.07,"Hide",true,tab)
			local closebtn = guiCreateButton(0.88,0.92,0.11,0.07,"Close",true,tab)
			guiGridListAddColumn(gridlist,"users",0.88)
			guiMemoSetReadOnly(memo,true)
			for i,user in ipairs (channel.users) do
				local row = guiGridListAddRow(gridlist)
				guiGridListSetItemText(gridlist,row,1,getIconFromLevel(user.level)..user.name,false,false)
				guiGridListSetItemData(gridlist,row,1,user.name)
			end
			guiGridListSetSortingEnabled(gridlist,true)
			addEventHandler("onClientGUIClick",closebtn,ircStopClient,false)
			addEventHandler("onClientGUIClick",hidebtn,ircHideClient,false)
			addEventHandler("onClientGUIAccepted",edit,
				function (edit)
					local text = guiGetText(edit)
					if text and text ~= "" then
						triggerServerEvent("ircSendMessage",resourceRoot,localPlayer,channel.name,text)
						guiSetText(edit,"")
					end
				end
			)
			g_channels[channel.name] = {memo=memo,edit=edit,gridlist=gridlist}
		end
		guiSetInputEnabled(true)
	end
)

addEventHandler("onClientResourceStop",resourceRoot,
	function ()
		if window then
			showCursor(false)
			guiSetInputEnabled(false)
		end
	end
)

function ircStopClient ()
	destroyElement(window)
	window = nil
	channels = {}
	showCursor(false)
	guiSetInputEnabled(false)
	triggerServerEvent("ircStopClient",resourceRoot,localPlayer)
end

function ircHideClient ()
	guiSetVisible(window,false)
	showCursor(false)
	guiSetInputEnabled(false)
end

addCommandHandler("irc",
	function ()
		if window then
			guiSetVisible(window,true)
			showCursor(true)
			guiSetInputEnabled(true)
		end
	end,false
)

local icons = {"+","%","@","&","~"}
function getIconFromLevel (level)
	return icons[level] or ""
end

function getLevelFromIcon (ico)
	for i,icon in ipairs (icons) do
		if icon == ico then
			return i
		end
	end
	return false
end

function addMessage (channel,message)
	guiSetText(g_channels[channel].memo,guiGetText(g_channels[channel].memo)..""..message)
end

addEvent("onIRCMessage",true)
addEventHandler("onIRCMessage",root,
	function (user,channel,message)
		if g_channels[channel] then
			addMessage(channel,"<"..user.."> "..message)
		end
	end
)

addEvent("onIRCUserJoin",true)
addEventHandler("onIRCUserJoin",root,
	function (user,channel,vhost)
		if g_channels[channel] then
			addMessage(channel,"* "..user.." joined ("..vhost..")")
			local row = guiGridListAddRow(g_channels[channel].gridlist)
			guiGridListSetItemText(g_channels[channel].gridlist,row,1,user,false,false)
			guiGridListSetItemData(g_channels[channel].gridlist,row,1,user)
		end
	end
)

addEvent("onIRCUserPart",true)
addEventHandler("onIRCUserPart",root,
	function (user,channel,reason)
		if g_channels[channel] then
			addMessage(channel,"* "..user.." parted ("..reason..")")
			for i=0,guiGridListGetRowCount(g_channels[channel].gridlist)-1 do
				if guiGridListGetItemData(g_channels[channel].gridlist,i,1) == user then
					guiGridListRemoveRow(g_channels[channel].gridlist,i)
				end
			end
		end
	end
)

addEvent("onIRCUserQuit",true)
addEventHandler("onIRCUserQuit",root,
	function (user,reason)
		for channel,info in pairs (g_channels) do
			for i=0,guiGridListGetRowCount(info.gridlist)-1 do
				if guiGridListGetItemData(info.gridlist,i,1) == user then
					guiGridListRemoveRow(info.gridlist,i)
					addMessage(channel,"* "..user.." quit ("..reason..")")
				end
			end
		end
	end
)

addEvent("onIRCLevelChange",true)
addEventHandler("onIRCLevelChange",root,
	function (user,channel,oldlevel,newlevel)
		if g_channels[channel] then
			addMessage(channel,"* "..user.." changed level from "..tostring(oldlevel).." to "..tostring(newlevel))
			for i=0,guiGridListGetRowCount(g_channels[channel].gridlist)-1 do
				if guiGridListGetItemData(g_channels[channel].gridlist,i,1) == user then
					guiGridListSetItemText(g_channels[channel].gridlist,i,1,getIconFromLevel(newlevel)..user,false,false)
				end
			end
		end
	end
)

addEvent("onIRCUserChangeNick",true)
addEventHandler("onIRCUserChangeNick",root,
	function (user,oldnick,newnick)
		for channel,info in pairs (g_channels) do
			for i=0,guiGridListGetRowCount(info.gridlist)-1 do
				if guiGridListGetItemData(info.gridlist,i,1) == oldnick then
					addMessage(channel,"* "..oldnick.." is now know as "..newnick)
					local oldIcon = string.sub(guiGridListGetItemText(info.gridlist,i,1),1,1)
					if not getLevelFromIcon(oldIcon) then oldIcon = "" end
					guiGridListSetItemText(info.gridlist,i,1,oldIcon..newnick,false,false)
					guiGridListSetItemData(info.gridlist,i,1,newnick)
				end
			end
		end
	end
)