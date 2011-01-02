/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#include "TextureDictionary.hpp"

namespace
{
	const char NullData[] = { 0x00 };
}

TextureDictionary::TextureDictionary()
{
	mFile_Data.id = CHUNK_FILE;
	mFile_Data.marker = VERSION_GTASA;
	mInfo_Data.id = CHUNK_INFO;
	mInfo_Data.chunk_size = 0x04;
	mInfo_Data.marker = VERSION_GTASA;
	mInfo_Data.count = 0;
	mInfo_Data.unknown = 2;
	mExtraInfo_Data.id = CHUNK_EXTRA;
	mExtraInfo_Data.marker = VERSION_GTASA;
	mExtraInfo_Data.chunk_size = 0;
	mExtraInfo_Data.data = NULL;
}

int TextureDictionary::addTexture(txd_texture_data_t tex, txd_extra_info_t extInf)
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

void TextureDictionary::removeTexture(int id)
{
	mTextureInfo.erase(id);
}

void TextureDictionary::writeInfo(std::fstream& fs, txd_info_t info)
{
	fs.write((char*)&info.id, sizeof(info.id));
	fs.write((char*)&info.chunk_size, sizeof(info.chunk_size));
	fs.write((char*)&info.marker, sizeof(info.marker));
	fs.write((char*)&info.count, sizeof(info.count));
	fs.write((char*)&info.unknown, sizeof(info.unknown));
}

void TextureDictionary::writeExtraInfo(std::fstream& fs, txd_extra_info_t info)
{
	fs.write((char*)&info.id, sizeof(info.id));
	fs.write((char*)&info.chunk_size, sizeof(info.chunk_size));
	fs.write((char*)&info.marker, sizeof(info.marker));
	if(info.data != NULL)
		fs.write((char*)&info.data, sizeof(info.data));
}

void TextureDictionary::writeTextureInfo(std::fstream& fs, Texture_Info tex, uint32_t size)
{
	// writing header begin
	uint32_t t = 0x15;
	fs.write((char*)&t, sizeof(t));
	fs.write((char*)&size, sizeof(size));
	t = VERSION_GTASA;
	fs.write((char*)&t, sizeof(t));
	// writing header end
	// texture data writing start
	fs.write((char*)&tex.TextureData.id, sizeof(tex.TextureData.id));
	fs.write((char*)&tex.TextureData.chunk_size, sizeof(tex.TextureData.chunk_size));
	fs.write((char*)&tex.TextureData.marker, sizeof(tex.TextureData.marker));
	fs.write((char*)&tex.TextureData.version, sizeof(tex.TextureData.version));
	fs.write((char*)&tex.TextureData.filter_flags, sizeof(tex.TextureData.filter_flags));
	
	fs.write((char*)tex.TextureData.texture_name, 32);
	fs.write((char*)tex.TextureData.alpha_name, 32);

	fs.write((char*)&tex.TextureData.alpha_flags, sizeof(tex.TextureData.alpha_flags));
	fs.write((char*)&tex.TextureData.direct3d_texture_format, sizeof(tex.TextureData.direct3d_texture_format));
	fs.write((char*)&tex.TextureData.width, sizeof(tex.TextureData.width));
	fs.write((char*)&tex.TextureData.height, sizeof(tex.TextureData.height));
	fs.write((char*)&tex.TextureData.depth, sizeof(tex.TextureData.depth));
	fs.write((char*)&tex.TextureData.mipmap_count, sizeof(tex.TextureData.mipmap_count));
	fs.write((char*)&tex.TextureData.texcode_type, sizeof(tex.TextureData.texcode_type));
	fs.write((char*)&tex.TextureData.flags, sizeof(tex.TextureData.flags));
	if(tex.TextureData.palette != NULL)
		fs.write((char*)tex.TextureData.palette, 256*4);
	fs.write((char*)&tex.TextureData.data_size, sizeof(tex.TextureData.data_size));
	fs.write((char*)tex.TextureData.data, tex.TextureData.data_size);
	// this needs an interface because writing mipmaps_t struct to file will not give correct results
	/*if(tex.TextureData.mipmaps != NULL)
		fs.write((char*)tex.TextureData.mipmaps, tex.TextureData.mipmap_count);*/
	// texture data writing end
	writeExtraInfo(fs, tex.ExtraInfo);
}

uint32_t TextureDictionary::getFileChunkSize()
{
	uint32_t s = 6 * sizeof(uint32_t) + 2 * sizeof(uint16_t) + mExtraInfo_Data.chunk_size;
	std::map< int, Texture_Info>::iterator it;
	for(it = mTextureInfo.begin(); it != mTextureInfo.end(); it++)
	{
		s = s + getTextureChunkSize(it->second) + 3 * sizeof(uint32_t);
	}
	return s;
}

uint32_t TextureDictionary::getTextureChunkSize(Texture_Info tex)
{
	uint32_t s = 6 * sizeof( uint32_t );
	s += tex.TextureData.chunk_size;
	s += tex.ExtraInfo.chunk_size;
	//printf("texChunk:%d\n", s);
	return s;
}

bool TextureDictionary::Write(const char* filename)
{
	mFile_Data.chunk_size = getFileChunkSize();
	std::fstream fs(filename, std::ios::out|std::ios::binary);

	if( fs.is_open() )
	{
		fs.write((char*)&mFile_Data.id, sizeof(mFile_Data.id));
		fs.write((char*)&mFile_Data.chunk_size, sizeof(mFile_Data.chunk_size));
		fs.write((char*)&mFile_Data.marker, sizeof(mFile_Data.marker));
		// info
		mInfo_Data.count = mTextureInfo.size();
		writeInfo(fs, mInfo_Data);
		// textures
		std::map< int, Texture_Info>::iterator it;
		for(it = mTextureInfo.begin(); it != mTextureInfo.end(); it++)
		{
			writeTextureInfo(fs, it->second, getTextureChunkSize(it->second));
		}
		// extra info
		writeExtraInfo(fs, mExtraInfo_Data);

		fs.close();
		return true;
	}
	return false;
}