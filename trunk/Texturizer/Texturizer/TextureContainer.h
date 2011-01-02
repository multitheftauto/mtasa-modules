/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42
*
******************************************************/

#ifndef TEXTURECONTAINER_H_
#define TEXTURECONTAINER_H_

#include <map>

#include "TextureWriter.h"

class TextureContainer
{
public:
	TextureContainer( );
	~TextureContainer( );
	
	int AddContainer( );

	bool HasMember( int index );

	std::map< int, TextureDictionary *> mContainers;

private:
	int mIndex;
};

#endif