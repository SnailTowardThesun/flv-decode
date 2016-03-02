cmake_minimum_required(VERSION 3.3)
project(flv_decode)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp protocol/FlvHeader.cpp protocol/FlvHeader.h protocol/FlvPackage.cpp protocol/FlvPackage.h kernel/FlvKernel.h app/FlvBuffer.cpp app/FlvBuffer.h app/FlvLog.cpp app/FlvLog.h FlvDecode.cpp FlvDecode.h kernel/FlvCommonHeaderFile.h)
add_executable(flv_decode ${SOURCE_FILES})