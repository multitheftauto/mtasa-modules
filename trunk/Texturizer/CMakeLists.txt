cmake_minimum_required (VERSION 2.6 FATAL_ERROR)

project (Texturizer)

# The version number.
set (Texturizer_VERSION_MAJOR 0)
set (Texturizer_VERSION_MINOR 0)
set (Texturizer_VERSION_RELEASE 1)

if (TXD_VERSION_MAJOR)
set (Texturizer_VERSION_MAJOR ${TXD_VERSION_MAJOR})
endif (TXD_VERSION_MAJOR)

if (TXD_VERSION_MINOR)
set (Texturizer_VERSION_MINOR ${TXD_VERSION_MINOR})
endif (TXD_VERSION_MINOR)

if (TXD_VERSION_RELEASE)
set (Texturizer_VERSION_RELEASE ${TXD_VERSION_RELEASE})
endif (TXD_VERSION_RELEASE)

message(STATUS "Version Set To: ${Texturizer_VERSION_MAJOR}.${Texturizer_VERSION_MINOR}.${Texturizer_VERSION_RELEASE}")

configure_file ( "${PROJECT_SOURCE_DIR}/AppVersion.h.in" "${PROJECT_BINARY_DIR}/AppVersion.h" )

include_directories("${PROJECT_BINARY_DIR}")

find_package(GD REQUIRED)
include_directories(${GD_INCLUDE_DIR})
set(LIBS ${LIBS} ${GD_LIBRARIES})
ADD_DEFINITIONS(${GD_DEFINES})

find_package(SQUISH)
include_directories(${SQUISH_INCLUDE_DIR})
set(LIBS ${LIBS} ${SQUISH_LIBRARIES})

 
add_subdirectory (TextureWriter)
add_subdirectory (Texturizer)