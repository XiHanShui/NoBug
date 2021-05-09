//
// Created by WJ on 2021/5/9.
//

#ifndef FFMPEGDEMO_JAVACALLHELPER_H
#define FFMPEGDEMO_JAVACALLHELPER_H

#include <jni.h>
// 标记线程
#define  THREAD_MAIN 1
#define  THREAD_CHILD 2

// 错误Code
// 打不开视频
#define FFMPEG_CAN_NOT_OPEN_URL 1
//找不到流媒体
#define  FFMPEG_CAN_NOT_FIND_STREAMS 2
// 找不到解码器
#define  FFMPEG_FIND_DECODER_FAIL 3
//无法更具解码器创建上下文
#define  FFMPEG_ALLOC_CODEC_CONTEXT_FAIL 4
// 根据流信息 配置上下文失败
#define  FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL 5
//打开解码器失败
#define FFMPEG_OPEN_DECODER_FAIL 6
// 没有音视频
#define  FFMPEG_NO_MEDIA 7


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


#endif //FFMPEGDEMO_JAVACALLHELPER_H
