SET( txdw_SRCS pstdint.h ChunkInfo.hpp TextureData.hpp TextureData.cpp TextureExtra.hpp TextureExtra.cpp TextureDictionary.hpp TextureDictionary.cpp TextureInfo.hpp TextureInfo.cpp TextureWriter.h)

include_directories (${CMAKE_SOURCE_DIR}/Texturizer)

ADD_LIBRARY( TextureWriter STATIC ${txdw_SRCS} )