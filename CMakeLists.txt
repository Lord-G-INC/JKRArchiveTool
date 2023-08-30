cmake_minimum_required(VERSION 3.8)
project(JKRArchiveLib)
include_directories(Include)
option(MAKE_EXE "Create exe" OFF)
file(GLOB_RECURSE LIBRARY_SOURCE
    "Include/*.h"
    "Include/*.hpp"
    "Source/BinaryReaderAndWriter.cpp"
    "Source/JKRArchive.cpp"
    "Source/Util.cpp"
    "Source/JKRCompression.cpp"
)
add_library(JKRArchiveLib STATIC ${LIBRARY_SOURCE})
if(MAKE_EXE)
    add_executable(JKRArchiveTools "Source/Main.cpp")
    target_link_libraries(JKRArchiveTools PUBLIC JKRArchiveLib)
endif(MAKE_EXE)