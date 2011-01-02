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

#include "ml_base.h"

ILuaModuleManager10 *pModuleManager = NULL;
TextureContainer *	mTextureContainer = NULL;
GdImgManager * mImgManager = NULL;

// Initialisation function (module entrypoint)
MTAEXPORT bool InitModule ( ILuaModuleManager10 *pManager, char *szModuleName, char *szAuthor, float *fVersion )
{
    pModuleManager = pManager;
	mTextureContainer = new TextureContainer;
	mImgManager = new GdImgManager;

    // Set the module info
#if WIN32
    strncpy_s ( szModuleName, MAX_INFO_LENGTH, MODULE_NAME, MAX_INFO_LENGTH );
    strncpy_s ( szAuthor, MAX_INFO_LENGTH, MODULE_AUTHOR, MAX_INFO_LENGTH );
#else
    strncpy ( szModuleName, MODULE_NAME, MAX_INFO_LENGTH );
    strncpy ( szAuthor, MODULE_AUTHOR, MAX_INFO_LENGTH );
#endif

    (*fVersion) = (float)MODULE_VERSION;

    return true;
}


MTAEXPORT void RegisterFunctions ( lua_State * luaVM )
{
    if ( pModuleManager && luaVM )
    {
		pModuleManager->RegisterFunction ( luaVM, "createTxdContainer", CFunctions::CreateTxdContainer );

		pModuleManager->RegisterFunction ( luaVM, "txdContainerAddImage", CFunctions::TxdContainerAddImage ); // Automaps!
		//pModuleManager->RegisterFunction ( luaVM, "txdContainerAddImagePng", CFunctions::TxdContainerAddImagePng );
		//pModuleManager->RegisterFunction ( luaVM, "txdContainerAddImageJpeg", CFunctions::TxdContainerAddImageJpeg );
		//pModuleManager->RegisterFunction ( luaVM, "txdContainerAddImageGif", CFunctions::TxdContainerAddImageGif );

		pModuleManager->RegisterFunction ( luaVM, "txdContainerAddImagePtr", CFunctions::TxdContainerAddImagePtr );

		pModuleManager->RegisterFunction ( luaVM, "saveTxdContainer", CFunctions::SaveTxdContainer );

		// GD Functions...
		pModuleManager->RegisterFunction ( luaVM, "gd_info", CFunctions::gd_info );

		pModuleManager->RegisterFunction ( luaVM, "imageCreate", CFunctions::imgCreate );
		pModuleManager->RegisterFunction ( luaVM, "imageCreateTrueColor", CFunctions::imgCreateTrueColor );
		pModuleManager->RegisterFunction ( luaVM, "imageCreateFromPng", CFunctions::imgCreateFromPng );
		pModuleManager->RegisterFunction ( luaVM, "imageCreateFromGif", CFunctions::imgCreateFromGif );
		pModuleManager->RegisterFunction ( luaVM, "imageCreateFromJpeg", CFunctions::imgCreateFromJpeg );

		pModuleManager->RegisterFunction ( luaVM, "imageSX", CFunctions::imgSX );
		pModuleManager->RegisterFunction ( luaVM, "imageSY", CFunctions::imgSY );
		pModuleManager->RegisterFunction ( luaVM, "imageIsTrueColor", CFunctions::imgIsTrueColor );

		pModuleManager->RegisterFunction ( luaVM, "imagePng", CFunctions::imgPng );
		pModuleManager->RegisterFunction ( luaVM, "imageGif", CFunctions::imgGif );
		pModuleManager->RegisterFunction ( luaVM, "imageJpeg", CFunctions::imgJpeg );

		pModuleManager->RegisterFunction ( luaVM, "imageGetAlphaBlending", CFunctions::imgGetAlphaBlending );
		pModuleManager->RegisterFunction ( luaVM, "imageSetAlphaBlending", CFunctions::imgSetAlphaBlending );
		pModuleManager->RegisterFunction ( luaVM, "imageGetAntiAlias", CFunctions::imgGetAntiAlias );
		pModuleManager->RegisterFunction ( luaVM, "imageSetAntiAlias", CFunctions::imgSetAntiAlias );
		pModuleManager->RegisterFunction ( luaVM, "imageGetInterlace", CFunctions::imgGetInterlace );
		pModuleManager->RegisterFunction ( luaVM, "imageSetInterlace", CFunctions::imgSetInterlace );
		pModuleManager->RegisterFunction ( luaVM, "imageGetSaveAlpha", CFunctions::imgGetSaveAlpha );
		pModuleManager->RegisterFunction ( luaVM, "imageSetSaveAlpha", CFunctions::imgSetSaveAlpha );

		pModuleManager->RegisterFunction ( luaVM, "imageColorAllocate", CFunctions::imgColorAllocate );
		pModuleManager->RegisterFunction ( luaVM, "imageColorAllocateAlpha", CFunctions::imgColorAllocateAlpha );
		pModuleManager->RegisterFunction ( luaVM, "imageColorClosest", CFunctions::imgColorClosest );
		pModuleManager->RegisterFunction ( luaVM, "imageColorClosestAlpha", CFunctions::imgColorClosestAlpha );
		pModuleManager->RegisterFunction ( luaVM, "imageColorExact", CFunctions::imgColorExact );
		pModuleManager->RegisterFunction ( luaVM, "imageColorExactAlpha", CFunctions::imgColorExactAlpha );
		pModuleManager->RegisterFunction ( luaVM, "imageColorResolve", CFunctions::imgColorResolve );
		pModuleManager->RegisterFunction ( luaVM, "imageColorResolveAlpha", CFunctions::imgColorResolveAlpha );
		pModuleManager->RegisterFunction ( luaVM, "imageGetColorAt", CFunctions::imgGetColorAt );
		pModuleManager->RegisterFunction ( luaVM, "imageSetColorAt", CFunctions::imgSetColorAt );
		pModuleManager->RegisterFunction ( luaVM, "imageColorDeallocate", CFunctions::imgColorDeallocate );
		pModuleManager->RegisterFunction ( luaVM, "imageColorsTotal", CFunctions::imgColorsTotal );
		pModuleManager->RegisterFunction ( luaVM, "imageColorSpecial", CFunctions::imgColorSpecial );

		pModuleManager->RegisterFunction ( luaVM, "imageFontHeight", CFunctions::imgFontHeight );
		pModuleManager->RegisterFunction ( luaVM, "imageFontWidth", CFunctions::imgFontWidth );
		pModuleManager->RegisterFunction ( luaVM, "imageChar", CFunctions::imgChar );
		pModuleManager->RegisterFunction ( luaVM, "imageCharUp", CFunctions::imgCharUp );
		pModuleManager->RegisterFunction ( luaVM, "imageString", CFunctions::imgString );
		pModuleManager->RegisterFunction ( luaVM, "imageStringUp", CFunctions::imgStringUp );
		pModuleManager->RegisterFunction ( luaVM, "imageTtfText", CFunctions::imgTtfText );
		pModuleManager->RegisterFunction ( luaVM, "imageTtfBBox", CFunctions::imgTtfBBox );

		pModuleManager->RegisterFunction ( luaVM, "imageGetStyle", CFunctions::imgGetStyle );
		pModuleManager->RegisterFunction ( luaVM, "imageSetStyle", CFunctions::imgSetStyle );
		pModuleManager->RegisterFunction ( luaVM, "imageGetThickness", CFunctions::imgGetThickness );
		pModuleManager->RegisterFunction ( luaVM, "imageSetThickness", CFunctions::imgSetThickness );
		pModuleManager->RegisterFunction ( luaVM, "imageGetBrush", CFunctions::imgGetBrush );
		pModuleManager->RegisterFunction ( luaVM, "imageSetBrush", CFunctions::imgSetBrush );
		pModuleManager->RegisterFunction ( luaVM, "imageGetTile", CFunctions::imgGetTile );
		pModuleManager->RegisterFunction ( luaVM, "imageSetTile", CFunctions::imgSetTile );

		pModuleManager->RegisterFunction ( luaVM, "imageLine", CFunctions::imgLine );
		pModuleManager->RegisterFunction ( luaVM, "imageArc", CFunctions::imgArc );
		pModuleManager->RegisterFunction ( luaVM, "imageRectangle", CFunctions::imgRectangle );
		pModuleManager->RegisterFunction ( luaVM, "imageEllipse", CFunctions::imgEllipse );

		pModuleManager->RegisterFunction ( luaVM, "imageFill", CFunctions::imgFill );
		pModuleManager->RegisterFunction ( luaVM, "imageFillToBorder", CFunctions::imgFillToBorder );
		pModuleManager->RegisterFunction ( luaVM, "imageFilledArc", CFunctions::imgFilledArc );
		pModuleManager->RegisterFunction ( luaVM, "imageFilledRectangle", CFunctions::imgFilledRectangle );
		pModuleManager->RegisterFunction ( luaVM, "imageFilledEllipse", CFunctions::imgFilledEllipse );
		
		pModuleManager->RegisterFunction ( luaVM, "imageDestroy", CFunctions::imgDestroy );
		pModuleManager->RegisterFunction ( luaVM, "imageCleanup", CFunctions::imgCleanup );
		
    }
}


MTAEXPORT bool DoPulse ( void )
{
    return true;
}

MTAEXPORT bool ShutdownModule ( void )
{
    delete mTextureContainer;
    return true;
}
