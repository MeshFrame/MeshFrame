cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED ENV{MESHFRAME_DIRECTORY})
    message(FATAL_ERROR "not defined environment variable:MESHFRAME_DIRECTORY")  
else()
	message("Defined environment variable:MESHFRAME_DIRECTORY:")
	message( $ENV{MESHFRAME_DIRECTORY})
endif() 

project(MeshSimplify1)
include_directories($ENV{MESHFRAME_DIRECTORY})
include_directories($ENV{EIGEN_DIRECTORY})

set(FreeGlutDir $ENV{MESHFRAME_DIRECTORY}/MeshFrame/3rdParty/freeglut)
include_directories(${FreeGlutDir}/include)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FreeGlutLibDir ${FreeGlutDir}/lib/x64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FreeGlutLibDir ${FreeGlutDir}/lib)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
link_directories(${FreeGlutLibDir})

file(GLOB SRC
    "*.cpp"
	"*.h"
	"$ENV{MESHFRAME_DIRECTORY}/MeshFrame/core/viewer/MeshViewer.cpp"
	"$ENV{MESHFRAME_DIRECTORY}/MeshFrame/core/bmp/RgbImage.cpp"
)

add_executable(MeshSimplify1 ${SRC})