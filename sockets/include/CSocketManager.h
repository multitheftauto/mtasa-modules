/******************************************************
*
* Project:    ml_sockets (Win32, Linux).
* Website:    http://multitheftauto-modules.googlecode.com/
* Developers: Gamesnert
*
******************************************************/

#ifndef _CSOCKMANAGER_H
#define _CSOCKMANAGER_H

class CSocket;

#include <vector>
#include <deque>
#include <assert.h>
#include "ml_sockets.h"

class CSocketManager
{
public:
    static void DoPulse();
    static void ResourceStopped( lua_State * luaVM );

    static void SocketAdd   (CSocket*& pSocket);
    static void SocketRemove(CSocket*& pSocket, bool bTriggerCloseEvent=true);
    static bool GetSocket   (CSocket*& pSocket, void* pUserdata);
    static bool GetSocketByLuaVM(CSocket*& pSocket, lua_State * luaVM);

    static void HandleStop  ();
};

#endif
