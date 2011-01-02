/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#include "TextureInfo.hpp"

TextureInfo::TextureInfo()
{
	mFile_Data.id = CHUNK_FILE;
	mFile_Data.marker = VERSION_GTASA;
	mFile_Data.chunk_size = 0;
	mFile_Data.data = 0;
	mInfo_Data.id = CHUNK_INFO;
	mInfo_Data.marker = VERSION_GTASA;
	mInfo_Data.count = 0;
	mInfo_Data.unknown = 2;
	mExtraInfo_Data.id = CHUNK_EXTRA;
	mExtraInfo_Data.marker = VERSION_GTASA;
	mExtraInfo_Data.chunk_size = 0;
	mExtraInfo_Data.data = NULL;
}

TextureInfo::~TextureInfo()
{
	if(mFile_Data.data != 0)
		delete mFile_Data.data;
}

int TextureInfo::addTexture(txd_texture_data_t tex, txd_extra_info_t extInf)
{
	int id = 0;
	while(mTextureInfo.count(id) == 1)
		id++;
	Texture_Info ti;
	ti.TextureData = tex;
	ti.ExtraInfo = extInf;
	mTextureInfo[id] = ti;
	return id;
}

void TextureInfo::removeTexture(int id)
{
	mTextureInfo.erase(id);
}

txd_file_t TextureInfo::getTextureFileData()
{
	uint8_t* data;
	uint32_t size = 0;
	std::vector<txd_texture_t> vec;
	// loe kokku kui suur peab olema data, eralda nii palju m2lu ja kopeeri vajalik info sellese :D
	mInfo_Data.count = mTextureInfo.size();
	size = size + sizeof(mInfo_Data);
	size = size + sizeof(mExtraInfo_Data);
	std::map< int, Texture_Info>::iterator it;
	for(it = mTextureInfo.begin(); it != mTextureInfo.end(); it++)
	{
		txd_texture_t tt;
		tt.id = 0x15;
		tt.marker = VERSION_GTASA;
		int c = 0;
		tt.chunk_size = sizeof(it->second.TextureData) + sizeof(it->second.ExtraInfo)+1;
		tt.data = new uint8_t[tt.chunk_size];
		data = reinterpret_cast<uint8_t*>(&it->second.TextureData);
		for(int i = 0; i < sizeof(it->second.TextureData); i++, c++)
			tt.data[c] = data[i];
		data = reinterpret_cast<uint8_t*>(&it->second.ExtraInfo);
		for(int i = 0; i < sizeof(it->second.ExtraInfo); i++, c++)
			tt.data[c] = data[i];
		tt.chunk_size = sizeof(it->second.TextureData) + sizeof(it->second.ExtraInfo);
		vec.push_back(tt);
		size = size + sizeof(tt);
	}
	mFile_Data.chunk_size = size+1;
	printf("%d\n", size);
	mFile_Data.data = new uint8_t[size+1];
	int c = 0;
	data = reinterpret_cast<uint8_t*>(&mInfo_Data);
	for(int i = 0; i < sizeof(mInfo_Data); i++, c++)
		mFile_Data.data[c] = data[i];
	for(size_t j=0;j<vec.size(); j++)
	{
		data = reinterpret_cast<uint8_t*>(&(vec[j]));
		for(int i = 0; i < sizeof(vec[j]); i++, c++)
			mFile_Data.data[c] = data[i];
	}
	data = reinterpret_cast<uint8_t*>(&mExtraInfo_Data);
	for(int i = 0; i < sizeof(mExtraInfo_Data); i++, c++)
		mFile_Data.data[c] = data[i];
	return mFile_Data;
}
