//
// Created by WJ on 2021/4/17.
//

#ifndef FFMPEGPLAYER_LOG_H
#define FFMPEGPLAYER_LOG_H
#include <android/log.h>
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"FFmpeg",__VA_ARGS__)
#endif //FFMPEGPLAYER_LOG_H
