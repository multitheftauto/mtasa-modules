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

#include "CFunctions.h"
#include "extra/CLuaArguments.h"

int CFunctions::CreateTxdContainer ( lua_State* luaVM )
{
	// bool/int createTxdContainer( void )
    if ( luaVM )
    {
		int i = mTextureContainer->AddContainer();
        if ( i != 0 )
		{
			
			lua_pushnumber( luaVM, i );
			return 1;
		}
		
		lua_pushboolean ( luaVM, false );
        return 1;
    }
    return 0;
}

int CFunctions::TxdContainerAddImage ( lua_State* luaVM )
{
	// bool txdContainerAddImage( int container, string name, string filePath[, bool compress, string alphaname] )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 2 ) == LUA_TSTRING &&
            lua_type ( luaVM, 3 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				int contId = (int)lua_tonumber(luaVM, 1);
				const char* tName = lua_tostring(luaVM, 2);
				const char* fName = lua_tostring(luaVM, 3);
				bool compress = true; //lua_type ( luaVM, 4 ) == LUA_TBOOLEAN?true:false;

				const char* alphaName;
				bool hasAlpha = false;
				if( lua_type ( luaVM, 5 ) == LUA_TSTRING )
				{
					alphaName = lua_tostring(luaVM, 5);
					hasAlpha = true;
				}

				if( mTextureContainer->HasMember( contId ) )
				{
					std::string 
						fNewPath,
						fMetaPath;

					if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
					{
						uint32_t format = compress?(hasAlpha?FORMAT_1555:FORMAT_565):(hasAlpha?FORMAT_8888:FORMAT_888);

						TextureImage img(fNewPath);
						img.LoadImageToData(format, hasAlpha);

						if( img.errorCode != 0 )
						{
							pModuleManager->DebugPrintf( luaVM, "txdContainerAddImage: Image Error(%d).", img.errorCode );
							lua_pushboolean( luaVM, false );
						}
						else
						{
							if(compress) img.Compress();

							txd_texture_data_t tex;

							tex.id = 0x01;
							tex.marker = VERSION_GTASA;
							tex.version = 0x09;
							tex.filter_flags = 0x1101;
							
							memset(tex.texture_name, 0x0, 32 ); 
							memset(tex.alpha_name, 0x0, 32 ); 

							strcpy(tex.texture_name, tName);
							if(hasAlpha) strcpy(tex.alpha_name, alphaName);

							tex.alpha_flags = format;
							tex.direct3d_texture_format = compress?0x31545844:0x0; // 0x31545844 = DXT1
							tex.width = (uint16_t)img.width;
							tex.height = (uint16_t)img.height;
							tex.depth = 0x10;
							tex.mipmap_count = 1;
							tex.texcode_type = 0x04;
							tex.flags = compress?(hasAlpha?9:8):(hasAlpha?1:0);

							tex.palette = NULL;

							tex.data_size = img.fileData.size();
							tex.data = new uint8_t[img.fileData.size()];
							for(int i = 0; i < (int)img.fileData.size(); i++)
							{
								tex.data[i] = (uint8_t)img.fileData[i];
							}

							tex.chunk_size = 0;

							tex.chunk_size += ( 5 * sizeof(uint32_t) );
							tex.chunk_size += 64;
							tex.chunk_size += ( 2 * sizeof(uint16_t) );
							tex.chunk_size += ( 4 * sizeof(uint8_t) );
							tex.chunk_size += tex.depth==8?256*4:0;
							tex.chunk_size += tex.data_size;
							//tex.chunk_size += tex.mipmap_count;

							txd_extra_info_t extInf;
							extInf.id = CHUNK_EXTRA;
							extInf.marker = VERSION_GTASA;
							extInf.chunk_size = 0;
							extInf.data = 0;

							mTextureContainer->mContainers[contId]->addTexture( tex, extInf );
							lua_pushboolean( luaVM, true );
						}
						
						return 1;
					}
					else pModuleManager->DebugPrintf( luaVM, "Bad filepath in: txdContainerAddImage" );
				}
				else pModuleManager->DebugPrintf( luaVM, "Bad container id in: txdContainerAddImage" );
			}
			lua_pushboolean( luaVM, false );
		}
		else if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 2 ) == LUA_TSTRING &&
            lua_type ( luaVM, 3 ) == LUA_TUSERDATA )
		{
			return TxdContainerAddImagePtr(luaVM);
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: txdContainerAddImage" );
			lua_pushboolean( luaVM, false );
		}
        return 1;
    }
    return 0;
}

