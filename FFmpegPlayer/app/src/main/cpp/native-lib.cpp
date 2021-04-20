#include <jni.h>
#include <string>
#include "Log.h"


extern "C"
JNIEXPORT jlong JNICALL
Java_com_socct_ffmpegplayer_EnjoyPlayer_nativeInit(JNIEnv *env, jobject thiz) {
    LOGE("%s", "检测初始化");

    return 1;


}


extern "C"
JNIEXPORT void JNICALL
Java_com_socct_ffmpegplayer_EnjoyPlayer_setDataSource(JNIEnv *env, jobject thiz, jlong handle,
                                                      jstring path) {

}extern "C"
JNIEXPORT void JNICALL
Java_com_socct_ffmpegplayer_EnjoyPlayer_onStop(JNIEnv *env, jobject thiz, jlong handle) {

}extern "C"
JNIEXPORT void JNICALL
Java_com_socct_ffmpegplayer_EnjoyPlayer_onPause(JNIEnv *env, jobject thiz, jlong handle) {

}