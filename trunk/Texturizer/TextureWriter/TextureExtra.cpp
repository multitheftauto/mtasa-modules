/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#include "TextureExtra.hpp"

TextureExtra::TextureExtra()
{
	m_Data.id = CHUNK_EXTRA;
	m_Data.marker = VERSION_GTASA;
	m_Data.chunk_size = 0;
	m_Data.data = 0;
}