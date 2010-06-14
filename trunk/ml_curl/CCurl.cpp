/*********************************************************
*
*  Multi Theft Auto: San Andreas
*
*  Curl module for mta.
*
*  Developers:
*  Alexander de Jong <alexander.t.dejong@gmail.com>
*
*********************************************************/

#include <stdlib.h>
#include "CCurl.h"

CCurl::CCurl(lua_State* luaVM)
{
	m_pUserData			= lua_newuserdata(luaVM, 128);
	m_pCurl				= NULL;
	curl_global_init(CURL_GLOBAL_ALL);
}

CCurl::~CCurl()
{
	curl_easy_cleanup(m_pCurl);
	m_pUserData			= NULL;
	m_pCurl				= NULL;
	curl_global_cleanup();
}

CCurl* CCurl::init(lua_State* luaVM, const char* url)
{
	CCurl* pCurl;

	pCurl = new CCurl(luaVM);

	if(pCurl)
	{
		pCurl->m_pCurl = curl_easy_init();
		curl_easy_setopt(pCurl->m_pCurl, CURLOPT_URL, pCurl->escape(url));
		mtacurl.push_back( pCurl );
		return pCurl;
	}
	return NULL;
}

char* CCurl::escape(const char* url)
{
	return curl_easy_escape(m_pCurl, url, strlen(url));
}

void CCurl::cleanup()
{
	return curl_easy_cleanup(m_pCurl);
}

void CCurl::close()
{
	return CCurl::~CCurl();
}


// internal
CCurl* CCurl::Get(void* pUserData)
{
	for ( vector < CCurl* >::iterator iter = mtacurl.begin( ); iter != mtacurl.end( ); ++ iter )
		if( (* iter)->getUserData( ) == pUserData )
			return *iter;
	return NULL;
}

void CCurl::doPulse()
{
	unsigned int i = 0;
	if(mtacurl.size())
	{
		while(i < mtacurl.size())
		{
			CCurl* pCurl = mtacurl[i];
			if(pCurl)
			{
				++ i;
			}
			else
			{
				mtacurl.erase(mtacurl.begin() + i );
			}
		}
	}
}