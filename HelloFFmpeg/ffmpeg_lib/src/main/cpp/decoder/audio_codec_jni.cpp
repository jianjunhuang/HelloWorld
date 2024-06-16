#include <jni.h>
#include "libavcodec/avcodec.h"
#include "ffmpeg_audio_decoder.h"
#include "../android_log.h"

static jfieldID g_native_decoder_field_id;

JavaVM *javaVM;
extern "C"
__attribute__((unused)) JNIEXPORT void JNICALL
Java_xyz_juncat_ffmpeg_decoder_FFmpegAudioDecoder_nativeInit(JNIEnv *env, jobject thiz) {
    LOGD("FFmpegAudioDecoder init");
    FFmpegAudioDecoder *ffmpegAudioDecoder = new FFmpegAudioDecoder();
    ffmpegAudioDecoder->init(env, thiz);
    env->SetLongField(thiz, g_native_decoder_field_id, reinterpret_cast<jlong>(ffmpegAudioDecoder));
}

extern "C"
JNIEXPORT int JNICALL
Java_xyz_juncat_ffmpeg_decoder_FFmpegAudioDecoder_nativeDecodeWithPath(JNIEnv *env, jobject thiz,
                                                                       jstring path) {
    FFmpegAudioDecoder *ffmpegAudioDecoder = reinterpret_cast<FFmpegAudioDecoder *>(env->GetLongField(
            thiz,
            g_native_decoder_field_id));

    if (ffmpegAudioDecoder != nullptr) {
        return ffmpegAudioDecoder->decode(env, path);
    } else {
        LOGE("ffmpegAudioDecoder is null, could not decode");
    }
    return -1;
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_ffmpeg_decoder_FFmpegAudioDecoder_nativeRelease(JNIEnv *env, jobject thiz) {
    FFmpegAudioDecoder *ffmpegAudioDecoder = reinterpret_cast<FFmpegAudioDecoder *>(env->GetLongField(
            thiz,
            g_native_decoder_field_id));
    if (ffmpegAudioDecoder) {
        LOGD("nativeRelease audio_decoder ");
        ffmpegAudioDecoder->release(env);
        delete ffmpegAudioDecoder;
    }
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    LOGD("JNI_OnLoad in ");
    jclass clazz = env->FindClass("xyz/juncat/ffmpeg/decoder/FFmpegAudioDecoder");
    if (clazz != nullptr) {
        g_native_decoder_field_id = env->GetFieldID(clazz, "nativeDecoder", "J");
    }

    javaVM = vm;
    return JNI_VERSION_1_6;
}