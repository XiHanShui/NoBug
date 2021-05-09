//
// Created by WJ on 2021/5/9.
//

#include "JavaCallHelper.h"

JavaCallHelper::JavaCallHelper(JavaVM *_javaVM, JNIEnv *_env, jobject &_jobj) : javaVm(_javaVM),
                                                                                env(env) {
    jobj = env->NewGlobalRef(_jobj);
    jclass jclazz = env->GetObjectClass(jobj);
    jmid_error = env->GetMethodID(jclazz, "onError", "(I)V");
    jmid_prepare = env->GetMethodID(jclazz, "onPrepare", "()V");
    jmid_progress = env->GetMethodID(jclazz, "onProgress", "(I)V");
}

JavaCallHelper::~JavaCallHelper() {
    env->DeleteGlobalRef(jobj);
    jobj = nullptr;
    jmid_error = nullptr;
    jmid_progress = nullptr;
    jmid_prepare = nullptr;
}

void JavaCallHelper::onError(int code, int thread) {
    if (thread == THREAD_MAIN) {
        env->CallVoidMethod(jobj, jmid_error, code);
    } else {
        JNIEnv *jniEnv;
        if (javaVm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_error, code);
        javaVm->DetachCurrentThread();
    }
}

void JavaCallHelper::onPrepare(int thread) {
    if (thread == THREAD_MAIN) {
        env->CallVoidMethod(jobj, jmid_prepare);
    } else {
        JNIEnv *jniEnv;
        if (javaVm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_prepare);
        javaVm->DetachCurrentThread();
    }
}

void JavaCallHelper::onProgress(int progress, int thread) {
    if (thread == THREAD_MAIN) {
        env->CallVoidMethod(jobj, jmid_progress, progress);
    } else {
        JNIEnv *jniEnv;
        if (javaVm->AttachCurrentThread(&jniEnv, nullptr) != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(jobj, jmid_progress, progress);
        javaVm->DetachCurrentThread();
    }
}
