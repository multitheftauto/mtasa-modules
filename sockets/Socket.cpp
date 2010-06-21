/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert & x86
****************************************************************/

#include "Socket.h"

Socket::Socket(lua_State *luaVM, string host, unsigned short port)
{
    m_connected           = false;
    m_awaitingDestruction = false;

    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_port   = htons(port);
    
    if (!VerifyIP(host))
    {
        return;
    }

    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    u_long ulMode = 1;
    ioctlsocket(m_sock, FIONBIO, &ulMode);

    if (m_sock == -1 || connect(m_sock, (sockaddr*)&m_addr, sizeof(m_addr)) != 0)
    {
        int error = WSAGetLastError();

        if (error != WSAEWOULDBLOCK)
        {
            makeAwaitDestruction();
            return;
        }
    }

    m_userdata   = lua_newuserdata(luaVM, 128);
}

Socket::~Socket()
{
    CFunctions::triggerEvent("onSockClosed", m_userdata);

    if (m_connected)
    {
        shutdown(m_sock, 1);
#ifdef WIN32
        closesocket(m_sock);
#else
        close(m_sock);
#endif
    }
}

void Socket::doPulse()
{
    char buffer[SOCK_RECV_LIMIT + 1];

    int retval = recv(m_sock, buffer, SOCK_RECV_LIMIT, 0);

    int iError = WSAGetLastError();

    if (!m_connected && (iError == WSAEWOULDBLOCK || iError == 0))
    {
        m_connected  = true;
        CFunctions::triggerEvent("onSockOpened", m_userdata);
    }

    if (retval > 0)
    {
        buffer[retval] = '\0';

        CFunctions::triggerEvent("onSockData", m_userdata, buffer);
    }
    else if(iError != WSAEWOULDBLOCK && iError != WSAENOTCONN && iError != 0)
    {
        m_connected  = false;
        makeAwaitDestruction();
        return;
    }
}

void Socket::makeAwaitDestruction()
{
    m_awaitingDestruction = true;
}

bool Socket::VerifyIP(const string& host)
{
    hostent* Hostent;
    unsigned long IP = inet_addr(host.c_str());

    if (IP != INADDR_NONE)
    {
        m_addr.sin_addr.s_addr = IP;
        return true;
    }
    else
    {
        Hostent = gethostbyname(host.c_str());
        if (Hostent == NULL)
        {
            return false;
        }
        else
        {
            memcpy(&(m_addr.sin_addr), Hostent->h_addr_list[0], 4);
            return true;
        }
    }
}

bool Socket::sendData(const string& data)
{
    return send(m_sock, data.c_str(), data.length(), 0) != -1;
}

void* Socket::getUserdata()
{
    return m_userdata;
}
