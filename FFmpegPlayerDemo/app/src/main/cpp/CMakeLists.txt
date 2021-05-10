# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html


cmake_minimum_required(VERSION 3.10.2)


project("ffmpeg")
aux_source_directory(${CMAKE_SOURCE_DIR} SOURCE)


add_library(
        ffmpeg
        SHARED
        ${SOURCE}
        AudioChannel.cpp
)


set(libs ${CMAKE_SOURCE_DIR}/${ANDROID_ABI})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${libs}")
include_directories(${CMAKE_SOURCE_DIR}/include)


find_library(
        log-lib
        log)


target_link_libraries(
        ffmpeg
        avfilter
        avformat
        avcodec
        avutil
        swresample
        swscale
        rtmp
        z
        android
        OpenSLES
        ${log-lib})