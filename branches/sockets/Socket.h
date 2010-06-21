/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert, mabako & x86
****************************************************************/

#ifndef __MTA_SOCKET_H
#define __MTA_SOCKET_H

/* OpenSSL headers */
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#include "ml_sockets.h"

class Socket
{
friend class Sockets;
public:
			 Socket					( lua_State *luaVM );
private:
			 Socket					( lua_State *luaVM, const string host, const unsigned short port );
protected:
			~Socket					( void );

	void	DoPulse					( void );

public:
	bool	SendData				( const string& data );

	bool	IsAwaitingDestruction	( void ) { return m_bAwaitingDestruction; };
	void	MakeAwaitDestruction	( void ) { m_bAwaitingDestruction = true; };

	void*	GetUserData				( void )	{ return m_pUserData; };

protected:
	BIO*	m_pBIO;

private:
	void*	m_pUserData;

	bool	m_bConnected;
	bool	m_bAwaitingDestruction;
};

#endif
