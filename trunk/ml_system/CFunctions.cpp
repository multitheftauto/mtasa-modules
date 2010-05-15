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

#include <stdlib.h>
#include "CFunctions.h"
#include "extra/CLuaArguments.h"

int CFunctions::System ( lua_State* luaVM )
{
    if ( luaVM )
    {
        if (lua_type(luaVM, 1) == LUA_TSTRING)
        {
            const char* m_bCommand = lua_tostring(luaVM, 1);

            /**********************************************
            *   System command (supports: Win32 & Linux).
            *   Win32 return value: 1 = fail.
            *   Linux return value: -1 = fail.
            **********************************************/
            int result = system(m_bCommand);
#ifdef WIN32
            if ( result != 1 )
#else
            if ( result != -1 )
#endif
            {
                lua_pushboolean(luaVM, true);
                return 1;
            }
        }
    }

    lua_pushboolean(luaVM, false);
    return 1;
}