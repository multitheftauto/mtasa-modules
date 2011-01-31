/****************************************************************
*        Multi Theft Auto : San Andreas - Curl module.
*        Info: Win32/Linux Curl
*        Developers: mrdejong
****************************************************************/

#include "Mtacurl.h"

/**
Some handler functions...
*/
static int l_easy_headerhandler(void *ptr, size_t size, size_t nmemb, void *stream)
{
	lua_State* L = (lua_State*)stream;
	lua_getfield(L, -1, "headerfunction");
	lua_pushlstring(L, (char*) ptr, nmemb * size);
	lua_call(L, 1, 0);
	return nmemb*size;
}

static int l_easy_writehandler(void *ptr, size_t size, size_t nmemb, void *stream)
{
	lua_State* L = (lua_State*)stream;

	lua_getfield(L, -1, "writefunction");
	lua_pushlstring(L, (char*)ptr, nmemb * size);
	lua_call(L, 1, 0);
	return nmemb*size;
}

static int l_easy_readhandler(void *ptr, size_t size, size_t nmemb, void *stream)
{
	lua_State* L = (lua_State*)stream;
	size_t n;
	int old_top = lua_gettop(L);
	const char *str;
	lua_getfield(L, -1, "readfunction");
	lua_pushinteger(L, nmemb * size);
	lua_call(L, 1, 1);
	str = lua_tolstring(L, -1, &n);
	if (n > nmemb*size)
		luaL_error(L, "String returned from readfunction is too long (%d)", n);
	memcpy(ptr, str, n);
	lua_settop(L, old_top);
	return n;
}

/**
Curl stuff!
*/
Mtacurl::Mtacurl( lua_State *luaVM )
{
	m_pUserData				= lua_newuserdata(luaVM, 128);
	m_pCurl					= curl_easy_init();
	m_pConnected			= true;
	m_bAwaitingDestruction	= false;
}

Mtacurl::Mtacurl( lua_State *luaVM, const char* url, size_t length )
{
	m_pUserData				= lua_newuserdata(luaVM, 128);
	m_pCurl					= curl_easy_init();
	char* host				= curl_easy_escape(m_pCurl, url, length);
	m_pConnected			= true;
	m_bAwaitingDestruction	= false;

	curl_easy_setopt(m_pCurl, CURLOPT_URL, host);
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

CURLcode Mtacurl::perform( void )
{
	return curl_easy_perform( m_pCurl );
}

const void* Mtacurl::getResult( void )
{
	return m_pBuffer;
}

/**
@remove
*/
CURLcode Mtacurl::send( void )
{
	m_pBuflen = sizeof(m_pBuffer);
	return curl_easy_send(m_pCurl, m_pBuffer, m_pBuflen, m_pBufsize);
}

int Mtacurl::setup_writefunction( lua_State *L ) 
{
	CURLcode iDataSetup = curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, L);
	if(iDataSetup != CURLE_OK)
		luaL_error(L, "%s", strerror( iDataSetup ));

	CURLcode iFuncSetup = curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, l_easy_writehandler);
	if(iFuncSetup != CURLE_OK)
		luaL_error(L, "%s", strerror( iFuncSetup ));
	return 0;
}

int Mtacurl::setup_readfunction( lua_State *L )
{
	CURLcode iDataSetup = curl_easy_setopt(m_pCurl, CURLOPT_READDATA, L);
	if(iDataSetup != CURLE_OK)
		luaL_error(L, "%s", strerror( iDataSetup ));

	CURLcode iFuncSetup = curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, l_easy_readhandler);
	if(iFuncSetup != CURLE_OK)
		luaL_error(L, "%s", strerror( iFuncSetup ));
	return 0;
}

int Mtacurl::setup_headerfunction( lua_State *L )
{
	CURLcode iDataSetup = curl_easy_setopt(m_pCurl, CURLOPT_WRITEHEADER, L);
	if(iDataSetup != CURLE_OK)
		luaL_error(L, "%s", strerror( iDataSetup ));

	CURLcode iFuncSetup = curl_easy_setopt(m_pCurl, CURLOPT_HEADERFUNCTION, l_easy_headerhandler);
	if(iFuncSetup != CURLE_OK)
		luaL_error(L, "%s", strerror( iFuncSetup ));
	return 0;
}

int Mtacurl::clear_writefunction( lua_State *L )
{
	CURLcode result = curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, NULL);
	if(result != CURLE_OK)
		luaL_error(L, "%s", strerror( result ));
	return 0;
}

int Mtacurl::clear_readfunction( lua_State *L )
{
	CURLcode result = curl_easy_setopt(m_pCurl, CURLOPT_READFUNCTION, NULL);
	if(result != CURLE_OK)
		luaL_error(L, "%s", strerror( result ));
	return 0;
}

int Mtacurl::clear_headerfunction( lua_State *L )
{
	CURLcode result = curl_easy_setopt(m_pCurl, CURLOPT_HEADERFUNCTION, NULL);
	if(result != CURLE_OK)
		luaL_error(L, "%s", strerror( result ));
	return 0;
}

void Mtacurl::cleanup( void )
{
	return curl_easy_cleanup(m_pCurl);
}

char *Mtacurl::escape( const char* url, size_t length )
{
	return curl_easy_escape(m_pCurl, url, length);
}

const char* Mtacurl::strerror( CURLcode error )
{
	return curl_easy_strerror( error );
}

void Mtacurl::DoPulse( )
{
	// Everything here sucks like hell....
	/**if(sizeof(m_pBuffer) > 0)
	{
		CFunctions::triggerEvent("onCurlResult", GetUserData());
	}*/
}