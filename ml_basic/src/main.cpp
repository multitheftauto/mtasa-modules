#include <main.hpp>
#include <string>

MTAEXPORT bool InitModule(
	ILuaModuleManager10* pManager,
	char* szModuleName,
	char* szAuthor,
	float* fVersion
) {
	pModuleManager = pManager;

	Helper::strncpy_s(szModuleName, MODULE_NAME, MAX_INFO_LENGTH); // Set module name
	Helper::strncpy_s(szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH); // Set module author
	*fVersion = MODULE_VERSION; // Set module version

	pModuleManager->Printf("Module \"%s\" has been started!\n", MODULE_NAME);

	pModuleManager->Printf("pModuleManager->Printf\n");
	pModuleManager->ErrorPrintf("pModuleManager->ErrorPrintf\n");

	return true;
}

MTAEXPORT bool DoPulse() { return true; }
MTAEXPORT bool ShutdownModule() {
	pModuleManager->Printf("Module \"%s\" is shutting down!\n", MODULE_NAME);
	return true;
}

MTAEXPORT void RegisterFunctions(lua_State* luaVM) {
	pModuleManager->DebugPrintf(luaVM, "pModuleManager->DebugPrintf\n");
}
MTAEXPORT void ResourceStopping(lua_State* luaVM) {}
MTAEXPORT void ResourceStopped(lua_State* luaVM) {}