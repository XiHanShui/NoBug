//
// Created by WJ on 2021/4/17.
//

#ifndef FFMPEGPLAYER_JAVACALLHELPER_H
#define FFMPEGPLAYER_JAVACALLHELPER_H

#include <jni.h>

// 线程标记
#define  THREAD_MAIN 1
#define  THREAD_CHILD 2

// 错误code
//打不开视频
#define  CAN_NOT_OPEN_URL 1
//找不到媒体流
#define  CAN_NOT_FIND_STREAMS 2
//找不到解码器
#define  FIND_DECODER_FAIL 3
// 无法创建解码器上下文
#define  ALLOC_CODEC_CONTEXT_FAIL 4
// 根据流信息 配置上下文参数失败
#define  CODEC_CONTEXT_PARAMETERS_FAIL 5
// 打开解码器失败
#define  OPEN_DECODER_FAIL 6
// 没有音视频
#define  NO_MEDIA 7

class JavaCallHelper {

public:
    JavaCallHelper(JavaVM *_javaVM, JNIEnv *_env, jobject &_jobj);

    ~JavaCallHelper();

    void onError(int code, int thread = THREAD_MAIN);

    void onPrepare(int thread = THREAD_MAIN);

    void onProgress(int progress, int thread = THREAD_MAIN);


public:
    JavaVM *javaVm;
    JNIEnv *env;
    jobject jobj;
    jmethodID jmid_error;
    jmethodID jmid_prepare;
    jmethodID jmid_progress;
};


#endif //FFMPEGPLAYER_JAVACALLHELPER_H
