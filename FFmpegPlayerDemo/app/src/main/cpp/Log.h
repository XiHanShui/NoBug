//
// Created by WJ on 2021/5/9.
//

#ifndef FFMPEGDEMO_LOG_H
#define FFMPEGDEMO_LOG_H

#include <android/log.h>

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"FFMPEG_",__VA_ARGS__)


void loge(char *format, char *msg) {
    __android_log_print(ANDROID_LOG_ERROR, "FFMPEG_", format, msg);
}

void loge(char *msg) {
    loge(msg, nullptr);
}


#endif //FFMPEGDEMO_LOG_H
