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
#include "ml_sockets.h"
#include "include/ILuaModuleManager.h"
extern ILuaModuleManager10 *pModuleManager;

#define MAX_SOCKETS 255

struct sSocketsStorage
{
    CSocket*    m_pSocket;
    bool        m_bListen;

    sSocketsStorage ( CSocket* pSocket, bool bListen = false )
    {
        m_pSocket = pSocket;
        m_bListen = bListen;
    }
};

class CSocketManager
{
public:
    static void DoPulse     ();
    
    static bool SocketLimitExceeded   ( void );
    static bool SocketAdd   ( CSocket*& pSocket, bool bListen = false );
    static bool SocketRemove( CSocket*& pSocket );
    static bool GetSocket   ( CSocket*& pSocket, void* pUserdata );

    static void HandleStop  ();
};

#endif
