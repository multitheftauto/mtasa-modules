/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#ifndef TEXTUREINFO_HPP_
#define TEXTUREINFO_HPP_

#include "ChunkInfo.hpp"
#include <map>
#include <vector>

struct Texture_Info
{
	txd_texture_data_t			TextureData;
	txd_extra_info_t			ExtraInfo;
};

class TextureInfo
{
public:
	TextureInfo();
	~TextureInfo();

	txd_file_t getTextureFileData();

	// All added textures are struct object, it is users responsibility to free any memory given to texture_data
	int addTexture(txd_texture_data_t tex, txd_extra_info_t extInf);
	void removeTexture(int id);

private:
	txd_file_t						mFile_Data;
	txd_info_t						mInfo_Data;
	txd_extra_info_t				mExtraInfo_Data;
	std::map< int, Texture_Info>	mTextureInfo;
};

#endif