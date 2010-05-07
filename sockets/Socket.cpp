/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert & x86
****************************************************************/

#include "Socket.h"

Socket::Socket(lua_State *luaVM, string host, unsigned short port)
{
    m_connecting = false;
    m_connected  = false;

    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_port   = htons(port);
    
    if (!VerifyIP(host.c_str()))
    {
        return;
    }

    m_connecting = true;

    // Create Thread
#ifdef WIN32
    m_thread     = (HANDLE)_beginthread(&CFunctions::doPulse, 0, this);
#else
    m_thread     = pthread_create(&m_thread, NULL, CFunctions::doPulse, this);
#endif

    m_userdata   = lua_newuserdata(luaVM, 128);
}

Socket::~Socket()
{
    CFunctions::triggerEvent("onSockClosed", m_userdata, "nil", "nil");

#ifdef WIN32
    if ( m_thread )
    {
        CloseHandle(m_thread);
    }

    if (m_connected)
    {
        shutdown(m_sock, 1);
        closesocket(m_sock);
    }
#else
    if ( m_thread )
    {
        pthread_detach(m_thread);
        pthread_cancel(m_thread);
    }

    if (m_connected)
    {
        shutdown(m_sock, 1);
        close(m_sock);
    }
#endif

    m_connecting = false;
    m_connected  = false;
}

void Socket::doPulse()
{
    if (m_connecting)
    {
        m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_sock == -1 || connect(m_sock, (sockaddr*)&m_addr, sizeof(m_addr)) != 0)
        {
            m_connecting = false;
            return;
        }

        CFunctions::triggerEvent("onSockOpened",m_userdata,"nil","nil");

        m_connected  = true;
        m_connecting = false;
    }
    else if (m_connected)
    {
        char buffer[SOCK_RECV_LIMIT + 1];

        int retval = recv(m_sock, buffer, SOCK_RECV_LIMIT, 0);

        if (retval > 0)
        {
            buffer[retval] = '\0';

            CFunctions::triggerEvent("onSockData", m_userdata, buffer, "nil");
        }
        else
        {
            m_connected  = false;
            m_connecting = false;
            return;
        }
    }
}

bool Socket::isConnected()
{
    return m_connected;
}

bool Socket::isConnecting()
{
    return m_connecting;
}

bool Socket::VerifyIP(string host)
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

bool Socket::sendData(string data)
{
    if (send(m_sock, data.c_str(), data.length(), 0) == -1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void* Socket::getUserdata()
{
    return m_userdata;
}
