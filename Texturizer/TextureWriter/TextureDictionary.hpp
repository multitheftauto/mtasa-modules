/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42, JoRX
*
******************************************************/

#ifndef TEXTUREINFOWRITER_HPP_
#define TEXTUREINFOWRITER_HPP_

#include "pstdint.h"

#include <fstream>
#include <map>
#include <vector>
#include "ChunkInfo.hpp"

class TextureDictionary
{
private:
	struct Texture_Info
	{
		txd_texture_data_t			TextureData;
		txd_extra_info_t			ExtraInfo;
	};

	txd_file_t						mFile_Data;
	txd_info_t						mInfo_Data;
	txd_extra_info_t				mExtraInfo_Data;
	std::map< int, Texture_Info>	mTextureInfo;

	// Helper methods for writing
	void writeInfo(std::fstream& fs, txd_info_t info);
	void writeExtraInfo(std::fstream& fs, txd_extra_info_t info);
	void writeTextureInfo(std::fstream& fs, Texture_Info tex, uint32_t size);

	// Helpe methods for getting chunk sizes
	uint32_t getFileChunkSize();
	uint32_t getTextureChunkSize(Texture_Info tex);

public:
	// Default constructor
	TextureDictionary();

	/**
	 * Adds new texture to dictionary, freeing all memory allocated for this is users responsibility.
	 * @param textureData Texture data to be added to dictionary.
	 * @param extraInfo Extra info for added texture data.
	 * @returns int Itentifier for added texture, if adding fails -1 will be returned.
	 */
	int addTexture(txd_texture_data_t textureData, txd_extra_info_t extraInfo);

	/**
	 * Removes Texture with matching <i>id</i>.
	 * @param id Identifier for texture to be removed.
	 */
	void removeTexture(int id);

	/**
	 * Writes texture dictionary to <i>filename</i>.
	 * @param filename File where texture dictionary will be written.
	 * @returns bool Success.
	 */
	bool Write(const char* filename);
};

#endif
