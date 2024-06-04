#pragma once

// Max length of module name and author
#define MAX_INFO_LENGTH 128

#include <lua/lua.hpp>
#include <string>

#ifndef __CChecksum_H
#define __CChecksum_H
struct CChecksum { unsigned long ulCRC; unsigned char mD5[16]; };
#endif

struct ILuaModuleManager {
    virtual void ErrorPrintf(const char* szFormat, ...) = 0;
    virtual void DebugPrintf(lua_State* luaVM, const char* szFormat, ...) = 0;
    virtual void Printf(const char* szFormat, ...) = 0;

    virtual bool RegisterFunction(
        lua_State* luaVM,
        const char* szFunctionName,
        lua_CFunction Func
    ) = 0;

    // This function might not work if module and MTA
    // were compiled with different compiler versions
    virtual bool GetResourceName(lua_State* luaVM, std::string& strName) = 0;
    virtual CChecksum GetResourceMetaChecksum(lua_State* luaVM) = 0;
    virtual CChecksum GetResourceFileChecksum(
        lua_State* luaVM,
        const char* szFile
    ) = 0;
};

struct ILuaModuleManager10 : public ILuaModuleManager {
    virtual unsigned long GetVersion() = 0;
    virtual const char* GetVersionString() = 0;
    virtual const char* GetVersionName() = 0;
    virtual unsigned long GetNetcodeVersion() = 0;
    virtual const char* GetOperatingSystemName() = 0;

    virtual lua_State* GetResourceFromName(const char* szResourceName) = 0;

    // GetResourceName above doesn't work if module and MTA
    // were compiled with different compiler versions
    virtual bool GetResourceName(lua_State* luaVM, char* szName, size_t length) = 0;
    virtual bool GetResourceFilePath(
        lua_State* luaVM,
        const char* fileName,
        char* path,
        size_t length
    ) = 0;
};