#pragma once

#include <mta/ILuaModuleManager.hpp>
#include <mta/CLuaArguments.hpp>
#include <mta/init.hpp>

#ifdef _WIN32
#	define MTAEXPORT extern "C" __declspec(dllexport)
#else
#	define MTAEXPORT extern "C"
#endif
#define MAX_RESOURCE_LENGTH 256

// Uncomment line below if you dont want to mess with Linker settings
// Alternatively define _AUTO_LINK in Preprocessor Definitions
//#define _AUTO_LINK

#if defined(_M_X64) || defined(__x86_64__) || defined(_M_AMD64)
#	define ANY_x64
#	ifdef _WIN64
#		define WIN_x64
#		ifdef _AUTO_LINK
#			pragma comment(lib, "lua5.1_64.lib")
#		endif
#	else
#		define LINUX_x64
#	endif
#else
#	define ANY_x86
#	ifdef _WIN32
#		define WIN_x86
#		ifdef _AUTO_LINK
#			pragma comment(lib, "lua5.1.lib")
#		endif
#	else
#		define LINUX_x86
#	endif
#endif

extern inline ILuaModuleManager10* pModuleManager = nullptr;

static std::string GetResourceName(lua_State* luaVM) noexcept {
	char resourceName[MAX_RESOURCE_LENGTH];
	pModuleManager->GetResourceName(luaVM, resourceName, MAX_RESOURCE_LENGTH);
	return std::string(resourceName);
}
static void* GetRootElement(lua_State* luaVM) noexcept {
	lua_getglobal(luaVM, "root");
	return lua_touserdata(luaVM, -1);
}
static void* GetResourceRootElement(lua_State* luaVM) noexcept {
	lua_getglobal(luaVM, "resourceRoot");
	return lua_touserdata(luaVM, -1);
}