# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
cmake_minimum_required(VERSION 3.18.1)
project("practise")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(jniLibs ${CMAKE_SOURCE_DIR}/../nativeLibs)

file(GLOB src_files *.cpp)


include_directories(
        include
)
link_directories(${jniLibs}/${ANDROID_ABI})

add_library(
        practise
        SHARED
        ${src_files})
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