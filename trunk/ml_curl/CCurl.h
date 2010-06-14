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

class CCurl;

#ifndef __CCURL_H
#define __CCURL_H

#include <stdio.h>
#include "include/curl/curl.h"
#include "ml_system.h"

class CCurl
{				
public:
				CCurl			(lua_State* luaVM);
				~CCurl			(void);
	void*		getUserData		(void) { return m_pUserData; };
	CCurl*		init			(lua_State* luaVM, const char* url);
	char*		escape			(const char* url);
	void		cleanup			(void);
	void		close			(void);

	// internal
	CCurl*		Get				(void* pUserData);
	void		doPulse			(void);

private:
	void*					m_pUserData;
	CURL*					m_pCurl;
	vector < CCurl* > mtacurl;
};

#endif