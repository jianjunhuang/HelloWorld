#include <jni.h>
#include <pthread.h>
#include <logutil.h>
#include <unistd.h>

//
// Created by jianjun huang on 2024/1/21.
//

struct ThreadArgs {
    int id;
    int result;
};

void *printThreadHello(void *) {
    LOGD("thread created");
    return nullptr;
}

void *printArgsThreadHello(void *arg) {
    if (arg == nullptr) {
        LOGD("thread start: args == null");
        pthread_exit(0);
    }
    ThreadArgs *args = static_cast<ThreadArgs *>(arg);
    LOGD("thread created:id %d: ", args->id);
    LOGD("thread created:result %d: ", args->result);
    return nullptr;
}

void *printJoinThreadHello(void *arg) {
    if (arg == nullptr) {
        LOGD("thread start: args == null");
        pthread_exit(0);
    }
    ThreadArgs *args = static_cast<ThreadArgs *>(arg);
    LOGD("thread created:id %d: ", args->id);
    LOGD("thread created:result %d: ", args->result);
    sleep(1);
    return reinterpret_cast<void *>(args->result);
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_createJNIThread(JNIEnv *env, jobject thiz) {

    pthread_t handles;
    int result = pthread_create(&handles, nullptr, printArgsThreadHello, nullptr);
    if (result == 0) {
        LOGD("thread create success");
    } else {
        LOGD("thread create failed");
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_createJNIThreadWithArgs(JNIEnv *env, jobject thiz) {
    pthread_t handles;
    ThreadArgs *args = new ThreadArgs;
    args->id = 0;
    args->result = 200;
    int result = pthread_create(&handles, nullptr, printArgsThreadHello, args);
    if (result == 0) {
        LOGD("thread create success");
    } else {
        LOGD("thread create failed");
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_joinThreadInJNI(JNIEnv *env, jobject thiz) {
    pthread_t handles;
    ThreadArgs *args = new ThreadArgs;
    args->id = 0;
    args->result = 200;
    int result = pthread_create(&handles, nullptr, printJoinThreadHello, args);
    if (result == 0) {
        LOGD("thread create success");
    } else {
        LOGD("thread create failed");
    }
    void *ret = nullptr;
    //挂起，等待线程结束
    pthread_join(handles, &ret);
    LOGD("thread join result -> %d", ret);
}