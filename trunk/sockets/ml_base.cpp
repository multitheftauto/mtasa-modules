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

	WSADATA WSA;
	if(WSAStartup(MAKEWORD(2, 0), &WSA) != 0) // Startup (Needed for Win32)
    {
        return false;
    }

	return true;
}


MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
	if ( pModuleManager && luaVM )
	{
		// Functions
		pModuleManager->RegisterFunction(luaVM, "socketConnect", CFunctions::socketConnect);
//		pModuleManager->RegisterFunction(luaVM, "socketDisconnect", CFunctions::socketDisconnect);
//		pModuleManager->RegisterFunction(luaVM, "socketSendData", CFunctions::socketSendData);

		// Events
		CFunctions::addEvent(luaVM, "onSocketConnected");
		CFunctions::addEvent(luaVM, "onSocketDisconnected");
		CFunctions::addEvent(luaVM, "onSocketDataReceive");
	}
}


MTAEXPORT bool DoPulse ( void )
{
	return true;
}

MTAEXPORT bool ShutdownModule ( void )
{
	WSACleanup();
	return true;
}
