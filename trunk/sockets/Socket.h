/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert & x86
****************************************************************/

#ifndef __MTA_SOCK_H
#define __MTA_SOCK_H

#include "ml_base.h"

class Socket
{
public:
    Socket                     ( lua_State *luaVM, string host, unsigned short port );
    ~Socket                    ( );

    bool isAwaitingDestruction ( ) { return m_awaitingDestruction; }
    bool sendData              ( const string& data );
    bool VerifyIP              ( const string& host );

    void doPulse               ( );
    void makeAwaitDestruction  ( );

    void* getUserdata          ( );

private:
#ifdef WIN32
    SOCKET             m_sock;
    SOCKADDR_IN        m_addr;
#else
    int                m_sock;
    struct sockaddr_in m_addr;
#endif

    void* m_userdata;

    bool m_connected;
    bool m_awaitingDestruction;
};

#endif
