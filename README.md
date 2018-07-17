# MeshFrame

![CGAL](Docs/MeshFrame.png)

A light-weighted, efficient and header-only mesh processing frame work.
Its speed is superior to other state-of-the-art libraries like OpenMesh, 
MeshLab or CGAL. It supports dynamic mesh structure editing, supports 
runtime dynamic properties, supports triangle/tetrahedral mesh, with a 
built-in viewer, and also includes a large number of grid processing algorithms.

Usage
=============
MeshFrame is designed to be used in header-only style, which means users are not 
required to pre-compile any binary library before using. Only thing user need to do is 
put MeshFrame in include directory. We sugguest that user set MeshFrame directory 
as a environment viariable **"MESHFRAME_DIRECTORY"**, and use a CMakeLists.txt like this:

```
cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED ENV{MESHFRAME_DIRECTORY})
    message(FATAL_ERROR "not defined environment variable:MESHFRAME_DIRECTORY")  
else()
	message("Defined environment variable:MESHFRAME_DIRECTORY:")
	message( $ENV{MESHFRAME_DIRECTORY})
endif() 

project(Demo)
include_directories($ENV{MESHFRAME_DIRECTORY})

file(GLOB SRC
    #Add your source files here
	"$ENV{MESHFRAME_DIRECTORY}/MeshLib/core/viewer/MeshViewer.cpp"
	"$ENV{MESHFRAME_DIRECTORY}/MeshLib/core/bmp/RgbImage.cpp"
)

add_executable(Demo ${SRC})
```

If you want to use the viewer moudule, you should link your project to freeglut 
library, which is provided in "MeshFrame/3rdParty/", the CMakeLists.txt is like 
this:  

```
cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED ENV{MESHFRAME_DIRECTORY})
    message(FATAL_ERROR "not defined environment variable:MESHFRAME_DIRECTORY")  
else()
	message("Defined environment variable:MESHFRAME_DIRECTORY:")
	message( $ENV{MESHFRAME_DIRECTORY})
endif() 

project(MeshViewer)
include_directories($ENV{MESHFRAME_DIRECTORY})
set(FreeGlutDir $ENV{MESHFRAME_DIRECTORY}/MeshLib/3rdParty/freeglut)
include_directories(${FreeGlutDir}/include)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FreeGlutLibDir ${FreeGlutDir}/lib/x64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(FreeGlutLibDir ${FreeGlutDir}/lib)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

include_directories($ENV{MESHFRAME_DIRECTORY})
link_directories(${FreeGlutLibDir})

file(GLOB SRC
    #Add your source files here
	"$ENV{MESHFRAME_DIRECTORY}/MeshLib/core/viewer/MeshViewer.cpp"
	"$ENV{MESHFRAME_DIRECTORY}/MeshLib/core/bmp/RgbImage.cpp"
)
add_executable(MeshViewer ${SRC})
```

## Dependencies
MeshFrame requires no necessary dependencies, and it can work just fine alone. 
However, some of the modules requires optional dependences like [Eigen](http://eigen.tuxfamily.org) for 
some computational algorithms, and [FreeGlut](http://http://freeglut.sourceforge.net) for visualization.
The FreeGlut is provided with MeshFrame in "MeshFrame/3rdParty/".

## Contact

MeshFrame is a group endeavor developed by [He Chen](http://ankachan.github.io/), [Haoyu Chen](https://github.com/IronHaoyu), Yiming Zhu, [sskqgfnnh](https://github.com/sskqgfnnh).

Please contact us if you have questions or comments. For troubleshooting, please post an issue on github.

This is MeshFrame official mailbox:

**MeshFrame@hotmail.com**

Or you can contact He Chen at:

**ankachan@126.com** 


