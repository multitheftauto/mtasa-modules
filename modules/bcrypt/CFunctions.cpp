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

#include "CFunctions.h"
#include "extra/CLuaArguments.h"
#include <cstring>
#include <random>
#include <algorithm>

namespace blowfish
{
    extern "C"
    {
        #include "libs/blowfish/ow-crypt.h"
    }
}

#define HASH_SIZE 60
#define SALT_SIZE 30 
#define ENTROPY_SIZE 32


int CFunctions::BcryptDigest ( lua_State* L )
{
    if ( L )
    {
        const char* key = luaL_checkstring ( L, 1 );
        const char* salt = luaL_checkstring ( L, 2 );

        char hash [HASH_SIZE+1];
        blowfish::crypt_rn ( key, salt, hash, sizeof(hash) );
        lua_pushlstring ( L, hash, HASH_SIZE );

        return 1;
    }

    lua_pushboolean ( L, false );
    return 1;
}

int CFunctions::BcryptSalt ( lua_State* L )
{
    if ( L )
    {
        unsigned long logRounds = luaL_checkinteger ( L, 1 );

        char salt [SALT_SIZE];
        char entropy [ENTROPY_SIZE];
        
        std::random_device rd;
        std::mt19937 gen ( rd ( ) );
        std::generate_n ( entropy, ENTROPY_SIZE, gen );
        

        blowfish::crypt_gensalt_rn ( "$2y$", logRounds, entropy, sizeof ( entropy ), salt, sizeof ( salt ) );
        lua_pushlstring ( L, salt, sizeof ( salt ) );

        return 1;
    }

    lua_pushboolean ( L, false );
    return 1;
}

int CFunctions::BcryptVerify ( lua_State* L )
{
    if ( L )
    {
        const char* key = luaL_checkstring ( L, 1 );
        const char* digest = luaL_checkstring ( L, 2 );

        char hash [HASH_SIZE+1];
        memset ( hash, 0, sizeof ( hash ) );

        blowfish::crypt_rn ( key, digest, hash, sizeof ( hash ) );

        int verified = strncmp ( hash, digest, sizeof ( hash ) ) == 0;

        lua_pushboolean ( L, verified );

        return 1;
    }

    lua_pushboolean ( L, false );
    return 1;
}