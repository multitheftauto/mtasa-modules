/*********************************************************
*
*  Multi Theft Auto: San Andreas - Deathmatch
*
*  ml_base, External lua add-on module
*
*  Copyright © 2003-2008 MTA.  All Rights Reserved.
*
*  Grand Theft Auto is © 2002-2003 Rockstar North
*
*  THE FOLLOWING SOURCES ARE PART OF THE MULTI THEFT
*  AUTO SOFTWARE DEVELOPMENT KIT AND ARE RELEASED AS
*  OPEN SOURCE FILES. THESE FILES MAY BE USED AS LONG
*  AS THE DEVELOPER AGREES TO THE LICENSE THAT IS
*  PROVIDED WITH THIS PACKAGE.
*
*********************************************************/

class CFunctions;

#ifndef __CFUNCTIONS_H
#define __CFUNCTIONS_H

#include <stdio.h>
#include <math.h>

#include "include/ILuaModuleManager.h"
#include "TextureContainer.h"

#include "TextureImage.h"
#include "GdImgManager.h"
#include "ModuleFilePaths.h"

#include "TextureWriter.h"

#ifdef WIN32
#include <iterator>
#endif

extern ILuaModuleManager10 *pModuleManager;
extern TextureContainer	   *mTextureContainer;
extern GdImgManager * mImgManager;


class CFunctions
{
public:

        static int      CreateTxdContainer          ( lua_State* luaVM );
        static int      TxdContainerAddImage        ( lua_State* luaVM );
        static int      TxdContainerAddImagePtr     ( lua_State* luaVM );
        static int      SaveTxdContainer            ( lua_State* luaVM );

		// GD Functions...
		
		static int      gd_info				        ( lua_State* luaVM );

		static int      imgCreate			        ( lua_State* luaVM );
		static int      imgCreateTrueColor          ( lua_State* luaVM );
		static int      imgCreateFromPng            ( lua_State* luaVM );
		static int      imgCreateFromGif            ( lua_State* luaVM );
		static int      imgCreateFromJpeg           ( lua_State* luaVM );

		static int      imgSX                       ( lua_State* luaVM );
		static int      imgSY                       ( lua_State* luaVM );
		static int      imgIsTrueColor              ( lua_State* luaVM );

		static int      imgPng                      ( lua_State* luaVM );
		static int      imgGif                      ( lua_State* luaVM );
		static int      imgJpeg                     ( lua_State* luaVM );

		static int      imgGetAlphaBlending         ( lua_State* luaVM );
		static int      imgSetAlphaBlending         ( lua_State* luaVM );
		static int      imgGetAntiAlias				( lua_State* luaVM );
		static int      imgSetAntiAlias				( lua_State* luaVM );
		static int      imgGetInterlace				( lua_State* luaVM );
		static int      imgSetInterlace				( lua_State* luaVM );
		static int      imgGetSaveAlpha 			( lua_State* luaVM );
		static int      imgSetSaveAlpha				( lua_State* luaVM );

		static int      imgColorAllocate            ( lua_State* luaVM );
		static int      imgColorAllocateAlpha       ( lua_State* luaVM );
		static int      imgColorClosest             ( lua_State* luaVM );
		static int      imgColorClosestAlpha		( lua_State* luaVM );
		static int      imgColorExact				( lua_State* luaVM );
		static int      imgColorExactAlpha			( lua_State* luaVM );
		static int      imgColorResolve				( lua_State* luaVM );
		static int      imgColorResolveAlpha		( lua_State* luaVM );
		static int      imgGetColorAt				( lua_State* luaVM );
		static int      imgSetColorAt				( lua_State* luaVM );
		static int      imgColorDeallocate			( lua_State* luaVM );
		static int      imgColorsTotal				( lua_State* luaVM );
		static int      imgColorSpecial				( lua_State* luaVM );

		static int      imgFontHeight				( lua_State* luaVM );
		static int      imgFontWidth				( lua_State* luaVM );
		static int      imgChar						( lua_State* luaVM );
		static int      imgCharUp					( lua_State* luaVM );
		static int      imgString					( lua_State* luaVM );
		static int      imgStringUp					( lua_State* luaVM );
		static int      imgTtfText					( lua_State* luaVM );
		static int      imgTtfBBox					( lua_State* luaVM );

		static int      imgGetStyle					( lua_State* luaVM );
		static int      imgSetStyle					( lua_State* luaVM );
		static int      imgGetThickness				( lua_State* luaVM );
		static int      imgSetThickness				( lua_State* luaVM );
		static int      imgGetBrush					( lua_State* luaVM );
		static int      imgSetBrush					( lua_State* luaVM );
		static int      imgGetTile					( lua_State* luaVM );
		static int      imgSetTile					( lua_State* luaVM );

		static int      imgLine						( lua_State* luaVM );
		static int      imgArc						( lua_State* luaVM );
		static int      imgRectangle				( lua_State* luaVM );
		static int      imgEllipse					( lua_State* luaVM );
		
		static int      imgFill						( lua_State* luaVM );
		static int      imgFillToBorder				( lua_State* luaVM );
		static int      imgFilledArc				( lua_State* luaVM );
		static int      imgFilledRectangle			( lua_State* luaVM );
		static int      imgFilledEllipse			( lua_State* luaVM );

		static int      imgDestroy					( lua_State* luaVM );
		static int      imgCleanup					( lua_State* luaVM );
};
#endif
