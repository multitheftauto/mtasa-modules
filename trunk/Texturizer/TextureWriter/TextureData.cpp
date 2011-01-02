/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#include "TextureData.hpp"

TextureData::TextureData(const char * tName, const char * aName, bool hasAlpha, bool compress, TextureImage * img)
{
	uint32_t format = compress?(hasAlpha?FORMAT_1555:FORMAT_565):(hasAlpha?FORMAT_8888:FORMAT_888);
	
	img->LoadImageToData(format, hasAlpha);
	if(compress) img->Compress();

	m_Data.id = 0x01;
	m_Data.marker = VERSION_GTASA;
	m_Data.version = 0x09;
	m_Data.filter_flags = 0x1101;

	memset(m_Data.texture_name, 0x0, 32 ); 
	memset(m_Data.alpha_name, 0x0, 32 ); 

#if WIN32
	strcpy_s(m_Data.texture_name, 32, tName);
#else
	strcpy(m_Data.texture_name, tName);
#endif
	if(hasAlpha)
	{
#if WIN32
		strcpy_s(m_Data.alpha_name, 32, aName);
#else
		strcpy(m_Data.alpha_name, aName);
#endif
	}

	m_Data.alpha_flags = format;
	m_Data.direct3d_texture_format = compress?0x31545844:0x0; // 0x31545844 = DXT1
	m_Data.width = (uint16_t)img->width;
	m_Data.height = (uint16_t)img->height;
	m_Data.depth = 0x10;
	m_Data.mipmap_count = 1;
	m_Data.texcode_type = 0x04;
	m_Data.flags = compress?(hasAlpha?9:8):(hasAlpha?1:0);

	m_Data.palette = NULL;

	m_Data.data_size = img->fileData.size();
	m_Data.data = new uint8_t[img->fileData.size()];

	m_Data.chunk_size += ( 5 * sizeof(uint32_t) );
	m_Data.chunk_size += 64;
	m_Data.chunk_size += ( 2 * sizeof(uint16_t) );
	m_Data.chunk_size += ( 4 * sizeof(uint8_t) );
	m_Data.chunk_size += m_Data.depth==8?256*4:0;
	m_Data.chunk_size += m_Data.data_size;
}