/******************************************************
*
* Project:    ml_sockets (Win32, Linux).
* Website:    http://multitheftauto-modules.googlecode.com/
* Developers: Gamesnert
*
******************************************************/

#include "CSocketManager.h"

// Vector for holding all sockets
vector <CSocket*> vecSockets;
deque <CSocket*> processQueue;
deque <CSocket*> deleteList;

void CSocketManager::DoPulse()
{
    // Loop through all sockets
    processQueue = deque <CSocket*>(vecSockets.begin(), vecSockets.end());
    while (!processQueue.empty())
    {
        CSocket* pSocket = processQueue.front();
        processQueue.pop_front();

        // Do a pulse at the socket
        if (!pSocket->DoPulse())
        {
            // If the pulse indicates failure, remove the socket
            SocketRemove(pSocket);
        }
    }

    // Finally cleanup sockets that were removed
    while (!deleteList.empty())
    {
        CSocket* pSocket = deleteList.front();
        deleteList.pop_front();
        SAFE_DELETE(pSocket);
    }
}

void CSocketManager::SocketAdd(CSocket*& pSocket)
{
    // Add the socket to the loop stuff
    vecSockets.push_back(pSocket);
}

void CSocketManager::SocketRemove(CSocket*& pSocket)
{
    ListRemove(vecSockets, pSocket);
    ListRemove(processQueue, pSocket);
    pSocket->CloseSocketWithEvent();
    ListRemove(deleteList, pSocket);
    deleteList.push_back(pSocket);
}

bool CSocketManager::GetSocket(CSocket*& pSocket, void* pUserdata)
{
    // Make sure a value has been passed. Don't bother if there hasn't been
    if (pUserdata == NULL)
        return false;

    // Loop through all sockets
    for (unsigned int i = 0; i < vecSockets.size(); ++i)
    {
        // Compare the current socket's userdata with the one we're looking for
        if (vecSockets[i]->GetUserdata() == pUserdata)
        {
            // If it's the one we want, assign pSocket to it and return true
            pSocket = vecSockets[i];
            return true;
        }
    }

    return false;
}

void CSocketManager::HandleStop()
{
    // Triggered at module stop. Simply destroys all sockets
    for (unsigned int i = 0; i < vecSockets.size(); ++i)
        SAFE_DELETE(vecSockets[i]);

    for (unsigned int i = 0; i < deleteList.size(); ++i)
        SAFE_DELETE(deleteList[i]);

    vecSockets.clear();
    processQueue.clear();
    deleteList.clear();
}
