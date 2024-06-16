#include <jni.h>
#include "libavcodec/avcodec.h"
#include "../android_log.h"
#include "ffmpeg_retriever.h"

static jfieldID g_native_retriever_field_id;


JavaVM *javaVM;

extern "C"
JNIEXPORT int JNICALL
Java_xyz_juncat_ffmpeg_retriever_MediaRetriever_nativeInit(JNIEnv *env, jobject thiz) {
    FFmpegRetriever *retriever = new FFmpegRetriever();
    env->SetLongField(thiz, g_native_retriever_field_id, reinterpret_cast<jlong>(retriever));
    return 0;
}


extern "C"
JNIEXPORT int JNICALL
Java_xyz_juncat_ffmpeg_retriever_MediaRetriever_nativeRelease(JNIEnv *env, jobject thiz) {
    FFmpegRetriever *retriever = reinterpret_cast<FFmpegRetriever *>(env->GetLongField(thiz,
                                                                                       g_native_retriever_field_id));
    if (!retriever) {
        LOGD("FFmpegRetriever is null");
        return -1;
    }
    retriever->release(env);
    delete retriever;
    return 0;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_xyz_juncat_ffmpeg_retriever_MediaRetriever_getFrameAtTime(JNIEnv *env, jobject thiz,
                                                               jstring path, jint width,
                                                               jint height,
                                                               jlong timeUs) {
    FFmpegRetriever *retriever = reinterpret_cast<FFmpegRetriever *>(env->GetLongField(thiz,
                                                                                       g_native_retriever_field_id));
    if (!retriever) {
        LOGD("FFmpegRetriever is null");
        return nullptr;
    }
    return retriever->getFrameAtTime(env, path, width, height, timeUs);
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    LOGD("JNI_OnLoad in ");

    jclass clazz = env->FindClass("xyz/juncat/ffmpeg/retriever/MediaRetriever");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    g_native_retriever_field_id = env->GetFieldID(clazz, "mNativeContext", "J");
    javaVM = vm;
    return JNI_VERSION_1_6;
}