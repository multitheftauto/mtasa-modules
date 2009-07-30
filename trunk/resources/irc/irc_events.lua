local root=getRootElement()

addEvent("onIRCJoin")
addEvent("onIRCQuit")
addEvent("onIRCMessage")
addEvent("onIRCAction")

function handleRawIRCData(msg)
	outputDebugString(msg)
	
	local msgTable=split(msg,32)
	
	local userAdress=gettok(msgTable[1],1,33)
	local user   = string.sub(userAdress,2,#userAdress)
	local action = msgTable[2]
	
	if action~="QUIT" and action~="JOIN" then
		local channel= msgTable[3]
		if action=="PRIVMSG" then
			table.remove(msgTable,1)
			table.remove(msgTable,1)
			table.remove(msgTable,1)
			
			local outputMsg=table.concat(msgTable," ")
			outputMsg=string.sub(outputMsg,2,#outputMsg)
			if string.find(outputMsg,"ACTION")==1 then
				local actionString=string.sub(outputMsg,9,#outputMsg)
				triggerEvent("onIRCAction",root,channel,user,actionString)
			else
				triggerEvent("onIRCMessage",root,channel,user,outputMsg)
			end
		end
	elseif action=="QUIT" then
		table.remove(msgTable,1)
		table.remove(msgTable,1)
		
		local reason=table.concat(msgTable," ")
		      reason=string.sub(reason,2,#reason)
		triggerEvent("onIRCQuit",root,user,reason)
	elseif action=="JOIN" then
		local channel=msgTable[3]
		      channel=string.sub(channel,2,#channel)
		triggerEvent("onIRCJoin",root,user,channel)
	end
end
addEventHandler("onIRCRaw",root,handleRawIRCData)