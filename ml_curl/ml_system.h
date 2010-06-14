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

/** MODULE SPECIFIC INFORMATION **/
#ifndef __ml_system_H
#define __ml_system_H

// Disabling warnings
#ifdef WIN32
    #pragma warning (disable : 4267) // DISABLE: conversion from 'size_t' to 'int', possible loss of data
    #pragma warning (disable : 4996) // DISABLE: 'strcpy': This function or variable may be unsafe.
#endif

// Function for making sure a pointer has a value before deleting it; possibly prevents crashes
#define SAFE_DELETE(p) { if (p) { delete (p); (p) = NULL; } }

#define MODULE_NAME         "Curl module"
#define MODULE_AUTHOR       "Alexander de Jong (AKA mrdejong)"
#define MODULE_VERSION      1.0

#ifdef WIN32
    #define WIN32_MEAN_AND_LEAN
#endif

// Includes
#include <string.h>
#include <stdio.h>

#include "Common.h"
#include "Mtacurls.h"
#include "CFunctions.h"
#include "include/ILuaModuleManager.h"

using namespace std;

#endif