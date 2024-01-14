//
// Created by jianjun huang on 2024/1/14.
//

#ifndef HELLO_LOGUTIL_H
#define HELLO_LOGUTIL_H

#include <android/log.h>
#include <jni.h>

#define LOG_TAG "HelloJNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif //HELLO_LOGUTIL_H
