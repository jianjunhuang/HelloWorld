#include <jni.h>
#include "../android_log.h"
#include <string>

static jfieldID g_native_retriever_field_id;


JavaVM *javaVM;

extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libswscale/swscale.h"
    #include "libavformat/avformat.h"
    #include "libswresample/swresample.h"
    #include "libyuv.h"
}

extern "C"
JNIEXPORT jstring JNICALL
Java_xyz_juncat_ffmpeg_info_FFmpegInfo_getFFmpegVersion(JNIEnv *env, jobject thiz) {

    const char *config = avcodec_configuration();
    const char *version = AV_STRINGIFY(LIBAVCODEC_VERSION);
    std::string result = "FFmpeg version: \n";
    result += "codec:";
    result +=  AV_STRINGIFY(LIBAVCODEC_VERSION);
    result += "\n";
    result += "util:";
    result +=  AV_STRINGIFY(LIBAVUTIL_VERSION);
    result += "\n";
    result += "swscale:";
    result +=  AV_STRINGIFY(LIBSWSCALE_VERSION);
    result += "\n";
    result += "format:";
    result +=  AV_STRINGIFY(LIBAVFORMAT_VERSION);
    result += "\n";
    result += "swresample:";
    result +=  AV_STRINGIFY(LIBSWRESAMPLE_VERSION);
    result += "\n";
    result += "FFmpeg config: ";
    result += config;

    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    LOGD("JNI_OnLoad in ");

    jclass clazz = env->FindClass("xyz/juncat/ffmpeg/info/FFmpegInfo");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    g_native_retriever_field_id = env->GetFieldID(clazz, "mNativeContext", "J");
    javaVM = vm;
    return JNI_VERSION_1_6;
}
