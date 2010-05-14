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

class CFunctions;

#ifndef __CFUNCTIONS_H
#define __CFUNCTIONS_H

#include "ml_base.h"
#include "Socket.h"
#include <stdio.h>

#include "include/ILuaModuleManager.h"
extern ILuaModuleManager10 *pModuleManager;

class Socket;

class CFunctions
{
public:
        static int sockOpen            (lua_State* luaVM);
        static int sockWrite           (lua_State* luaVM);
        static int sockClose           (lua_State* luaVM);
        
        static int getSocketByUserdata (Socket*& socket, const void* userdata);

        static void deleteAllSockets   ();
//      static void debugPrint         (char* text);
        static void Cooldown           (int ms);
        static void saveLuaData        (lua_State* luaVM);
        static void addEvent           (lua_State* luaVM, const char* szEventName);
        static void triggerEvent       (const string& eventName, void* userdata, const string& arg1);
        static void closeSocket        (void* userdata);

#ifdef WIN32
        static void doPulse            (void* args);
#else
        static void* doPulse           (void* args);
#endif
};
#endif
