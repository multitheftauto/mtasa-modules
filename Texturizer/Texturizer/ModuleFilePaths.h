/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42
*
******************************************************/

#ifndef __MODULEFILEPATHS_H
#define __MODULEFILEPATHS_H

#include <stdio.h>
#include "include/ILuaModuleManager.h"

extern ILuaModuleManager10 *pModuleManager;

/**
 *  The following functions are based on OR taken from the following files in MTA: SA source code:
 *  
 *   *    MTA10_Server/mods/deathmatch/logic/CResourceManager.cpp
 *   *    MTA10_Server/mods/deathmatch/logic/Utils.h
 *   *    MTA10_Server/mods/deathmatch/logic/Utils.cpp
 *   *    MTA10/mods/shared_logic/Utils.h
 *   *    MTA10/mods/shared_logic/Utils.cpp
 */

bool ParseResourcePathInput ( std::string strInput, std::string rName, std::string &strPath, std::string &strMetaPath );

#endif // __MODULEFILEPATHS_H