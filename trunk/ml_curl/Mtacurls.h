#ifndef __MTA_CURL_H
#define __MTA_CURL_H

#include <string>
#include "ml_system.h"

class Mtacurl;
class Mtacurls
{
public:
					Mtacurls( void );
					~Mtacurls( void );

	Mtacurl*		Create( lua_State* luaVM, const char* url );
	Mtacurl*		Get( void* pUserData );
	bool			Delete( Mtacurl* pMtacurl );
	
	void			DoPulse( );

private:
	vector < Mtacurl* > mtacurls;
};

#endif