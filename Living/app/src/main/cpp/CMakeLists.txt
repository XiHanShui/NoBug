# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html


cmake_minimum_required(VERSION 3.10.2)

project("living")

add_subdirectory(librtmp)

add_library(
        living

        SHARED

        native-lib.cpp)


find_library(
        log-lib
        rtmp
        log)


target_link_libraries(
        living
        ${log-lib})