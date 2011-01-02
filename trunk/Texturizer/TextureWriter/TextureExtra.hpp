/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#ifndef __TEXTUREEXTRA_H
#define __TEXTUREEXTRA_H

#include "ChunkInfo.hpp"

class TextureExtra
{
public:
	TextureExtra();

	txd_extra_info_t 
		m_Data;
};

#endif