cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED ENV{MESHFRAME_DIRECTORY})
    message(FATAL_ERROR "not defined environment variable:MESHFRAME_DIRECTORY")  
else()
	message("Defined environment variable:MESHFRAME_DIRECTORY:")
	message( $ENV{MESHFRAME_DIRECTORY})
endif() 

project(MotionSequenceDisplay)

include_directories($ENV{MESHFRAME_DIRECTORY})
set(FreeGlutDir $ENV{MESHFRAME_DIRECTORY}/MeshFrame/3rdParty/freeglut)
include_directories(${FreeGlutDir}/include)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FreeGlutLibDir ${FreeGlutDir}/lib/x64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FreeGlutLibDir ${FreeGlutDir}/lib)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

include_directories($ENV{AC_DIR})
include_directories($ENV{MESHFRAME_DIRECTORY})
link_directories(${FreeGlutLibDir})

file(GLOB SRC
    "*.h"
    "*.cpp"
	"$ENV{MESHFRAME_DIRECTORY}/MeshFrame/core/viewer/MeshViewer.cpp"
	"$ENV{MESHFRAME_DIRECTORY}/MeshFrame/core/bmp/RgbImage.cpp"
)
add_executable (DynamicFaceSequenceDisplay ${SRC})