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

#ifndef _MLSOCK_H
#define _MLSOCK_H

#include <assert.h>

/** MODULE SPECIFIC INFORMATION **/
#define MODULE_NAME			"Sockets Module"
#define	MODULE_AUTHOR		"Gamesnert, MCvarial & x86"
#define MODULE_VERSION		1.3f

// Include default MTA module SDK includes
#include "Common.h"
#include "CFunctions.h"
#include "include/ILuaModuleManager.h"
#include "extra/CLuaArguments.h"

// Tie the project together
#include "CSocket.h"
#include "CSocketManager.h"

// Set the using namespace to std, so we don't need the annoying std:: anymore
using namespace std;

// Function for making sure a pointer has a value before deleting it; possibly prevents crashes
#define SAFE_DELETE(p) { if (p) { delete (p); (p) = NULL; } }

#endif
