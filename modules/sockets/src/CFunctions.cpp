/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright (c) 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is (c) 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

#include "include/CFunctions.h"

int CFunctions::sockOpen(lua_State* luaVM)
{
    if ( luaVM )
    {
        // Make sure host is a string, and port is a number
        if (lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TNUMBER)
        {

            if ( CSocket::GetTotalOpenSocketCount () >= 255 )
            {
                pModuleManager->ErrorPrintf("[Sockets] Can't sockOpen as 255 sockets already open");
                lua_pushboolean(luaVM, false);
                return 1;
            }

            // Put the host in a string, and the port in an unsigned short
            string strHost        = lua_tostring(luaVM, 1);
            unsigned short usPort = static_cast < unsigned short > ( lua_tonumber ( luaVM, 2 ) );

            // Create the socket
            CSocket* pSocket = new CSocket(luaVM, strHost, usPort);
            void* pUserdata  = pSocket->GetUserdata();

            // The socket has got a userdata value if successfully created. It doesn't otherwise
            if (pUserdata == NULL)
            {
                SAFE_DELETE(pSocket);
                lua_pushboolean(luaVM, false);
                return 1;
            }

            // Add the socket to the Pulse list
            CSocketManager::SocketAdd(pSocket);

            // Return the userdata
            lua_pushlightuserdata(luaVM, pUserdata);
            return 1;
        }
    }

    lua_pushboolean(luaVM, false);
    return 1;
}

int CFunctions::sockWrite(lua_State *luaVM)
{
    if ( luaVM )
    {
        // Make sure the socket is an userdata value, and data is a string
        if (lua_type(luaVM, 1) == LUA_TLIGHTUSERDATA && lua_type(luaVM, 2) == LUA_TSTRING)
        {
            // Prepare variables
            void* pUserdata  = lua_touserdata(luaVM, 1);
            CSocket* pSocket = NULL;

            // Get the socket from the userdata. Gets stored in pSocket. If failed, returns false
            if (CSocketManager::GetSocket(pSocket, pUserdata))
            {
                // Try to send the data
                string strData = lua_tostring(luaVM, 2);
                bool bSuccess  = pSocket->Send(strData);

                // Return whether or not it was successful
                lua_pushboolean(luaVM, bSuccess);
                return 1;
            }
        }
    }

    lua_pushboolean(luaVM, false);
    return 1;
}

int CFunctions::sockClose(lua_State *luaVM)
{
    if ( luaVM )
    {
        // Make sure the socket is an userdata value
        if (lua_type(luaVM, 1) == LUA_TLIGHTUSERDATA)
        {
            // Prepare vars
            void* pUserdata  = lua_touserdata(luaVM, 1);
            CSocket* pSocket = NULL;

            // Get the socket
            if (CSocketManager::GetSocket(pSocket, pUserdata))
            {
                // Remove it
                CSocketManager::SocketRemove(pSocket);

                lua_pushboolean(luaVM, true);
                return 1;
            }
        }
    }

    lua_pushboolean(luaVM, false);
    return 1;
}

void CFunctions::AddEvent(lua_State *luaVM, const string& strEventName)
{
    CLuaArguments args;
    args.PushString(strEventName.c_str());
    args.PushBoolean(true);
    args.Call(luaVM, "addEvent");
}
