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

#include "ml_sockets.h"

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

#ifndef _DEBUG
    #undef assert
    #define assert(x) __noop(x)
#endif

    WSADATA wsaData;

    if ( WSAStartup ( MAKEWORD ( 2, 2 ), &wsaData ) != 0 )
    {
        assert ( false );
        pModuleManager->ErrorPrintf("[Sockets] Can't start Winsock, aborting...");
        return false;
    }
#endif

	return true;
}


MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
	if ( pModuleManager && luaVM )
	{
        // Register functions
        pModuleManager->RegisterFunction ( luaVM, "sockOpen",  CFunctions::sockOpen );
        pModuleManager->RegisterFunction ( luaVM, "sockWrite", CFunctions::sockWrite );
        pModuleManager->RegisterFunction ( luaVM, "sockIsConnected", CFunctions::sockIsConnected );
        pModuleManager->RegisterFunction ( luaVM, "sockClose", CFunctions::sockClose );

        // Add events
        CFunctions::AddEvent ( luaVM, "onSockOpened" );
        CFunctions::AddEvent ( luaVM, "onSockData" );
        CFunctions::AddEvent ( luaVM, "onSockClosed" );
	}
}


MTAEXPORT bool DoPulse ( void )
{
    CSocketManager::DoPulse ( );
	return true;
}


MTAEXPORT bool ShutdownModule ( void )
{
    // Stop all sockets before shutting down the module
    CSocketManager::HandleStop ( );

#ifdef WIN32
    // Cleanup Winsock stuff
    WSACleanup ( );
#endif

	return true;
}
