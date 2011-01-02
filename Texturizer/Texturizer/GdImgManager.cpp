/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42
*
******************************************************/

#include "GdImgManager.h"

GdImgManager::GdImgManager( )
{

}

GdImgManager::~GdImgManager( )
{
	for( std::map<void *, mtaGdImage >::iterator i = theImages.begin(); i != theImages.end(); i++)
	{
		gdImageDestroy(&(*i->second.img));
	}
	theImages.clear();
}

void * GdImgManager::GetResource( lua_State * luaVM )
{
	std::string mResource;
	if(pModuleManager->GetResourceName(luaVM, mResource))
	{
		lua_State * pResource = pModuleManager->GetResourceFromName( mResource.c_str() );
		return (void *)pResource;
	}
	return NULL;
}

void * GdImgManager::AddImage( lua_State * luaVM, int width, int height, bool trueColor = true )
{
	void * ptr = lua_newuserdata(luaVM, 128);
	if( ptr != NULL )
	{
		gdImagePtr imgPtr = trueColor?gdImageCreateTrueColor( width, height ):gdImageCreate( width, height );
		if(imgPtr != NULL)
		{
			mtaGdImage img;
			img.antialiase = false;
			img.img = imgPtr;
			img.resource = GetResource(luaVM);
			theImages[ptr] = img;
			return ptr;
		}
	}
	return NULL;
}

void * GdImgManager::AddImage( lua_State * luaVM, int type, std::string rFilePath )
{
	void * ptr = lua_newuserdata(luaVM, 128);
	if( ptr != NULL )
	{
#if WIN32
		FILE * in = NULL;
		fopen_s(&in, rFilePath.c_str(), "rb");
#else
		FILE * in = fopen(rFilePath.c_str(), "rb");
#endif
		if( in != NULL )
		{
			gdImagePtr imgPtr = NULL;
			
			switch(type)
			{
				case IMAGE_PNG:
				{
					imgPtr = gdImageCreateFromPng( in );
					break;
				}
				case IMAGE_JPEG:
				{
					imgPtr = gdImageCreateFromJpeg( in );
					break;
				}
				case IMAGE_GIF:
				{
					imgPtr = gdImageCreateFromGif( in );
					break;
				}
			}
			
			fclose(in);

			if(imgPtr != NULL)
			{
				mtaGdImage img;
				img.antialiase = false;
				img.img = imgPtr;
				img.resource = GetResource(luaVM);
				theImages[ptr] = img;
				return ptr;
			}
		}
	}
	return NULL;
}

void GdImgManager::RemoveImage( void * userData )
{
	gdImagePtr im = GetImage(userData);
	if(im != NULL)
	{
		gdImageDestroy(im);
		theImages.erase( theImages.find( userData ) );
	}
}

void GdImgManager::ResourceCleanup( void * userData )
{
	std::vector<void *> remImages;
	for(std::map< void *, mtaGdImage >::iterator i = theImages.begin(); i != theImages.end(); ++i)
	{
		if(i->second.resource == userData)
		{
			remImages.push_back(i->first);
		}
	}
	for(std::vector<void *>::iterator i = remImages.begin(); i != remImages.end(); ++i)
	{
		RemoveImage( *i );
	}
}

gdImagePtr GdImgManager::GetImage( void * userData )
{
	std::map< void *, mtaGdImage >::iterator idx = theImages.find( userData );
	return (idx != theImages.end())?(idx->second.img):NULL;
}

void GdImgManager::SetImageAA( void * userData, bool useAA )
{
	std::map< void *, mtaGdImage >::iterator idx = theImages.find( userData );
	if(idx != theImages.end())
	{
		idx->second.antialiase = useAA;
	}
}

bool GdImgManager::GetImageAA( void * userData )
{
	std::map< void *, mtaGdImage >::iterator idx = theImages.find( userData );
	return (idx != theImages.end())?(idx->second.antialiase):false;
}