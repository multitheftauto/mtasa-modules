/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42
*
******************************************************/

#ifndef __GDIMGMANAGER_H
#define __GDIMGMANAGER_H

#include <gd.h>
#include "gdfontt.h"
#include "gdfonts.h"
#include "gdfontmb.h"
#include "gdfontl.h"
#include "gdfontg.h"

#include <map>

#include "include/ILuaModuleManager.h"
#include "extra/CLuaArguments.h"

#define IMAGE_PNG				0
#define IMAGE_GIF				1
#define IMAGE_JPEG				2

extern ILuaModuleManager10 *pModuleManager;

struct mtaGdImage
{
	gdImagePtr
		img;
	bool
		antialiase;
	void * 
		resource;
};

class GdImgManager
{
private:
	std::map<void *, mtaGdImage> theImages;

	void * GetResource( lua_State * luaVM );

public:
	GdImgManager( );
	~GdImgManager( );

	gdImagePtr GetImage( void * userData );

	void SetImageAA( void * userData, bool useAA );
	bool GetImageAA( void * userData );

	void * AddImage( lua_State * luaVM, int width, int height, bool trueColor );
	void * AddImage( lua_State * luaVM, int type, std::string rFilePath );

	void RemoveImage( void * userData );
	void ResourceCleanup( void * userData );

};


/* {{{ based on php_find_gd_font
 */
static gdFontPtr find_gd_font(int size)
{
	gdFontPtr font;

	switch (size) {
		case 1:
			 font = gdFontGetTiny();
			 break;
		case 2:
			 font = gdFontGetSmall();
			 break;
		case 3:
			 font = gdFontGetMediumBold();
			 break;
		case 4:
			 font = gdFontGetLarge();
			 break;
		case 5:
			 font = gdFontGetGiant();
			 break;
		default:
			font = gdFontGetTiny();
			break;
	}

	return font;
}
/* }}} */

#endif // __GDIMGMANAGER_H