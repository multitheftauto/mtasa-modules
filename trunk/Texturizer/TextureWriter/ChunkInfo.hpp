/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#ifndef CHUNKINFO_HPP_
#define CHUNKINFO_HPP_

#include "pstdint.h"

#define VERSION_GTASA 0x1803ffff

#define CHUNK_INFO 0x01
#define CHUNK_FILE 0x16
#define CHUNK_EXTRA 0x03
#define CHUNK_TEXTURE 0x15

struct txd_chunk_t
{
	uint32_t id;
	uint32_t chunk_size;       // does not include id, chunk_size and marker
	uint32_t marker;           // 0x1803ffff or 0x1003ffff
	uint8_t*  data; // [chunk_size];
};

struct txd_file_t
{
	uint32_t id;               // 0x16 (22)
	uint32_t chunk_size;        // file_size - 12
	uint32_t marker;
	uint8_t*  data; //[chunk_size];
};

struct txd_info_t
{
	uint32_t id;               // 0x01 (1)
	uint32_t chunk_size;       // 0x04 (4)
	uint32_t marker;
	uint16_t count;            // number of textures in the dictionary
	uint16_t unknown;
};

struct txd_texture_t
{
	uint32_t id;               // 0x15 (21)
	uint32_t chunk_size;
	uint32_t marker;
	uint8_t*  data; //[chunk_size];
};

struct mipmaps_t
{
	uint32_t data_size;
	uint8_t*  data; //[data_size];
};

struct txd_texture_data_t
{
	uint32_t id;                         // 0x01 (1)
	uint32_t chunk_size;
	uint32_t marker;
	uint32_t version;                    // 0x08 (8) or 0x09 (9)
	uint32_t filter_flags;
	char     texture_name[32];
	char     alpha_name[32];
	uint32_t alpha_flags;
	uint32_t direct3d_texture_format;    // see below
	uint16_t width;
	uint16_t height;
	uint8_t  depth;
	uint8_t  mipmap_count;
	uint8_t  texcode_type;
	uint8_t  flags;
	uint8_t*  palette;	//[depth==8 ? 256 * 4 : 0];
	uint32_t data_size;
	uint8_t*  data;		//[data_size];
	mipmaps_t* mipmaps; //[mipmap_count - 1];
};

struct txd_extra_info_t
{
	uint32_t id;               // 0x03 (3)
	uint32_t chunk_size;
	uint32_t marker;
	uint8_t*  data; //[chunk_size];
};

#endif
