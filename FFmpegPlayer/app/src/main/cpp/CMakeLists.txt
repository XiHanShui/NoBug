cmake_minimum_required(VERSION 3.10.2)
project("ffmpegplayer")


# 添加依赖路径
set(libs ${CMAKE_SOURCE_DIR}/${ANDROID_ABI})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${libs}")

# 添加头文件地址
include_directories(${CMAKE_SOURCE_DIR}/include)
# 将库链接起来


add_library(
        native-lib
        SHARED
        native-lib.cpp
        EnjoyPlayer.cpp
        AudioChannel.cpp
        VideoChannel.cpp
        JavaCallHelper.cpp
)


find_library(
        log-lib
        log)

target_link_libraries(
        native-lib
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