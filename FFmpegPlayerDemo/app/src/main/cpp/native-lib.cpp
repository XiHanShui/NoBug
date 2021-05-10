#include <jni.h>
#include <string>
#include "JavaCallHelper.h"
#include "SafeQueue.h"
#include "Log.h"
#include "BaseChannel.h"
#include "JPlayer.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

extern "C"
JNIEXPORT jlong JNICALL
Java_com_socct_ffmpeg_JAudioPlayer_nativeInit(JNIEnv *env, jobject thiz) {

}

extern "C"
JNIEXPORT void JNICALL
Java_com_socct_ffmpeg_JAudioPlayer_prepare(JNIEnv *env, jobject thiz, jlong native_handle) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_socct_ffmpeg_JAudioPlayer_setDataSource(JNIEnv *env, jobject thiz, jstring path,
                                                 jlong native_handle) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_socct_ffmpeg_JAudioPlayer_onStart(JNIEnv *env, jobject thiz, jlong native_handle) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_socct_ffmpeg_JAudioPlayer_onPause(JNIEnv *env, jobject thiz, jlong native_handle) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_socct_ffmpeg_JAudioPlayer_onStop(JNIEnv *env, jobject thiz, jlong native_handle) {

}
