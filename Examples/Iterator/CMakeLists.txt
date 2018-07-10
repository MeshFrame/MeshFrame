cmake_minimum_required(VERSION 2.8)

project(IterateMeshTest)

include_directories($ENV{MESHLIB_ANDROID})

file(GLOB SRC
    "*.h"
    "*.cpp"
)
add_executable(IterateMeshTest ${SRC})