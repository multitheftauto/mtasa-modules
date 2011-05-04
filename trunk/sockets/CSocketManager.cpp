/******************************************************
*
* Project:    ml_sockets (Win32, Linux).
* Website:    http://multitheftauto-modules.googlecode.com/
* Developers: Gamesnert
*
******************************************************/

#include "CSocketManager.h"

// Vector for holding all sockets
vector <sSocketsStorage*> vecSockets;

void CSocketManager::DoPulse()
{
    // Loop through all sockets
    for (unsigned int i = 0; i < vecSockets.size(); ++i)
    {
        // Get the socket by the ID
        CSocket* pSocket = (CSocket *)vecSockets[i]->m_pSocket;

        // Do a pulse at the socket
        if (!pSocket->DoPulse())
        {
            // If the pulse indicates failure, remove the socket
            SocketRemove(pSocket);
        }
    }
}

bool CSocketManager::SocketLimitExceeded ( void )
{
    unsigned int uiSockets = vecSockets.size();
    if ( uiSockets > MAX_SOCKETS )
    {
        pModuleManager->ErrorPrintf ( "[Sockets] Sockets limit reached! (Max: %i)\n", MAX_SOCKETS );
        return true;
    }

    return false;
}

bool CSocketManager::SocketAdd ( CSocket*& pSocket, bool bListen )
{
    // Add the socket to the loop stuff
    sSocketsStorage* pSocketStorage = new sSocketsStorage ( pSocket, bListen );
    vecSockets.push_back ( pSocketStorage );

    return true;
}

bool CSocketManager::SocketRemove ( CSocket*& pSocket )
{
    // Check if an socket was actually specified
    if ( pSocket == NULL )
        return false;

    // Loop through all sockets
    for (unsigned int i = 0; i < vecSockets.size(); ++i)
    {
        // If the current is the one we're looking for...
        if ((CSocket *)vecSockets[i]->m_pSocket == pSocket)
        {
            // Remove it from the vector and delete it, then return true
            vecSockets.erase(vecSockets.begin() + i);
            SAFE_DELETE(pSocket);
            return true;
        }
    }

    return false;
}

bool CSocketManager::GetSocket ( CSocket*& pSocket, void* pUserdata )
{
    // Make sure a value has been passed. Don't bother if there hasn't been
    if (pUserdata == NULL)
        return false;

    // Loop through all sockets
    for (unsigned int i = 0; i < vecSockets.size(); ++i)
    {
        // Compare the current socket's userdata with the one we're looking for
        if ( (CSocket *)vecSockets[i]->m_pSocket->GetUserdata() == pUserdata )
        {
            // If it's the one we want, assign pSocket to it and return true
            pSocket = (CSocket *)vecSockets[i]->m_pSocket;
            return true;
        }
    }

    return false;
}

void CSocketManager::HandleStop ( void )
{
    // Triggered at module stop. Simply destroys all sockets
    for (unsigned int i = 0; i < vecSockets.size(); ++i)
        SAFE_DELETE((CSocket *)vecSockets[i]->m_pSocket);
}
