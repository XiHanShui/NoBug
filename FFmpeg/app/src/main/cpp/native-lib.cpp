#include <jni.h>
#include <string>
#include <pthread.h>
#include "util/LogUtil.h"
#include "SyncClock.h"
#include "ThreadSafeQueue.h"
#include "AVPacketQueue.h"
#include "MediaPlayer.h"
#include "MediaRecoder.h"
#include "PlayerWrapper.h"
#include "HWCodecPlayer.h"
#include "FFMediaPlayer.h"
#include "AudioDecoder.h"
#include "Decoder.h"
#include "DecoderBase.h"
#include "VideoDecoder.h"
#include "AudioGLRender.h"
#include "AudioRender.h"
#include "OpenSLRender.h"
#include "BaseGLRender.h"
#include "VRGLRender.h"
#include "VideoGLRender.h"
#include "NativeRender.h"
#include "VideoRender.h"
#include "ImageDef.h"
extern "C" {
#include <libavutil/macros.h>
#include <libavutil/version.h>
#include <libavformat/version.h>
#include <libavcodec/avcodec.h>
#include "libswscale/version.h"
#include "libavfilter/version.h"
}
using namespace std;




extern "C"
JNIEXPORT jstring JNICALL
Java_com_ffmpeg_practise_FFPlayer_getFFmpegVersion(JNIEnv *env, jclass clazz) {
    char strBuffer[1024 * 4] = {0};
    strcat(strBuffer, "libavcodec : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVCODEC_VERSION));
    strcat(strBuffer, "\nlibavformat : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFORMAT_VERSION));
    strcat(strBuffer, "\nlibavutil : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVUTIL_VERSION));
    strcat(strBuffer, "\nlibavfilter : ");
    strcat(strBuffer, AV_STRINGIFY(LIBAVFILTER_VERSION));
    strcat(strBuffer, "\nlibswresample : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWRESAMPLE_VERSION));
    strcat(strBuffer, "\nlibswscale : ");
    strcat(strBuffer, AV_STRINGIFY(LIBSWSCALE_VERSION));
    strcat(strBuffer, "\navcodec_configure : \n");
    strcat(strBuffer, avcodec_configuration());
    strcat(strBuffer, "\navcodec_license : ");
    strcat(strBuffer, avcodec_license());
    return env->NewStringUTF(strBuffer);
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeInit(JNIEnv *env, jobject thiz, jstring url,
                                             jint video_render_type, jobject surface) {
    return 1;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativePlay(JNIEnv *env, jobject thiz, jlong player_handle) {
    // TODO: implement nativePlay()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativePause(JNIEnv *env, jobject thiz, jlong player_handle) {

}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeSeekToPosition(JNIEnv *env, jobject thiz,
                                                       jlong player_handle, jfloat position) {
    // TODO: implement nativeSeekToPosition()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeStop(JNIEnv *env, jobject thiz, jlong player_handle) {
    // TODO: implement nativeStop()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeUnInit(JNIEnv *env, jobject thiz,
                                               jlong native_player_handle) {
    // TODO: implement nativeUnInit()
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeGetMediaParams(JNIEnv *env, jobject thiz, jint param_type) {
    return 1;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeOnSurfaceCreated(JNIEnv *env, jclass clazz,
                                                         jint render_type) {
    // TODO: implement nativeOnSurfaceCreated()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeOnSurfaceChanged(JNIEnv *env, jclass clazz,
                                                         jint render_type, jint width,
                                                         jint height) {
    // TODO: implement nativeOnSurfaceChanged()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeOnDrawframe(JNIEnv *env, jclass clazz, jint render_type) {
    // TODO: implement nativeOnDrawframe()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeSetGesture(JNIEnv *env, jclass clazz, jint render_type,
                                                   jfloat x_rotate_angle, jfloat y_rotate_angle,
                                                   jfloat scale) {
    // TODO: implement nativeSetGesture()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_nativeSetTouchLoc(JNIEnv *env, jclass clazz, jint render_type,
                                                    jfloat touch_x, jfloat touch_y) {
    // TODO: implement nativeSetTouchLoc()
}
extern "C"
JNIEXPORT void JNICALL
Java_com_ffmpeg_practise_FFPlayer_00024Companion_test(JNIEnv *env, jobject thiz, jboolean is_dev,
                                                      jboolean is_oversea) {

    string key;
    if (is_oversea) {
        if (is_dev){

        } else{

        }

    } else{
        if (is_dev){

        } else{

        }
    }
}