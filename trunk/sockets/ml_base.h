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

// Disabling warnings
#ifdef WIN32
    #pragma warning (disable : 4267) // DISABLE: conversion from 'size_t' to 'int', possible loss of data
    #pragma warning (disable : 4996) // DISABLE: 'strcpy': This function or variable may be unsafe.
#endif

// Function for making sure a pointer has a value before deleting it; possibly prevents crashes
#define SAFE_DELETE(p) { if (p) { delete (p); (p) = NULL; } }

// Defines
#define MODULE_NAME            "Sockets Module"
#define MODULE_AUTHOR          "Gamesnert, MCvarial & x86"
#define MODULE_VERSION         0.1f

#define SOCK_RECV_LIMIT 16384

#ifdef WIN32
    #define WIN32_MEAN_AND_LEAN
#endif

// Includes
#include <string.h>
#include <stdio.h>

#ifdef WIN32
    #include <winsock2.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
//  #include <pthread.h>
    #include <netdb.h>
    #include <cstdlib>
    #include <sys/select.h>
#endif

#include "Common.h"
#include "CFunctions.h"
#include "include/ILuaModuleManager.h"

// Setting namespace to std (so we don't have to do "std::" all the time)
using namespace std;

#endif
