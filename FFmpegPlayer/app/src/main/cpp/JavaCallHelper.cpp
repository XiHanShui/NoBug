//
// Created by WJ on 2021/4/17.
//

#include "JavaCallHelper.h"


JavaCallHelper::JavaCallHelper(JavaVM *_javaVM, JNIEnv *_env, jobject &_jobj) : javaVm(_javaVM),
                                                                                env(_env) {
    jclass clazz = _env->GetObjectClass(jobj);
    jmid_error = _env->GetMethodID(clazz, "onError", "(I)V");
    jmid_prepare = _env->GetMethodID(clazz, "onPrepare", "()V");
    jmid_progress = _env->GetMethodID(clazz, "onProgress", "(I)V");
    jobj = _env->NewGlobalRef(_jobj);

}

JavaCallHelper::~JavaCallHelper() {
    env->DeleteGlobalRef(jobj);
    jobj = nullptr;
}

void JavaCallHelper::onError(int code, int thread) {
    if (thread == THREAD_CHILD) {
        JNIEnv *jniEnv;
        if (javaVm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_error, code);
        javaVm->DetachCurrentThread();
    } else {
        env->CallVoidMethod(jobj, jmid_error, code);
    }
}

void JavaCallHelper::onPrepare(int thread) {

    if (thread == THREAD_CHILD) {
        JNIEnv *jniEnv;
        if (javaVm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_prepare);
        javaVm->DetachCurrentThread();
    } else {
        env->CallVoidMethod(jobj, jmid_prepare);
    }


}

void JavaCallHelper::onProgress(int progress, int thread) {
    if (thread == THREAD_CHILD) {
        JNIEnv *jniEnv;
        if (javaVm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_progress, progress);
        javaVm->DetachCurrentThread();
    } else {
        env->CallVoidMethod(jobj, jmid_progress, progress);
    }
}
