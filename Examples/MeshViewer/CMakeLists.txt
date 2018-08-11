cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED ENV{MESHFRAME_DIRECTORY})
    message(FATAL_ERROR "not defined environment variable:MESHLIB_ANDROID")  
else()
	message("Defined environment variable:MESHLIB_ANDROID:")
	message( $ENV{MESHFRAME_DIRECTORY})
endif() 

project(MeshViewer)
include_directories($ENV{MESHFRAME_DIRECTORY})
set(FreeGlutDir $ENV{MESHFRAME_DIRECTORY}/MeshFrame/3rdParty/freeglut)
include_directories(${FreeGlutDir}/include)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FreeGlutLibDir ${FreeGlutDir}/lib/x64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FreeGlutLibDir ${FreeGlutDir}/lib)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

include_directories($ENV{MESHFRAME_DIRECTORY})
link_directories(${FreeGlutLibDir})

file(GLOB SRC
    "*.h"
    "*.cpp"
	"$ENV{MESHFRAME_DIRECTORY}/MeshFrame/core/viewer/MeshViewer.cpp"
	"$ENV{MESHFRAME_DIRECTORY}/MeshFrame/core/bmp/RgbImage.cpp"
)
add_executable(MeshViewer ${SRC})