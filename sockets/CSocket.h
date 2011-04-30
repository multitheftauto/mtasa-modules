/******************************************************
*
* Project:    ml_sockets (Win32, Linux).
* Website:    http://multitheftauto-modules.googlecode.com/
* Developers: Gamesnert, x86
*
******************************************************/

#ifndef _CSOCK_H
#define _CSOCK_H

#include "ml_sockets.h"
#include "SocketErrors.h"
#include <string.h>

#define SOCK_RECV_LIMIT 1024 // 1 kB

#ifdef WIN32
    #include <winsock2.h>

    #undef errno
    #define errno WSAGetLastError()
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <cstdlib>
    #include <sys/select.h>
    #include <errno.h>
    #include <fcntl.h>

    typedef int SOCKET;
#endif

class CSocket
{
public:
    CSocket                     (lua_State *luaVM, const string& strHost, const unsigned short& usPort);
    ~CSocket                    ();

    bool Send                   (const string& data);
    bool DoPulse                ();
    bool IsConnected            ();
    int  GetLastSocketError     ();

    void* GetUserdata           ();

private:
    bool ProcessTargetLocation (const string& strHost, const unsigned short& usPort);
    void SetNonBlocking        ();
    void CloseSocket           ();
    int  HandleConnection      (const int& iError);
    void TriggerEvent          (const string eventName, const string arg1 = "");

    string             m_strHost;
    unsigned short     m_usPort;
    void*              m_pUserdata;
    bool               m_bConnected;
    lua_State*         m_pLuaVM;
    struct sockaddr_in m_sSockAddr;
    
    SOCKET             m_pSocket;
};

#endif
