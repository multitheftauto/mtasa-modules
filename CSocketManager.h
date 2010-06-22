#ifndef _CSOCKMANAGER_H
#define _CSOCKMANAGER_H

class CSocket;

#include <vector>
#include "ml_sockets.h"

class CSocketManager {
public:
    static void DoPulse();
    
    static void SocketAdd   (CSocket*& pSocket);
    static bool SocketRemove(CSocket*& pSocket);
    static bool GetSocket   (CSocket*& pSocket, void* pUserdata);

    static void HandleStop  ();
};

#endif
