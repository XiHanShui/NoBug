# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# 1. 定义so库和头文件所在目录
set(ffmpeg_lib_dir ${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
set(ffmpeg_head_dir ${CMAKE_SOURCE_DIR})
#2. 添加头文件目录
include_directories(${ffmpeg_head_dir}/include)
#3. 添加ffmpeg so
add_library(
        avcodec
        SHARED
        IMPORTED
)
set_target_properties(
        avcodec
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavcodec.so
)
add_library(
        avdevice
        SHARED
        IMPORTED
)
set_target_properties(
        avdevice
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavdevice.so
)

add_library(
        avfilter
        SHARED
        IMPORTED
)
set_target_properties(
        avfilter
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavfilter.so
)

add_library(
        avutil
        SHARED
        IMPORTED
)
set_target_properties(
        avutil
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libavutil.so
)

add_library(
        h264
        SHARED
        IMPORTED
)
set_target_properties(
        h264
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libh264.so
)

add_library(
        postproc
        SHARED
        IMPORTED
)
set_target_properties(
        postproc
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libpostproc.so
)



add_library(
        swresample
        SHARED
        IMPORTED
)
set_target_properties(
        swresample
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswresample.so
)

add_library(
        swscale
        SHARED
        IMPORTED
)
set_target_properties(
        swscale
        PROPERTIES IMPORTED_LOCATION
        ${ffmpeg_lib_dir}/libswscale.so
)

include_directories(${ffmpeg_head_dir}/include)
project("player")


add_library(native-lib
        SHARED
        native-lib.cpp)


find_library(log-lib
        log)


target_link_libraries(native-lib
        avcodec
        avdevice
        avfilter
        avutil
        h264
        postproc
        swresample
        swscale
        ${log-lib})