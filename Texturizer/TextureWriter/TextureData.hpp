/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#ifndef __TEXTUREDATA_H
#define __TEXTUREDATA_H

#include "ChunkInfo.hpp"
#include "TextureImage.h"

class TextureData
{
public:
	TextureData(const char * tName, const char * aName, bool hasAlpha, bool compress, TextureImage * img);

	txd_texture_data_t 
		m_Data;

};

#endif