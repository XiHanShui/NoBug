//
// Created by WJ on 2021/5/9.
//

#ifndef FFMPEGDEMO_LOG_H
#define FFMPEGDEMO_LOG_H

#include <android/log.h>

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"FFMPEG_",__VA_ARGS__)

#endif //FFMPEGDEMO_LOG_H
