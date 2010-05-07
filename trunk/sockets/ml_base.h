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

/** MODULE SPECIFIC INFORMATION **/

#ifndef __ML_BASE_H
#define __ML_BASE_H

#ifdef WIN32
	#pragma warning (disable : 4267) // DISABLE: conversion from 'size_t' to 'int', possible loss of data
	#pragma warning (disable : 4996) // DISABLE: 'strcpy': This function or variable may be unsafe.
#endif

// Defines
#define SAFE_DELETE(p) { if (p) { delete (p); (p) = NULL; } }

#define MODULE_NAME			"Sockets Module"
#define	MODULE_AUTHOR		"Gamesnert, MCvarial & x86"
#define MODULE_VERSION		0.1f

// Includes
#include "Common.h"
#include "CFunctions.h"
#include "include/ILuaModuleManager.h"

#endif
