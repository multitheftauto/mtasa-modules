#include "ChunkInfo.hpp"
#include "TextureDictionary.hpp"
#include <cstring>

int main()
{
	// add textures to t
	char alpan[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	char nullc[] = {0x74,0x65,0x73,0x74,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	txd_texture_data_t tex;
	tex.id = 0x01;
	tex.marker = VERSION_GTASA;
	tex.version = 0x09;                    // 0x08 (8) or 0x09 (9)
	tex.filter_flags = 0x1101; // FILTER_NONE
	
	memset(tex.texture_name, 0x0, 32 ); 
	memset(tex.alpha_name, 0x0, 32 ); 

	strcpy(tex.texture_name, nullc);
	strcpy(tex.alpha_name, alpan);

	tex.alpha_flags = 0x0600; // FORMAT_888
	tex.direct3d_texture_format = 0x16;
	tex.width = 4;
	tex.height = 4;
	tex.depth = 0x20;
	tex.mipmap_count = 0;
	tex.texcode_type = 0x04;
	tex.flags = 0x00;

	tex.palette = NULL; //[depth==8 ? 256 * 4 : 0];
	
	unsigned char img[64] = {
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255, 
		0, 0, 0, 255
	};

	tex.data_size = sizeof(img);
	tex.data = new uint8_t[tex.data_size]; //[data_size];
	/*for(int i=0, c=0; i < tex.data_size;c++)
		for(int j = 0; j < 4; j++)
			tex.data[i++] = img[c][j];*/
	for(int i = 0; i < tex.data_size; i++)
		tex.data[i] = (uint8_t)img[i];

	tex.mipmaps = NULL; //[mipmap_count - 1];
	tex.chunk_size = 0;

	tex.chunk_size += ( 5 * sizeof(uint32_t) );
	tex.chunk_size += 64;
	tex.chunk_size += ( 2 * sizeof(uint16_t) );
	tex.chunk_size += ( 4 * sizeof(uint8_t) );
	tex.chunk_size += tex.depth==8?256*4:0;
	tex.chunk_size += tex.data_size;
	tex.chunk_size += tex.mipmap_count;
	//tex.chunk_size += 52; // Some magic... Where do these come from... :(

	printf("%d", tex.chunk_size );
	
	txd_extra_info_t extInf;
	extInf.id = CHUNK_EXTRA;
	extInf.marker = VERSION_GTASA;
	extInf.chunk_size = 0;
	extInf.data = 0;
	TextureDictionary td;
	td.addTexture(tex, extInf);
	td.addTexture(tex, extInf);
	td.Write("t.txd");
	system("pause");
	return 0;
}