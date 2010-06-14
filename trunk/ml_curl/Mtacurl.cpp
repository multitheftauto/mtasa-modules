/****************************************************************
*        Multi Theft Auto : San Andreas - Curl module.
*        Info: Win32/Linux Curl
*        Developers: mrdejong
****************************************************************/

#include "Mtacurl.h"

Mtacurl::Mtacurl( lua_State *luaVM )
{
	m_pUserData				= lua_newuserdata(luaVM, 128);
	m_pCurl					= curl_easy_init();
	m_pConnected			= true;
	m_bAwaitingDestruction	= false;
}

Mtacurl::Mtacurl( lua_State *luaVM, const char* url )
{
	m_pUserData				= lua_newuserdata(luaVM, 128);
	m_pCurl					= curl_easy_init();
	char* host				= curl_easy_escape(m_pCurl, url, strlen(url));
	curl_easy_setopt(m_pCurl, CURLOPT_URL, host);
	m_pConnected			= true;
	m_bAwaitingDestruction	= false;
}

Mtacurl::~Mtacurl( )
{
	curl_easy_cleanup(m_pCurl);
	m_pCurl					= NULL;
	m_pUserData				= NULL;
	m_pConnected			= false;
}

CURLcode Mtacurl::setopt_boolean( CURLoption option, bool val)
{
	return curl_easy_setopt(m_pCurl, option, val);
}

CURLcode Mtacurl::setopt_number( CURLoption option, int val)
{
	return curl_easy_setopt(m_pCurl, option, val);
}

CURLcode Mtacurl::setopt_string( CURLoption option, const char* val)
{
	return curl_easy_setopt(m_pCurl, option, val);
}

void Mtacurl::DoPulse( )
{

}