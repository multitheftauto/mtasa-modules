/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert, mabako & x86
****************************************************************/

#include "Sockets.h"
#include "Socket.h"
#include "SocketSSL.h"

Sockets::Sockets( void )
{
	/* Initializing OpenSSL */
	SSL_library_init();
	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
	
	// Initalize our SSL context: Any protocol is fine, we're not picky today
	m_pCTX = SSL_CTX_new(SSLv23_client_method());
	if( m_pCTX )
	{
		// We don't want to verify anything
		SSL_CTX_set_verify(m_pCTX, SSL_VERIFY_NONE, 0);
	}
}

Sockets::~Sockets( void )
{
	if( m_pCTX )
	{
		SSL_CTX_free( m_pCTX );
		m_pCTX = NULL;
	}
}

Socket* Sockets::Create( lua_State* luaVM, const string host, const unsigned short port, bool ssl )
{
	Socket* pSocket;
	if( ssl )
		pSocket = new SocketSSL( luaVM, host, port );
	else
		pSocket = new Socket( luaVM, host, port );

	if( pSocket )
	{
		sockets.push_back( pSocket );
		return pSocket;
	}
	return NULL;
}

Socket* Sockets::Get( void* pUserData )
{
	for( vector < Socket* >::iterator iter = sockets.begin( ); iter != sockets.end( ); ++ iter )
		if( (*iter)->GetUserData( ) == pUserData )
			return *iter;
	return NULL;
}

void Sockets::DoPulse( )
{
	unsigned int i = 0;
	while( i < sockets.size( ) )
	{
		Socket* pSocket = sockets[i];
		if( pSocket )
		{
			if( !pSocket->IsAwaitingDestruction( ) )
			{
				pSocket->DoPulse( );
				++ i;
			}
			else
			{
				sockets.erase( sockets.begin() + i );
				delete pSocket;
			}
		}
		else
		{
			// Whoops, already gone? This might point to us doing something dumb
			sockets.erase( sockets.begin() + i );
		}
	}
}
