SET( include_SRCS include/ILuaModuleManager.h include/lauxlib.h include/lua.h include/lua.hpp include/luaconf.h include/lualib.h)
SET( extra_SRCS extra/CLuaArgument.h extra/CLuaArgument.cpp extra/CLuaArguments.h extra/CLuaArguments.cpp)
SET( mta_SRCS Common.h CThread.h CThread.cpp CThreadData.h CThreadData.cpp ModuleFilePaths.h ModuleFilePaths.cpp )

SET( txdCon_SRCS pstdint.h TextureContainer.h TextureContainer.cpp TextureImage.h TextureImage.cpp GdImgManager.h GdImgManager.cpp )
SET( main_SRCS CFunctions.h CFunctions.cpp ml_base.h ml_base.cpp )

include_directories (${CMAKE_SOURCE_DIR}/TextureWriter)

link_directories (${TextureWriter_BINARY_DIR})
link_directories (${TextureWriter_BINARY_DIR}/TextureWriter)
link_directories (${CMAKE_SOURCE_DIR}/Texturizer/lib)

message(STATUS "Lib: ${TextureWriter_BINARY_DIR}")

ADD_LIBRARY( Texturizer SHARED ${include_SRCS} ${extra_SRCS} ${mta_SRCS} ${txdCon_SRCS} ${main_SRCS} )
target_link_libraries (Texturizer TextureWriter)
target_link_libraries (Texturizer lua5.1.lib)
target_link_libraries (Texturizer ${GD_LIBRARIES})
target_link_libraries (Texturizer ${SQUISH_LIBRARIES})