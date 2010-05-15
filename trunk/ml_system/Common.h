/*********************************************************
*
*  Multi Theft Auto: San Andreas
*
*  Shell module which can execute commands.
*
*  Developers:
*  x86 <sebasdevelopment@gmx.com>
*
*********************************************************/

extern "C"
{
    #include "include/lua.h"
    #include "include/lualib.h"
    #include "include/lauxlib.h"
}

#ifdef WIN32
    #define MTAEXPORT extern "C" __declspec(dllexport)
#else
    #define MTAEXPORT extern "C"
#endif

#include <list>
#include <vector>
// Obviously i can't get us this so other includes will most likely be needed later on

using namespace std;

#ifndef __COMMON_H
#define __COMMON_H

// used in the function argument vector
#define MAX_ARGUMENTS 10
struct FunctionArguments
{
    lua_State* luaVM;
    unsigned char nArguments;
    unsigned char Type[10];
    void*   Arguments[10];
};

namespace FunctionArgumentType
{
    enum
    {
        TYPE_NUMBER = 1,
        TYPE_STRING = 2,
        TYPE_LIGHTUSERDATA = 3,
        TYPE_BOOLEAN = 4,
        TYPE_NIL = 5,
        TYPE_TABLE = 6
    };
}
#endif
