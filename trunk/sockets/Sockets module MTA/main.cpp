/*#include "sockets.h"

MTAEXPORT bool InitModule(ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion)
{
	pModuleManager = pManager;
	strncpy(szModuleName, MODULE_NAME, MAX_INFO_LENGTH);
	strncpy(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH);
	(*fVersion) = MODULE_VERSION;

	WSADATA WSA;
	if(WSAStartup(MAKEWORD(2, 0), &WSA) != 0) // Startup (Needed for Win32)
    {
        return false;
    }

	return true
}

MTAEXPORT void RegisterFunctions(lua_State *luaVM)
{
	if(pModuleManager && luaVM)
	{
		// Save luaVM and pModuleManager in CFunctions.cpp
		CFunctions::SaveLuaData(pModuleManager, luaVM);

		// Functions
		pModuleManager->RegisterFunction(luaVM, "socketConnect", CFunctions::socketConnect);
		pModuleManager->RegisterFunction(luaVM, "socketDisconnect", CFunctions::socketDisconnect);
		pModuleManager->RegisterFunction(luaVM, "socketSendData", CFunctions::socketSendData);

		// Events
		CFunctions::AddEvent(luaVM, "onSocketConnected");
		CFunctions::AddEvent(luaVM, "onSocketDisconnected");
		CFunctions::AddEvent(luaVM, "onSocketDataReceive");
	}
}


MTAEXPORT bool DoPulse(void)
{
	return true;
}

MTAEXPORT bool ShutdownModule(void)
{
	WSACleanup();
	return true;
}*/