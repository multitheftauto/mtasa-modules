/**
mtacurl

curl extention class
*/

#ifndef __MTACURL_H
#define __MTACURL_H

#include <string>
#include "ml_system.h"

#include "include/curl/curl.h"

class Mtacurl
{
friend class Mtacurls;
public:
			Mtacurl					( lua_State *luaVM );
private:
			Mtacurl					( lua_State *luaVM, const char* url );
protected:
			~Mtacurl				( void );
	void	DoPulse					( void );

public:
	CURLcode setopt_boolean			( CURLoption option, bool val );
	CURLcode setopt_number			( CURLoption option, int val );
	CURLcode setopt_string			( CURLoption option, const char* val );

	bool	IsAwaitingDestruction	( void ) { return m_bAwaitingDestruction; };
	void	MakeAwaitDestruction	( void ) { m_bAwaitingDestruction = true; };
	void*	GetUserData				( void ) { return m_pUserData; };

private:
	void*	m_pUserData;
	bool	m_pConnected;
	CURL*	m_pCurl;
	bool	m_bAwaitingDestruction;
};

#endif