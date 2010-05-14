/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright © 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is © 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

#include "CFunctions.h"
#include "extra/CLuaArguments.h"

lua_State* gLuaVM;
vector<Socket*> sockets;

int CFunctions::sockOpen(lua_State *luaVM)
{
    if (luaVM)
    {
        if (lua_type(luaVM, 1) == LUA_TSTRING && lua_type(luaVM, 2) == LUA_TNUMBER)
        {
            const char* host    = lua_tostring(luaVM, 1);
            unsigned short port = static_cast<unsigned short>(atoi(lua_tostring(luaVM, 2)));

            Socket* socket = new Socket(luaVM, host, port);

            if (!socket->isConnecting() && !socket->isConnected())
            {
                lua_pushboolean(luaVM, false);
                return 1;
            }

            void* userdata = socket->getUserdata();
            
            sockets.push_back(socket);

            lua_pushlightuserdata(luaVM,userdata);
            return 1;
        }
    }

    lua_pushboolean(luaVM, false);
    return 1;
}

int CFunctions::getSocketByUserdata(Socket*& socket, const void* userdata)
{
    for (unsigned int i = 0; i < sockets.size(); ++i)
    {
        Socket* sock = sockets[i];

        if (sock->getUserdata() == userdata)
        {
            socket = sock;
            return i;
        }
    }

    return -1;
}

int CFunctions::sockWrite(lua_State *luaVM)
{
    if (luaVM)
    {
        if (lua_type(luaVM, 1) == LUA_TLIGHTUSERDATA && lua_type(luaVM, 2) == LUA_TSTRING)
        {
            void* userdata   = lua_touserdata(luaVM, 1);

            Socket* theSocket = NULL;

            getSocketByUserdata(theSocket, userdata);

            if (theSocket != NULL)
            {
                lua_pushboolean(luaVM, theSocket->sendData(lua_tostring(luaVM, 2)));
                return 1;
            }
        }
    }

    lua_pushboolean(luaVM, false);
    return 1;
}

int CFunctions::sockClose(lua_State *luaVM)
{
    if (luaVM)
    {
        if (lua_type(luaVM, 1) == LUA_TLIGHTUSERDATA)
        {
            void*   userdata  = lua_touserdata(luaVM, 1);
            Socket* theSocket = NULL;

            if (getSocketByUserdata(theSocket, userdata) != -1)
            {
                closeSocket(userdata);

                lua_pushboolean(luaVM, true);
                return 1;
            }
        }
    }

    lua_pushboolean(luaVM, false);
    return 1;
}

void CFunctions::closeSocket(void* userdata)
{
    Socket* theSocket = NULL;
    
    int index = getSocketByUserdata(theSocket, userdata);

    if (theSocket != NULL)
    {
        sockets.erase(sockets.begin() + index);
        delete theSocket;
    }
}

void CFunctions::deleteAllSockets()
{
    for (unsigned int i = 0; i < sockets.size(); ++i)
        SAFE_DELETE(sockets[i]);
}

#ifdef WIN32
void CFunctions::doPulse(void* args)
#else
void* CFunctions::doPulse(void* args)
#endif
{
    while (true)
    {
        Socket* socket = (Socket*)args;

        if (socket->isConnected() || socket->isConnecting())
        {        
            socket->doPulse();
            Cooldown(50);
        }
        else
        {
            closeSocket(socket->getUserdata());
            Cooldown(1000);
        }
    }
}

void CFunctions::Cooldown(int ms)
{
#ifdef WIN32
    Sleep(ms);
#else
    usleep(ms);
#endif
}

void CFunctions::saveLuaData(lua_State *luaVM)
{
    gLuaVM = luaVM;
}

void CFunctions::addEvent(lua_State* luaVM, const char* szEventName)
{
    CLuaArguments args;
    args.PushString(szEventName);
    args.PushBoolean(true);
    args.Call(luaVM, "addEvent");
}

void CFunctions::triggerEvent(const string& eventName, void* userdata, const string& arg1)
{
    CLuaArguments args;
    args.PushString(eventName.c_str());

    lua_getglobal(gLuaVM, "root");
    CLuaArgument RootElement(gLuaVM, -1);

    args.PushUserData(RootElement.GetLightUserData()); // source
    args.PushUserData(userdata);

    if (arg1.length() > 0)
        args.PushString(arg1.c_str());

    args.Call(gLuaVM, "triggerEvent");
}

/*void CFunctions::debugPrint(char* text)
{
#ifdef _DEBUG
    printf(text);
#endif
}*/
