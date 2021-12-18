cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED ENV{MESHFRAME_DIRECTORY})
    message(FATAL_ERROR "not defined environment variable:MESHFRAME_DIRECTORY")  
else()
	message("Defined environment variable:MESHFRAME_DIRECTORY:")
	message( $ENV{MESHFRAME_DIRECTORY})
endif() 
include_directories($ENV{MESHFRAME_DIRECTORY})

project(ReadAndWirteMesh)


file(GLOB SRC
    "*.cpp"
	"*.h"
	"$ENV{MESHFRAME_DIRECTORY}/MeshLib/core/viewer/MeshViewer.cpp"
	"$ENV{MESHFRAME_DIRECTORY}/MeshLib/core/bmp/RgbImage.cpp"
)

add_executable(ReadAndWirteMesh ${SRC})