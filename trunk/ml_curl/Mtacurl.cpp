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
	// char* host				= curl_easy_escape(m_pCurl, url, strlen(url));
	m_pConnected			= true;
	m_bAwaitingDestruction	= false;

	curl_easy_setopt(m_pCurl, CURLOPT_URL, url);
}

Mtacurl::~Mtacurl( )
{
	curl_easy_cleanup(m_pCurl);
	m_pCurl					= NULL;
	m_pUserData				= NULL;
	m_pConnected			= false;
}

static size_t writeData			( char *data, size_t size, size_t nmemb, void *ctx )
{
	size_t          length = size * nmemb;
	
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

CURLcode Mtacurl::perform( void )
{
	return curl_easy_perform( m_pCurl );
}

const void* Mtacurl::getResult( void )
{
	return m_pBuffer;
}

CURLcode Mtacurl::send( void )
{
	m_pBuflen = sizeof(m_pBuffer);
	return curl_easy_send(m_pCurl, m_pBuffer, m_pBuflen, m_pBufsize);
}

void Mtacurl::cleanup( void )
{
	return curl_easy_cleanup(m_pCurl);
}

char *Mtacurl::escape( const char* url )
{
	return curl_easy_escape(m_pCurl, url, sizeof(url));
}

const char* Mtacurl::strerror( CURLcode error )
{
	return curl_easy_strerror( error );
}

void Mtacurl::DoPulse( )
{
	if(sizeof(m_pBuffer) > 0)
	{
		CFunctions::triggerEvent("onCurlResult", GetUserData());
	}
}