/******************************************************
*
* Project:    Texturizer (CMake).
* Website:    http://wiki.multitheftauto.com/wiki/Modules/Texturizer
* Developers: jyrno42
*
******************************************************/

#include "TextureImage.h"

TextureImage::TextureImage()
{
	fileName = "";
	myImage = NULL;
	errorCode = -1;
	fromFile = false;

	Init();
}
TextureImage::TextureImage(std::string fName)
{
	fileName = fName;
	myImage = NULL;
	errorCode = -1;
	fromFile = true;

	Init();
}
TextureImage::TextureImage(gdImagePtr im)
{
	fileName = "";
	myImage = im;
	errorCode = -1;
	fromFile = false;

	Init();
}

TextureImage::~TextureImage()
{
	fileData.clear();
}

void TextureImage::Init( )
{
	cFilter = FORMAT_DEFAULT;

	fOrder[FORMAT_DEFAULT] = ORDER_RGBA;
	fOrder[FORMAT_1555] = ORDER_RGBA;
	fOrder[FORMAT_565] = ORDER_RGBA;
	fOrder[FORMAT_4444] = ORDER_RGBA;
	fOrder[FORMAT_LUM8] = ORDER_RGBA;
	fOrder[FORMAT_8888] = ORDER_BGRA;
	fOrder[FORMAT_888] = ORDER_BGRA;
	fOrder[FORMAT_555] = ORDER_RGBA;
}

void TextureImage::LoadFromFile()
{
	std::fstream fs(fileName.c_str(), std::ios::in|std::ios::binary);
	if( fs.is_open() )
	{
		// get length of file:
		fs.seekg (0, std::ios::end);
		int length = fs.tellg();
		fs.seekg (0, std::ios::beg);

		// allocate memory:
		char * buffer = new char[length];

		// read data as a block:
		fs.read (buffer, length);
		fs.close();

		myImage = gdImageCreateFromPngPtr(length, buffer);
		delete[] buffer;
	}
	else
	{
		errorCode = -2;
	}
}

void TextureImage::LoadImageToData( uint32_t filter, bool mAlpha )
{
	cFilter = filter;

	if(fromFile)
	{
		LoadFromFile();
	}

	if( myImage == NULL )
	{
		errorCode = -3;
	}
	else
	{
		width = gdImageSX( myImage );
		height = gdImageSY( myImage );

		int len = width*height*4;

		for( int y = 0; y < height; y++)
		{
			for( int x = 0; x < width; x++)
			{
				int c = gdImageGetPixel( myImage, x, y );
				
				if( fOrder[cFilter] == ORDER_RGBA )
				{
					fileData.push_back( (unsigned char)gdImageRed( myImage, c ) );
					fileData.push_back( (unsigned char)gdImageGreen( myImage, c ) );
					fileData.push_back( (unsigned char)gdImageBlue( myImage, c ) );
					fileData.push_back( mAlpha?0xFF*(gdImageAlpha( myImage, c )/127):0xFF );
				}
				else
				{
					fileData.push_back( (unsigned char)gdImageBlue( myImage, c ) );
					fileData.push_back( (unsigned char)gdImageGreen( myImage, c ) );
					fileData.push_back( (unsigned char)gdImageRed( myImage, c ) );
					fileData.push_back( mAlpha?0xFF*(gdImageAlpha( myImage, c )/127):0xFF );
				}
			}
		}
		if(fromFile) gdImageDestroy(myImage);
		errorCode = 0;
	}
}

void TextureImage::Compress( )
{
	u8 * pixels = new u8[width*height*4];

	for( size_t i = 0; i < fileData.size(); i++)
	{
		pixels[i] = fileData[i];
	}
	u8 * block = new u8[width*height/2];

	CompressImage( pixels, width, height, block, 0);

	fileData.clear();
	for( int i = 0; i < width*height/2; i++)
	{
		fileData.push_back(block[i]);
	}

	//squish::Compress
}