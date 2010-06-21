/****************************************************************
*        Multi Theft Auto : San Andreas - Socket module.
*        Info: Win32/Linux Sockets
*        Developers: Gamesnert, mabako & x86
****************************************************************/

#ifndef __MTA_SOCKETSSL_H
#define __MTA_SOCKETSSL_H

/* OpenSSL headers */
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>
#include "ml_sockets.h"
#include "Socket.h"

class SocketSSL : Socket
{
friend class Socket;
friend class Sockets;
private:
			 SocketSSL				( lua_State *luaVM, const string host, const unsigned short port );
			~SocketSSL				( void ) { };
};

#endif