int CFunctions::TxdContainerAddImagePtr ( lua_State* luaVM )
{
	// bool txdContainerAddImagePtr( int container, string name, userdata im[, bool compress, string alphaname] )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 2 ) == LUA_TSTRING &&
            lua_type ( luaVM, 3 ) == LUA_TUSERDATA )
		{
			int contId = (int)lua_tonumber(luaVM, 1);
			const char* tName = lua_tostring(luaVM, 2);
			bool compress = true; //lua_type ( luaVM, 4 ) == LUA_TBOOLEAN?true:false;

			const char* alphaName;
			bool hasAlpha = false;
			if( lua_type ( luaVM, 5 ) == LUA_TSTRING )
			{
				alphaName = lua_tostring(luaVM, 5);
				hasAlpha = true;
			}

			if( mTextureContainer->HasMember( contId ) )
			{
				gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
				if( im != NULL )
				{
					TextureImage img(im);
					TextureData tData(tName, alphaName, hasAlpha, compress, &img);

					TextureExtra extra;

					mTextureContainer->mContainers[contId]->addTexture( tData.m_Data, extra.m_Data );
					lua_pushboolean( luaVM, true );
				}
				else pModuleManager->DebugPrintf( luaVM, "Bad image in: txdContainerAddImagePtr" );
			}
			else pModuleManager->DebugPrintf( luaVM, "Bad container id in: txdContainerAddImagePtr" );
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: txdContainerAddImagePtr" );
			lua_pushboolean( luaVM, false );
		}
        return 1;
	}
    return 0;
}

int CFunctions::SaveTxdContainer ( lua_State* luaVM )
{
	// bool saveTxdContainer( int container, string fName )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 2 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				int contId = (int)lua_tonumber(luaVM, 1);
				const char* fName = lua_tostring(luaVM, 2);
				
				if( mTextureContainer->HasMember( contId ) )
				{
					std::string 
						fNewPath,
						fMetaPath;

					if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
					{
						pModuleManager->DebugPrintf( luaVM, "saveTxdContainer: %s", fNewPath.c_str() );
						lua_pushboolean( luaVM, mTextureContainer->mContainers[contId]->Write(fNewPath.c_str()) );
						return 1;
					}
					else pModuleManager->DebugPrintf( luaVM, "Bad filepath in: saveTxdContainer" );
				}
				else pModuleManager->DebugPrintf( luaVM, "Bad container id in: saveTxdContainer" );
			}
			lua_pushboolean( luaVM, false );
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: saveTxdContainer" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::gd_info ( lua_State* luaVM )
{
	// table gd_info( )
    if ( luaVM )
    {
		// Table
		// GD Version = string GD_VERSION_STRING

		// Create a table...
		lua_createtable( luaVM, 0, 1 );
		
		// GD Version
		lua_pushstring( luaVM, "GD Version" );
		lua_pushstring( luaVM, GD_VERSION_STRING);
		lua_settable( luaVM, -3 );

		return 1;
    }
    return 0;
}

