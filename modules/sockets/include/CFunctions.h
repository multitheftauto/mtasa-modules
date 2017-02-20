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

class CFunctions;

#pragma once

#include <stdio.h>

#include "ml_sockets.h"
#include "ILuaModuleManager.h"
extern ILuaModuleManager10 *pModuleManager;

class CFunctions
{
public:
    static int sockOpen  (lua_State* luaVM);
    static int sockWrite (lua_State* luaVM);
    static int sockClose (lua_State* luaVM);

    static void AddEvent (lua_State* luaVM, const string& strEventName);
};
