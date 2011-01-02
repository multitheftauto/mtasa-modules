/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42
*
******************************************************/

#include "TextureContainer.h"

TextureContainer::TextureContainer() :
	mIndex(1)
{
	
}

TextureContainer::~TextureContainer()
{
	for( std::map< int, TextureDictionary * >::iterator i = mContainers.begin(); i != mContainers.end(); ++i)
	{
		delete i->second;
	}
	mContainers.clear();
}

int TextureContainer::AddContainer()
{
	mContainers[mIndex] = new TextureDictionary;
	if( mContainers[mIndex] != NULL )
	{
		mIndex++;
		return mIndex-1;
	}
	return 0;
}

bool TextureContainer::HasMember( int index )
{
	std::map< int, TextureDictionary * >::iterator idx = mContainers.find( index );
	return (idx != mContainers.end());
}