int CFunctions::imgCreate ( lua_State* luaVM )
{
	// bool/userdata imageCreate( int w, int h )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER )
		{
			void * ret = mImgManager->AddImage( luaVM, (int)lua_tonumber(luaVM, 1), (int)lua_tonumber(luaVM, 2), false );
			if( ret != NULL )
			{
				lua_pushlightuserdata( luaVM, ret );
				return 1;
			}
			lua_pushboolean( luaVM, false );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageCreate" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgCreateTrueColor ( lua_State* luaVM )
{
	// bool/userdata imageCreateTrueColor( int w, int h )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER )
		{
			void * ret = mImgManager->AddImage( luaVM, (int)lua_tonumber(luaVM, 1), (int)lua_tonumber(luaVM, 2), true );
			if( ret != NULL )
			{
				lua_pushlightuserdata( luaVM, ret );
				return 1;
			}
			lua_pushboolean( luaVM, false );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageCreateTrueColor" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgCreateFromPng ( lua_State* luaVM )
{
	// bool/userdata imageCreateFromPng( string File )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				const char* fName = lua_tostring(luaVM, 1);
				std::string 
					fNewPath,
					fMetaPath;

				if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
				{
					void * ret = mImgManager->AddImage( luaVM, IMAGE_PNG, fNewPath );
					if( ret != NULL )
					{
						lua_pushlightuserdata( luaVM, ret );
						return 1;
					}
				}
			}
			lua_pushboolean( luaVM, false );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageCreateFromPng" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgCreateFromGif ( lua_State* luaVM )
{
	// bool/userdata imageCreateFromGif( string File )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				const char* fName = lua_tostring(luaVM, 1);
				std::string 
					fNewPath,
					fMetaPath;

				if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
				{
					void * ret = mImgManager->AddImage( luaVM, IMAGE_GIF, fNewPath );
					if( ret != NULL )
					{
						lua_pushlightuserdata( luaVM, ret );
						return 1;
					}
				}
			}
			lua_pushboolean( luaVM, false );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageCreateFromGif" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgCreateFromJpeg ( lua_State* luaVM )
{
	// bool/userdata imageCreateFromJpeg( string File )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				const char* fName = lua_tostring(luaVM, 1);
				std::string 
					fNewPath,
					fMetaPath;

				if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
				{
					void * ret = mImgManager->AddImage( luaVM, IMAGE_JPEG, fNewPath );
					if( ret != NULL )
					{
						lua_pushlightuserdata( luaVM, ret );
						return 1;
					}
				}
			}
			lua_pushboolean( luaVM, false );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageCreateFromJpeg" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgSX ( lua_State* luaVM )
{
	// bool/int imageSX( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushnumber( luaVM, gdImageSX(im) );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSX" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgSY ( lua_State* luaVM )
{
	// bool/int imageSY( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushnumber( luaVM, gdImageSY(im) );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSY" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgIsTrueColor ( lua_State* luaVM )
{
	// bool imageIsTrueColor( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushboolean( luaVM, im->trueColor==1?true:false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageIsTrueColor" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgPng ( lua_State* luaVM )
{
	// bool imagePng( userdata im, string fPath[, int level = 0, int delete = false] )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
				if( im != NULL )
				{
					const char* fName = lua_tostring(luaVM, 2);
					std::string 
						fNewPath,
						fMetaPath;

					int level = (lua_type ( luaVM, 3 ) == LUA_TNUMBER) ? (int)lua_tonumber(luaVM, 3) : 0;
					bool dele = lua_toboolean(luaVM, 4)?true:false;

					if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
					{
						std::fstream out(fNewPath.c_str(), std::fstream::out | std::fstream::binary );
						if( out.is_open() )
						{
							int sis = 0;
							char * result = (char *)gdImagePngPtrEx( im, &sis, level );

							out.write(result, sis);
							
							out.close();
							gdFree(result);

							if(dele) mImgManager->RemoveImage( lua_touserdata( luaVM, 1 ) );

							lua_pushboolean( luaVM, true );
							return 1;
						}
						else
						{
							lua_pushboolean( luaVM, false );
							return 1;
						}
					}
					else
					{
						pModuleManager->DebugPrintf( luaVM, "Bad filepath in: imagePng" );
						lua_pushboolean( luaVM, false );
						return 1;
					}
				}
				else
				{
					lua_pushboolean( luaVM, false );
					return 1;
				}
			}
			lua_pushboolean( luaVM, false );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imagePng" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgGif ( lua_State* luaVM )
{
	// bool imageGif( userdata im, string fPath[, bool delete = false] )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
				if( im != NULL )
				{
					const char* fName = lua_tostring(luaVM, 2);
					std::string 
						fNewPath,
						fMetaPath;

					bool dele = lua_toboolean(luaVM, 3)?true:false;

					if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
					{
						std::fstream out(fNewPath.c_str(), std::fstream::out | std::fstream::binary );
						if( out.is_open() )
						{
							int sis = 0;
							char * result = (char *)gdImageGifPtr( im, &sis );

							out.write(result, sis);
							
							out.close();
							gdFree(result);

							if(dele) mImgManager->RemoveImage( lua_touserdata( luaVM, 1 ) );

							lua_pushboolean( luaVM, true );
							return 1;
						}
						else
						{
							lua_pushboolean( luaVM, false );
							return 1;
						}
					}
					else
					{
						pModuleManager->DebugPrintf( luaVM, "Bad filepath in: imageGif" );
						lua_pushboolean( luaVM, false );
						return 1;
					}
				}
				else
				{
					lua_pushboolean( luaVM, false );
					return 1;
				}
			}
			lua_pushboolean( luaVM, false );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGif" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgJpeg ( lua_State* luaVM )
{
	// bool imageJpeg( userdata im, string fPath[, float quality = 1, bool delete = false] )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
				if( im != NULL )
				{
					const char* fName = lua_tostring(luaVM, 2);
					std::string 
						fNewPath,
						fMetaPath;

					double quality = (lua_type ( luaVM, 3 ) == LUA_TNUMBER) ? (double)lua_tonumber(luaVM, 3) : 1;
					if(quality > 1) quality = 1.0;
					else if(quality < 0) quality = 0.0;

					int level = (int)floor(quality * 95.0);

					bool dele = lua_toboolean(luaVM, 4)?true:false;

					if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
					{
						std::fstream out(fNewPath.c_str(), std::fstream::out | std::fstream::binary );
						if( out.is_open() )
						{
							int sis = 0;
							char * result = (char *)gdImageJpegPtr( im, &sis, level );

							out.write(result, sis);
							
							out.close();
							gdFree(result);

							if(dele) mImgManager->RemoveImage( lua_touserdata( luaVM, 1 ) );

							lua_pushboolean( luaVM, true );
							return 1;
						}
						else
						{
							lua_pushboolean( luaVM, false );
							return 1;
						}
					}
					else
					{
						pModuleManager->DebugPrintf( luaVM, "Bad filepath in: imageJpeg" );
						lua_pushboolean( luaVM, false );
						return 1;
					}
				}
				else
				{
					lua_pushboolean( luaVM, false );
					return 1;
				}
			}
			lua_pushboolean( luaVM, false );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageJpeg" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgGetAlphaBlending ( lua_State* luaVM )
{
	// bool imageGetAlphaBlending( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushboolean( luaVM, im->alphaBlendingFlag==1?true:false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetAlphaBlending" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgSetAlphaBlending ( lua_State* luaVM )
{
	// bool imageSetAlphaBlending( userdata im, bool blendmode )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TBOOLEAN )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int blend = lua_toboolean(luaVM, 2)?1:0;
				gdImageAlphaBlending( im, blend );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetAlphaBlending" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgGetAntiAlias ( lua_State* luaVM )
{
	// bool imageGetAntiAlias( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			lua_pushboolean( luaVM, mImgManager->GetImageAA( lua_touserdata( luaVM, 1 ) ) );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetAntiAlias" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgSetAntiAlias ( lua_State* luaVM )
{
	// bool imageSetAntiAlias( userdata im, bool enabled )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TBOOLEAN )
		{
			bool useAA = lua_toboolean(luaVM, 2)?true:false;
			mImgManager->SetImageAA( lua_touserdata( luaVM, 1 ), useAA );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetAntiAlias" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgGetInterlace ( lua_State* luaVM )
{
	// bool imageGetInterlace( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushboolean( luaVM, gdImageGetInterlaced(im)==1?true:false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetInterlace" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgSetInterlace ( lua_State* luaVM )
{
	// bool imageSetInterlace( userdata im, bool interlace );
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TBOOLEAN )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int interlace = lua_toboolean(luaVM, 2)?1:0;
				gdImageInterlace(im, interlace);
				
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetInterlace" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgGetSaveAlpha ( lua_State* luaVM )
{
	// bool imageGetSaveAlpha( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushboolean( luaVM, im->saveAlphaFlag==1?true:false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetSaveAlpha" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgSetSaveAlpha ( lua_State* luaVM )
{
	// bool imageSetSaveAlpha( userdata im, bool savealpha );
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TBOOLEAN )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int savealpha = lua_toboolean(luaVM, 2)?1:0;
				gdImageSaveAlpha(im, savealpha);
				
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetSaveAlpha" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorAllocate ( lua_State* luaVM )
{
	// bool/int imageColorAllocate( userdata im, int red, int green, int blue)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int ret = gdImageColorAllocate( im, (int)lua_tonumber(luaVM, 2), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4) );
				if( ret != -1 ) lua_pushnumber( luaVM, ret );
				else            lua_pushboolean( luaVM, false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorAllocate" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorAllocateAlpha ( lua_State* luaVM )
{
	// bool/int imageColorAllocateAlpha( userdata im, int red, int green, int blue, int alpha)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER  &&
            lua_type ( luaVM, 5 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushnumber( luaVM, gdImageColorAllocateAlpha( im, (int)lua_tonumber(luaVM, 2), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4), (int)lua_tonumber(luaVM, 5) ) );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorAllocateAlpha" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorClosest ( lua_State* luaVM )
{
	// bool/int imageColorClosest( userdata im, int red, int green, int blue)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int ret = gdImageColorClosest( im, (int)lua_tonumber(luaVM, 2), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4) );
				if( ret != -1 ) lua_pushnumber( luaVM, ret );
				else            lua_pushboolean( luaVM, false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorClosest" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorClosestAlpha ( lua_State* luaVM )
{
	// bool/int imageColorClosestAlpha( userdata im, int red, int green, int blue, int alpha)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER  &&
            lua_type ( luaVM, 5 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushnumber( luaVM, gdImageColorClosestAlpha( im, (int)lua_tonumber(luaVM, 2), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4), (int)lua_tonumber(luaVM, 5) ) );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorClosestAlpha" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorExact ( lua_State* luaVM )
{
	// bool/int imageColorExact( userdata im, int red, int green, int blue)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int ret = gdImageColorExact( im, (int)lua_tonumber(luaVM, 2), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4) );
				if( ret != -1 ) lua_pushnumber( luaVM, ret );
				else            lua_pushboolean( luaVM, false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorExact" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorExactAlpha ( lua_State* luaVM )
{
	// bool/int imageColorExactAlpha( userdata im, int red, int green, int blue, int alpha)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER  &&
            lua_type ( luaVM, 5 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushnumber( luaVM, gdImageColorExactAlpha( im, (int)lua_tonumber(luaVM, 2), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4), (int)lua_tonumber(luaVM, 5) ) );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorExactAlpha" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorResolve ( lua_State* luaVM )
{
	// bool/int imageColorResolve( userdata im, int red, int green, int blue)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int ret = gdImageColorResolve( im, (int)lua_tonumber(luaVM, 2), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4) );
				if( ret != -1 ) lua_pushnumber( luaVM, ret );
				else            lua_pushboolean( luaVM, false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorResolve" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorResolveAlpha ( lua_State* luaVM )
{
	// bool/int imageColorResolveAlpha( userdata im, int red, int green, int blue, int alpha)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER  &&
            lua_type ( luaVM, 5 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushnumber( luaVM, gdImageColorResolveAlpha( im, (int)lua_tonumber(luaVM, 2), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4), (int)lua_tonumber(luaVM, 5) ) );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorResolveAlpha" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgGetColorAt ( lua_State* luaVM )
{
	// bool/int imageGetColorAt( userdata im, int x, int y)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int 
					x = (int)lua_tonumber(luaVM, 2),
					y = (int)lua_tonumber(luaVM, 3);
				if (gdImageBoundsSafe(im, x, y))
				{
					lua_pushnumber( luaVM, gdImageGetPixel(im, x, y) );
				}
				else
				{
					pModuleManager->DebugPrintf( luaVM, "imageGetColorAt: Pixel is out of bounds." );
					lua_pushboolean( luaVM, false );
				}
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetColorAt" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgSetColorAt ( lua_State* luaVM )
{
	// bool imageSetColorAt( userdata im, int x, int y, int color)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int 
					x = (int)lua_tonumber(luaVM, 2),
					y = (int)lua_tonumber(luaVM, 3),
					color = (int)lua_tonumber(luaVM, 4);
				if (gdImageBoundsSafe(im, x, y))
				{
					gdImageSetPixel(im, x, y, color);
					lua_pushboolean( luaVM, true );
				}
				else
				{
					pModuleManager->DebugPrintf( luaVM, "imageSetColorAt: Pixel is out of bounds." );
					lua_pushboolean( luaVM, false );
				}
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetColorAt" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorDeallocate ( lua_State* luaVM )
{
	// bool imageColorDeallocate( userdata im, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				gdImageColorDeallocate(im, (int)lua_tonumber(luaVM, 2) );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorDeallocate" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorsTotal ( lua_State* luaVM )
{
	// bool/int imageColorsTotal( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushnumber( luaVM, gdImageColorsTotal(im) );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorsTotal" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgColorSpecial ( lua_State* luaVM )
{
	// bool/int imageColorSpecial( string name )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TSTRING )
		{
			const char* txt = lua_tostring(luaVM, 1);
			if(strcmp(txt, "styled") == 0)
			{
				lua_pushnumber( luaVM, gdStyled );
			}
			else if(strcmp(txt, "brushed") == 0)
			{
				lua_pushnumber( luaVM, gdBrushed );
			}
			else if(strcmp(txt, "styledbrushed") == 0)
			{
				lua_pushnumber( luaVM, gdStyledBrushed );
			}
			else if(strcmp(txt, "tiled") == 0)
			{
				lua_pushnumber( luaVM, gdTiled );
			}
			else if(strcmp(txt, "transparent") == 0)
			{
				lua_pushnumber( luaVM, gdTransparent );
			}
			else if(strcmp(txt, "antialiased") == 0)
			{
				lua_pushnumber( luaVM, gdAntiAliased );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageColorSpecial" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgFontHeight ( lua_State* luaVM )
{
	// bool/int imageFontHeight( int font )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER )
		{
			lua_pushnumber( luaVM, find_gd_font((int)lua_tonumber(luaVM, 2))->h );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageFontHeight" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgFontWidth ( lua_State* luaVM )
{
	// bool/int imageFontWidth( int font )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER )
		{
			lua_pushnumber( luaVM, find_gd_font((int)lua_tonumber(luaVM, 2))->w );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageFontWidth" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgChar ( lua_State* luaVM )
{
	// bool imageChar( userdata im, int font, int x, int y, string char, int color)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 5 ) == LUA_TSTRING &&
            lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			const char* txt = lua_tostring(luaVM, 5);
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );

			if( im != NULL && txt != NULL )
			{
				char chr = txt[0];

				gdImageChar(im, find_gd_font( (int)lua_tonumber(luaVM, 2)), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4), chr, (int)lua_tonumber(luaVM, 6));
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageChar" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgCharUp ( lua_State* luaVM )
{
	// bool imageCharUp( userdata im, int font, int x, int y, string char, int color)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 5 ) == LUA_TSTRING &&
            lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			const char* txt = lua_tostring(luaVM, 5);
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );

			if( im != NULL && txt != NULL )
			{
				char chr = txt[0];

				gdImageCharUp(im, find_gd_font( (int)lua_tonumber(luaVM, 2)), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4), chr, (int)lua_tonumber(luaVM, 6));
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageCharUp" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgString ( lua_State* luaVM )
{
	// bool imageString( userdata im, int font, int x, int y, string char, int color)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 5 ) == LUA_TSTRING &&
            lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			const char* txt = lua_tostring(luaVM, 5);
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );

			if( im != NULL && txt != NULL )
			{
				gdImageString(im, find_gd_font( (int)lua_tonumber(luaVM, 2)), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4), (unsigned char*)txt, (int)lua_tonumber(luaVM, 6));
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageString" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgStringUp ( lua_State* luaVM )
{
	// bool imageStringUp( userdata im, int font, int x, int y, string char, int color)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 5 ) == LUA_TSTRING &&
            lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			const char* txt = lua_tostring(luaVM, 5);
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );

			if( im != NULL && txt != NULL )
			{
				gdImageStringUp(im, find_gd_font( (int)lua_tonumber(luaVM, 2)), (int)lua_tonumber(luaVM, 3), (int)lua_tonumber(luaVM, 4), (unsigned char*)txt, (int)lua_tonumber(luaVM, 6));
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageStringUp" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgTtfText ( lua_State* luaVM )
{
	// bool/int imageTtfText( userdata im, int size, int angle, int x, int y, int color, string fontFile, string text)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 4 ) == LUA_TNUMBER  &&
            lua_type ( luaVM, 5 ) == LUA_TNUMBER  &&
            lua_type ( luaVM, 6 ) == LUA_TNUMBER  &&
            lua_type ( luaVM, 7 ) == LUA_TSTRING  &&
            lua_type ( luaVM, 8 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				const char* fName = lua_tostring(luaVM, 7);
				std::string 
					fNewPath,
					fMetaPath;

				if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
				{
					gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
					if( im != NULL )
					{
						double size = lua_tonumber(luaVM, 2);
						double angle = lua_tonumber(luaVM, 3);
						int x = (int)lua_tonumber(luaVM, 4);
						int y = (int)lua_tonumber(luaVM, 5);
						int color = (int)lua_tonumber(luaVM, 6);
						const char* strTxt = lua_tostring(luaVM, 8);
						
						int rect[8] = {0, 0, 0, 0, 0, 0, 0, 0};
						char * err = gdImageStringFT( im, &rect[0], color, (char *)fNewPath.c_str(), size, angle, x, y, (char *)strTxt );

						if (err)
						{
							pModuleManager->DebugPrintf( luaVM, "Error with gdImageStringFT(%s) %s in: imageTtfText", err );
							return 1;
						}

						lua_createtable( luaVM, 0, 8 );
						
						for( int i = 0; i < 8; i++ )
						{
							lua_pushnumber( luaVM, i+1 );
							lua_pushnumber( luaVM, rect[i]);
							lua_settable( luaVM, -3 );
						}
						return 1;
					}
					else
					{
						lua_pushboolean( luaVM, false );
						return 1;
					}
				}
				else
				{
					pModuleManager->DebugPrintf( luaVM, "Bad filepath in: imageTtfText" );
					lua_pushboolean( luaVM, false );
					return 1;
				}
			}
			else
			{
				lua_pushboolean( luaVM, false );
				return 1;
			}
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageTtfText" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgTtfBBox ( lua_State* luaVM )
{
	// bool/table imageTtfBBox( int size, int angle, string fontFile, string text)
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
            lua_type ( luaVM, 3 ) == LUA_TSTRING  &&
            lua_type ( luaVM, 4 ) == LUA_TSTRING )
		{
			std::string mResource;
			if( pModuleManager->GetResourceName(luaVM, mResource) )
			{
				const char* fName = lua_tostring(luaVM, 3);
				std::string 
					fNewPath,
					fMetaPath;

				if( ParseResourcePathInput( std::string(fName), mResource, fNewPath, fMetaPath ) )
				{
					double size = lua_tonumber(luaVM, 1);
					double angle = lua_tonumber(luaVM, 2);
					const char* strTxt = lua_tostring(luaVM, 4);
					
					int rect[8] = {0, 0, 0, 0, 0, 0, 0, 0};
					char * err = gdImageStringFT( NULL, &rect[0], 0, (char *)fNewPath.c_str(), size, angle, 0, 0, (char *)strTxt );

					if (err)
					{
						pModuleManager->DebugPrintf( luaVM, "Error with gdImageStringFT(%s) in: imageTtfBBox", err );
						return 1;
					}

					lua_createtable( luaVM, 0, 8 );
					
					for( int i = 0; i < 8; i++ )
					{
						lua_pushnumber( luaVM, i+1 );
						lua_pushnumber( luaVM, rect[i]);
						lua_settable( luaVM, -3 );
					}
					return 1;
				}
				else
				{
					pModuleManager->DebugPrintf( luaVM, "Bad filepath in: imageTtfBBox" );
					lua_pushboolean( luaVM, false );
					return 1;
				}
			}
			else
			{
				lua_pushboolean( luaVM, false );
				return 1;
			}
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageTtfBBox" );
			lua_pushboolean( luaVM, false );
		}
		return 1;
    }
    return 0;
}

int CFunctions::imgGetStyle ( lua_State* luaVM )
{
	// bool/table imageGetStyle( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				if(im->style)
				{
					lua_createtable( luaVM, 0, im->styleLength );
					for(int i = 0; i < im->styleLength; i++)
					{
						lua_pushnumber( luaVM, i );
						lua_pushnumber( luaVM, im->style[i] );
						lua_settable( luaVM, -3 );
					}
				}
				else lua_pushboolean( luaVM, false );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetStyle" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgSetStyle ( lua_State* luaVM )
{
	// bool imageSetStyle( userdata im, table style )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TTABLE )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				std::vector<int> style;

				lua_pushnil( luaVM );
				while( lua_next( luaVM, 2 ) != 0 )
				{
					// key = lua_type( luaVM, -2 )
					// val = lua_type( luaVM, -1 )
					if( lua_type( luaVM, -1 ) == LUA_TNUMBER )
					{
						style.push_back((int)lua_tonumber( luaVM, -1 ));
					}
					lua_pop( luaVM, 1 );
				}

				int * arr = new int[style.size()];
#ifdef WIN32
				std::copy(style.begin(), style.end(), stdext::checked_array_iterator<int *>(arr, style.size()));
#else
				std::copy(style.begin(), style.end(), arr);
#endif

				gdImageSetStyle(im, arr, sizeof(arr));
				lua_pushboolean( luaVM, true );

				delete [] arr;
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetStyle" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgGetThickness ( lua_State* luaVM )
{
	// bool/int imageGetThickness( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				lua_pushnumber( luaVM, im->thick );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetThickness" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgSetThickness ( lua_State* luaVM )
{
	// bool imageSetThickness( userdata im, int thickness )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				gdImageSetThickness(im, (int)lua_tonumber( luaVM, 2 ));
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetThickness" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgGetBrush ( lua_State* luaVM )
{
	// bool/userdata imageGetBrush( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL && im->brush != NULL )
			{
				lua_pushlightuserdata( luaVM, (void *)im->brush );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetBrush" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgSetBrush ( lua_State* luaVM )
{
	// bool imageSetBrush( userdata im, userdata brush )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			gdImagePtr brush = mImgManager->GetImage( lua_touserdata( luaVM, 2 ) );
			if( im != NULL && brush != NULL )
			{
				gdImageSetBrush(im, brush);
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetBrush" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgGetTile ( lua_State* luaVM )
{
	// bool/userdata imageGetTile( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL && im->tile != NULL )
			{
				lua_pushlightuserdata( luaVM, (void *)im->tile );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageGetTile" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgSetTile ( lua_State* luaVM )
{
	// bool imageSetTile( userdata im, userdata tile )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			gdImagePtr tile = mImgManager->GetImage( lua_touserdata( luaVM, 2 ) );
			if( im != NULL && tile != NULL )
			{
				gdImageSetTile(im, tile);
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageSetTile" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgLine ( lua_State* luaVM )
{
	// bool/int imageLine( userdata im, int x1, int y1, int x2, int y2, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 5 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				gdImageLine( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), (int)lua_tonumber( luaVM, 6 ) );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageLine" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgArc ( lua_State* luaVM )
{
	// bool/int imageArc( userdata im, int cx, int cy, int w, int h, int start, int end, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 5 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 6 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 7 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 8 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				gdImageArc( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), (int)lua_tonumber( luaVM, 6 ), (int)lua_tonumber( luaVM, 7 ), (int)lua_tonumber( luaVM, 8 ) );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageArc" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgRectangle ( lua_State* luaVM )
{
	// bool/int imageRectangle( userdata im, int x1, int y1, int x2, int y2, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 5 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				gdImageRectangle( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), (int)lua_tonumber( luaVM, 6 ) );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageRectangle" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}


int CFunctions::imgEllipse ( lua_State* luaVM )
{
	// bool/int imageEllipse( userdata im, int cx, int cy, int w, int h, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 5 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
#ifdef HAVE_GD_IMAGEELLIPSE
				gdImageEllipse( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), (int)lua_tonumber( luaVM, 6 ) );
#else
				gdImageArc( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), 0, 360, (int)lua_tonumber( luaVM, 6 ) );
#endif
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageEllipse" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgFill ( lua_State* luaVM )
{
	// bool/int imageFill( userdata im, int x, int y, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				gdImageFill( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ) );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageFill" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgFillToBorder ( lua_State* luaVM )
{
	// bool/int imageFillToBorder( userdata im, int x, int y, int border, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 5 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				gdImageFillToBorder( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ) );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageFillToBorder" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgFilledArc ( lua_State* luaVM )
{
	// bool/int imageFilledArc( userdata im, int cx, int cy, int w, int h, int start, int end, int color[, table style] )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 5 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 6 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 7 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 8 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				int style = gdArc;
				if(lua_type ( luaVM, 9 ) == LUA_TTABLE)
				{
					lua_pushnil( luaVM );
					while( lua_next( luaVM, 9 ) != 0 )
					{
						// key = lua_type( luaVM, -2 )
						// val = lua_type( luaVM, -1 )
						if( lua_type( luaVM, -1 ) == LUA_TNUMBER )
						{
							int val = (int)lua_tonumber( luaVM, -1 );
							switch(val)
							{
								case gdArc:
								{
									style |= gdArc;
									break;
								}
								case gdChord:
								{
									style |= gdChord;
									break;
								}
								case gdNoFill:
								{
									style |= gdNoFill;
									break;
								}
								case gdEdged:
								{
									style |= gdEdged;
									break;
								}
							}
						}
						lua_pop( luaVM, 1 );
					}
				}

				gdImageFilledArc( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), (int)lua_tonumber( luaVM, 6 ), (int)lua_tonumber( luaVM, 7 ), (int)lua_tonumber( luaVM, 8 ), style );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageFilledArc" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgFilledRectangle ( lua_State* luaVM )
{
	// bool/int imageFilledRectangle( userdata im, int x1, int y1, int x2, int y2, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 5 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				gdImageFilledRectangle( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), (int)lua_tonumber( luaVM, 6 ) );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageFilledRectangle" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}


int CFunctions::imgFilledEllipse ( lua_State* luaVM )
{
	// bool/int imageFilledEllipse( userdata im, int cx, int cy, int w, int h, int color )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA &&
			lua_type ( luaVM, 2 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 3 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 4 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 5 ) == LUA_TNUMBER &&
			lua_type ( luaVM, 6 ) == LUA_TNUMBER )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
#ifdef HAVE_GD_IMAGEELLIPSE
				gdImageFilledEllipse( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), (int)lua_tonumber( luaVM, 6 ) );
#else
				gdImageFilledArc( im, (int)lua_tonumber( luaVM, 2 ), (int)lua_tonumber( luaVM, 3 ), (int)lua_tonumber( luaVM, 4 ), (int)lua_tonumber( luaVM, 5 ), 0, 360, (int)lua_tonumber( luaVM, 6 ), 0 );
#endif
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageFilledEllipse" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgDestroy ( lua_State* luaVM )
{
	// bool imageDestroy( userdata im )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			gdImagePtr im = mImgManager->GetImage( lua_touserdata( luaVM, 1 ) );
			if( im != NULL )
			{
				mImgManager->RemoveImage( lua_touserdata( luaVM, 1 ) );
				lua_pushboolean( luaVM, true );
			}
			else
			{
				lua_pushboolean( luaVM, false );
			}
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageDestroy" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}

int CFunctions::imgCleanup ( lua_State* luaVM )
{
	// bool imageCleanup( userdata resource )
    if ( luaVM )
    {
        if ( lua_type ( luaVM, 1 ) == LUA_TLIGHTUSERDATA )
		{
			mImgManager->ResourceCleanup( lua_touserdata( luaVM, 1 ) );
			return 1;
		}
		else
		{
			pModuleManager->DebugPrintf( luaVM, "Incorrect parameters in: imageCleanup" );
		}
    }
	lua_pushboolean( luaVM, false );
    return 0;
}