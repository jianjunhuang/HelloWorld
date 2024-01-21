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

/*
 * 线程互斥: 确保任意时刻只有一个线程可以占用资源
 * 需要避免死锁
 */
pthread_mutex_t mutex;
/*
 * 条件变量, 线程间唤醒和等待
 */
pthread_cond_t cond;
pthread_t waitHandle;
pthread_t notifyHandle;
int flag = 0;

void *waitThread(void *) {
    LOGD("wait: lock");
    pthread_mutex_lock(&mutex);
    while (flag == 0) {
        LOGD("wait: flag==0");
        //等待唤起, release mutex
        pthread_cond_wait(&cond, &mutex);
    }

    LOGD("wait: unlock");
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}

void *notifyThread(void *) {
    LOGD("notify: lock");
    //wait wait release lock
    pthread_mutex_lock(&mutex);
    flag = 1;
    LOGD("notify: unlock");
    pthread_mutex_unlock(&mutex);

    LOGD("notify: signal");
    pthread_cond_signal(&cond);
    pthread_exit(0);
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_jni_lib_HelloJNI_waitNativeThread(JNIEnv *env, jobject thiz) {
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond, nullptr);
    pthread_create(&waitHandle, nullptr, waitThread, nullptr);
    pthread_create(&notifyHandle, nullptr, notifyThread, nullptr);
}