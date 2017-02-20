/******************************************************
*
* Project:    ml_sockets (Win32, Linux).
* Website:    http://multitheftauto-modules.googlecode.com/
* Developers: Gamesnert
*
******************************************************/

#pragma once

// General
#define ERR_INVALID_SOCKET  -1
#define ERR_SEND_FAILURE    -1
#define ERR_CONNECT_FAILURE -1
#define ERR_CONNECT_SUCCESS 0
#define ERR_NO_ERROR        0


#ifdef WIN32
    // Win32 specific
    #define ERR_WOULD_BLOCK             WSAEWOULDBLOCK
    #define ERR_CONNECT_IN_PROGRESS     WSAEWOULDBLOCK
    #define ERR_NOT_CONNECTED           WSAENOTCONN
#else
    // Linux specific
    #define ERR_WOULD_BLOCK             EWOULDBLOCK
    #define ERR_CONNECT_IN_PROGRESS     EINPROGRESS
    #define ERR_NOT_CONNECTED           ENOTCONN
#endif
