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

#include "ml_base.h"

ILuaModuleManager10 *pModuleManager = NULL;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule ( ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion )
{
    pModuleManager = pManager;

    // Set the module info
    strncpy ( szModuleName, MODULE_NAME, MAX_INFO_LENGTH );
    strncpy ( szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH );
    (*fVersion) = MODULE_VERSION;

#ifdef WIN32
    // Winsock startup (Needed for Win32)
    WSADATA WSA;
    if(WSAStartup(MAKEWORD(2, 0), &WSA) != 0) 
    {
        pModuleManager->ErrorPrintf("[Sockets module] Can't start Winsock, aborting...");
        return false;
    }
#endif

    return true;
}


MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
    if ( pModuleManager && luaVM )
    {
        // Save luaVM in CFunctions.cpp
        CFunctions::saveLuaData(luaVM);

        // Create functions
        pModuleManager->RegisterFunction(luaVM, "sockOpen", CFunctions::sockOpen);
        pModuleManager->RegisterFunction(luaVM, "sockWrite", CFunctions::sockWrite);
        pModuleManager->RegisterFunction(luaVM, "sockClose", CFunctions::sockClose);

        // Define events
        CFunctions::addEvent(luaVM, "onSockOpened");
        CFunctions::addEvent(luaVM, "onSockData");
        CFunctions::addEvent(luaVM, "onSockClosed");
    }
}

MTAEXPORT bool DoPulse ( void )
{
    // CFunction has access to the sockets vector, that's why this spaghetti is here
    CFunctions::doPulse();

    return true;
}

MTAEXPORT bool ShutdownModule ( void )
{
    // CFunctions has access to the sockets vector, that's why this spaghetti is here
    CFunctions::deleteAllSockets();

#ifdef WIN32
    // Cleaning up all Winsock stuff
    WSACleanup();
#endif

    return true;
}
