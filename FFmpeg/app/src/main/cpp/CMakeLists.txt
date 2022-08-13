# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.18.1)
project("practise")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(jniLibs ${CMAKE_SOURCE_DIR}/../nativeLibs)

file(GLOB src-files
        ${CMAKE_SOURCE_DIR}/*.cpp
        ${CMAKE_SOURCE_DIR}/util/*.cpp
        ${CMAKE_SOURCE_DIR}/common/*.cpp
        ${CMAKE_SOURCE_DIR}/player/*.cpp
        ${CMAKE_SOURCE_DIR}/player/decoder/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/video/*.cpp
        ${CMAKE_SOURCE_DIR}/player/render/audio/*.cpp
        ${CMAKE_SOURCE_DIR}/recorder/SingleVideoRecorder.cpp
        ${CMAKE_SOURCE_DIR}/recorder/SingleAudioRecorder.cpp
        ${CMAKE_SOURCE_DIR}/recorder/MediaRecorderContext.cpp
        ${CMAKE_SOURCE_DIR}/recorder/GLCameraRender.cpp
        ${CMAKE_SOURCE_DIR}/recorder/MediaRecorder.cpp
        )

include_directories(
        include
        glm
        common
        ${CMAKE_SOURCE_DIR}/recorder
        ${CMAKE_SOURCE_DIR}/util
        ${CMAKE_SOURCE_DIR}/player
        ${CMAKE_SOURCE_DIR}/player/decoder
        ${CMAKE_SOURCE_DIR}/player/render
        ${CMAKE_SOURCE_DIR}/player/render/audio
        ${CMAKE_SOURCE_DIR}/player/render/video)


link_directories(${jniLibs}/${ANDROID_ABI})

add_library(
        practise
        SHARED
        ${src-files}
)
set(third-party-libs
        avformat
        avcodec
        avfilter
        swresample
        swscale
        avutil
        fdk-aac
        x264
        z
        android
        OpenSLES
        )


find_library(
        log-lib
        log)


target_link_libraries(
        practise
        ${third-party-libs}
        ${log-lib})