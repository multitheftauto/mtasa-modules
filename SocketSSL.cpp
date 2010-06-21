/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert, mabako & x86
****************************************************************/

#include "SocketSSL.h"
#include <sstream>

extern Sockets* sockets;

SocketSSL::SocketSSL( lua_State *luaVM, const string host, const unsigned short port ) : Socket( luaVM )
{
	// Concat host and port
	stringstream host_port;
	host_port << host << ":" << port;
	
	// New BIO for SSL
	m_pBIO = BIO_new_ssl_connect( sockets->GetSSLContext( ) );
	
	// Something with key expiration handled by openssl
	SSL* ssl;
	BIO_get_ssl( m_pBIO, &ssl );
	SSL_set_mode( ssl, SSL_MODE_AUTO_RETRY );
	
	// Where we want to connect to
	BIO_set_conn_hostname( m_pBIO, (char *) host_port.str( ).c_str( ) );
	
	// Actually connect
	while( BIO_do_handshake( m_pBIO ) <= 0 )
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

	// Use non-blocking I/O
	BIO_set_nbio( m_pBIO, 1 );
}
