/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert, mabako & x86
****************************************************************/

#include "Socket.h"
#include <sstream>

Socket::Socket( lua_State *luaVM )
{
	m_bConnected           = false;
	m_bAwaitingDestruction = false;
	m_pUserData            = lua_newuserdata(luaVM, 128);
	m_pBIO                 = NULL;
}

Socket::Socket( lua_State *luaVM, const string host, const unsigned short port )
{
	m_bConnected           = false;
	m_bAwaitingDestruction = false;
	m_pUserData            = lua_newuserdata(luaVM, 128);
	m_pBIO                 = NULL;
	
	// Concat host and port
	stringstream host_port;
	host_port << host << ":" << port;
	
	// Create a new connection
	m_pBIO = BIO_new_connect( (char *) host_port.str( ).c_str( ) );
	if( m_pBIO == NULL )
	{
		return;
	}
	
	// Use non-blocking I/O
	BIO_set_nbio( m_pBIO, 1 );
	
	// Actually connect
	while( BIO_do_connect( m_pBIO ) <= 0 )
	{
		if( BIO_should_retry( m_pBIO ) )
#ifdef WIN32
			Sleep(1);
#else
			usleep(1);
#endif
		else
			return;
	}
}

Socket::~Socket( )
{
	if( m_bConnected && m_pUserData )
	{
		CFunctions::triggerEvent( "onSockClosed", m_pUserData );
	}
	
	if( m_pBIO )
	{
		BIO_free_all( m_pBIO );
	}
}

void Socket::DoPulse( )
{
	if( !m_bConnected )
	{
		// Trigger the event the first time we pulse it.
		CFunctions::triggerEvent( "onSockOpened", m_pUserData );
		m_bConnected = true;
	}
	
	// Read some stuff
	char pBuffer[SOCK_RECV_LIMIT + 1];
	int iReadLength = BIO_read( m_pBIO, pBuffer, sizeof(pBuffer)-1 ); // >0: success, length; == 0: no data read; <0: error
	
	if( iReadLength > 0 )
	{
		pBuffer[iReadLength] = '\0';
		CFunctions::triggerEvent("onSockData", m_pUserData, pBuffer);
	}
}

bool Socket::SendData( const string& data )
{
	return BIO_write( m_pBIO, data.c_str( ), data.length( ) ) > 0;
}
