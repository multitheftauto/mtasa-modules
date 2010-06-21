/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert, mabako & x86
****************************************************************/

#ifndef __MTA_SOCKETS_H
#define __MTA_SOCKETS_H

/* OpenSSL headers */
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#include "ml_sockets.h"

class Socket;
class Sockets
{
public:
			 Sockets( void );
			~Sockets( void );

	Socket*	Create( lua_State* luaVM, const string host, const unsigned short port, bool ssl = false );
	Socket*	Get( void* pUserData );
	bool	Delete( Socket* pSocket );

	void	DoPulse( );

	SSL_CTX*	GetSSLContext( void )		{ return m_pCTX; };
private:
	vector < Socket* > sockets;
	
	SSL_CTX*	m_pCTX;
};

#endif
