#include "Mtacurls.h"
#include "Mtacurl.h"
#include "include/curl/curl.h"

Mtacurls::Mtacurls( void )
{
	curl_global_init(CURL_GLOBAL_ALL);
}

Mtacurls::~Mtacurls( void )
{
	curl_global_cleanup();
}

Mtacurl* Mtacurls::Create( lua_State* luaVM, const char* url )
{
	Mtacurl* pMtacurl;

	if(url == NULL) {
		pMtacurl = new Mtacurl(luaVM);
	}
	else {
		pMtacurl = new Mtacurl(luaVM, url);
	}

	if(pMtacurl)
	{
		mtacurls.push_back(pMtacurl);
		return pMtacurl;
	}
	return NULL;
}

Mtacurl* Mtacurls::Get( void* pUserData )
{
	for ( vector < Mtacurl* >::iterator iter = mtacurls.begin( ); iter != mtacurls.end( ); ++ iter )
		if( (*iter)->GetUserData( ) == pUserData )
			return *iter;
	return NULL;
}

void Mtacurls::DoPulse( )
{
	unsigned int i = 0;
	while( i < mtacurls.size() )
	{
		Mtacurl* pMtacurl = mtacurls[i];
		if ( pMtacurl )
		{
			if(!pMtacurl->IsAwaitingDestruction())
			{
				pMtacurl->DoPulse( );
				++ i;
			}
			else
			{
				mtacurls.erase( mtacurls.begin() + i );
				delete pMtacurl;
			}
		}
		else
		{
			mtacurls.erase(mtacurls.begin() + i );
		}
	}
}