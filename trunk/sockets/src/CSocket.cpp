/******************************************************
*
* Project:    ml_sockets (Win32, Linux).
* Website:    http://multitheftauto-modules.googlecode.com/
* Developers: Gamesnert, ccw
*
******************************************************/

#include "CSocket.h"

CSocket::CSocket(lua_State *luaVM, const string& strHost, const unsigned short& usPort)
{
    // Prepare variables
    m_bConnected = false;
    m_strHost    = strHost;
    m_usPort     = usPort;
    m_pLuaVM     = luaVM;
    m_pUserdata  = NULL;
    m_pSocket    = ERR_INVALID_SOCKET;
    
    // Prepare data for connection (cancel on failure)
    if (!ProcessTargetLocation(strHost, usPort))
        return;
    
    // Create the socket, and put it in non-blocking mode
    m_pSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Exit if the socket failed to create
    if (m_pSocket == ERR_INVALID_SOCKET)
        return;

    // Set the socket to non-blocking mode
    SetNonBlocking();

    // Make the socket connect
    if (connect(m_pSocket, (sockaddr*)&m_sSockAddr, sizeof(m_sSockAddr)) != ERR_CONNECT_SUCCESS)
    {
        // If the connection failed, check why
        int iError = GetLastSocketError();

        // If the error is ERR_WOULD_BLOCK (meaning it's in progress) then ignore
        if (iError != ERR_CONNECT_IN_PROGRESS)
        {
            // Error, so unload the socket and exit the function
            CloseSocket();
            return;
        }
    }

    // Create userdata, for identification
    m_pUserdata  = lua_newuserdata(luaVM, 128);
}

CSocket::~CSocket()
{
    // Close the socket, if it exists
    if (m_pSocket != ERR_INVALID_SOCKET)
    {
        CloseSocket();
    }
}

void CSocket::CloseSocketWithEvent()
{
    // Close the socket, if it exists, and trigger the closed event
    if (m_pSocket != ERR_INVALID_SOCKET)
    {
        CloseSocket();
        TriggerEvent("onSockClosed");
    }
}

bool CSocket::Send(const string& data)
{
    // Make sure the socket exists
    if (m_pSocket == ERR_INVALID_SOCKET)
        return false;

    // Pretend zero length send was ok
    if (data.length() == 0)
        return true;

    // Send the data and return whether it was successful
    return (send(m_pSocket, data.c_str(), data.length(), 0) != ERR_SEND_FAILURE);
}

bool CSocket::DoPulse()
{
    // Make sure the socket exists before taking action
    if (m_pSocket != ERR_INVALID_SOCKET)
    {
        // Wait for connect to complete before proceeding
        if (!m_bConnected)
        {
            struct timeval tv = { 0, 0 };
            fd_set wfds;
            FD_ZERO(&wfds);
            FD_SET(m_pSocket, &wfds);
            // See if socket it writable
            int ret = select(m_pSocket+1, NULL, &wfds, NULL, &tv);
            if (ret == 0)
               return true;     // Not writable yet
            if (ret == -1)
               return false;    // select error
        }

        // Create a buffer for catching received data
        // (1 byte larger than receive limit, because another character has to be added)
        char chBuffer[SOCK_RECV_LIMIT + 1];

        // Receive the data
        int iLength = recv(m_pSocket, chBuffer, SOCK_RECV_LIMIT, 0);

        // Check if there were any errors
        int iError = GetLastSocketError();

        // Check if the socket just connected. If connection failed, return false
        if (!m_bConnected && (HandleConnection(iError) == ERR_CONNECT_FAILURE))
            return false;

        // If connected, handle data processing
        if (m_bConnected)
        {
            // Process data if there is any
            if (iLength > 0)
            {
                // Add a NULL at the end of the data, or the data will appear corrupted
                chBuffer[iLength] = '\0';

                TriggerEvent("onSockData",chBuffer);
            }
            else if (iError != ERR_NO_ERROR && iError != ERR_WOULD_BLOCK)
            {
                // An error has occured, so time to kill the socket
                m_bConnected = false;
                return false;
            }
        }
    }
    else
        // If the socket doesn't exist, well, error?
        return false;

    // If the call makes it up till here, it has been a huge success! Cake and true as a reward!
    return true;
}

bool CSocket::IsConnected()
{
    // If there's no socket, then we don't have a connection
    if (m_pSocket == ERR_INVALID_SOCKET)
        return false;

    // If there is, show whether there is a connection
    return m_bConnected;
}

void* CSocket::GetUserdata()
{
    return m_pUserdata;
}

bool CSocket::ProcessTargetLocation(const string& strHost, const unsigned short& usPort)
{
    memset(&m_sSockAddr, 0, sizeof(m_sSockAddr));
    m_sSockAddr.sin_family = AF_INET;
    m_sSockAddr.sin_port   = htons(usPort);

    hostent* Hostent;
    unsigned long IP = inet_addr(strHost.c_str());

    if (IP != INADDR_NONE)
    {
        m_sSockAddr.sin_addr.s_addr = IP;
        return true;
    }
    else
    {
        Hostent = gethostbyname(strHost.c_str());
        if (Hostent == NULL)
        {
            return false;
        }
        else
        {
            memcpy(&(m_sSockAddr.sin_addr), Hostent->h_addr_list[0], 4);
            return true;
        }
    }
}

void CSocket::SetNonBlocking()
{
    // Multi-platform function for setting the socket to non-blocking mode

#ifdef WIN32
    u_long ulMode = 1;
    ioctlsocket(m_pSocket, FIONBIO, &ulMode);
#else
    fcntl(m_pSocket, F_SETFL, fcntl(m_pSocket, F_GETFL) | O_NONBLOCK);
#endif
}

int CSocket::GetLastSocketError()
{
    // Multi-platform function for getting the last socket error

#ifdef WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

void CSocket::CloseSocket()
{
    // Close the connection before closing the socket
    shutdown(m_pSocket, 1);

#ifdef WIN32
    closesocket(m_pSocket);
#else
    close(m_pSocket);
#endif
    
    // Unset the socket variable, so there's no mistaking there
    m_pSocket = ERR_INVALID_SOCKET;
}

int CSocket::HandleConnection(const int& iError)
{
    if (iError == ERR_NO_ERROR || iError == ERR_WOULD_BLOCK)
    {
        m_bConnected = true;
        TriggerEvent("onSockOpened");
        return 1;
    }
    else if (iError == ERR_NOT_CONNECTED)
    {
        return 0;
    }
    else
    {
        printf("Could not connect due to error: %i",iError); // TEMP DEBUG
        return -1;
    }
}

void CSocket::TriggerEvent(const string eventName, const string arg1)
{
    CLuaArguments args;
    args.PushString(eventName.c_str());

    lua_getglobal(m_pLuaVM, "root");
    CLuaArgument RootElement(m_pLuaVM, -1);

    args.PushUserData(RootElement.GetLightUserData());
    args.PushUserData(m_pUserdata);

    if (arg1.length() > 0)
        args.PushString(arg1.c_str());

    args.Call(m_pLuaVM, "triggerEvent");
}
