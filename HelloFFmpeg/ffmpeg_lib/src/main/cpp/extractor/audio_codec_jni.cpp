#include <jni.h>
#include "libavcodec/avcodec.h"
#include "../android_log.h"
#include "ffmpeg_audio_extractor.h"

static jfieldID g_native_extractor_field_id;


JavaVM *javaVM;

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_ffmpeg_extractor_AudioFileExtractor_nativeInit(JNIEnv *env, jobject thiz) {
    LOGD("nativeInit audio_file_extractor");
    FFmpegAudioExtractor *ffmpegAudioExtractor = new FFmpegAudioExtractor();
    env->SetLongField(thiz, g_native_extractor_field_id,reinterpret_cast<jlong>(ffmpegAudioExtractor));
}

extern "C"
JNIEXPORT int JNICALL
Java_xyz_juncat_ffmpeg_extractor_AudioFileExtractor_nativeExtract(JNIEnv *env, jobject thiz,
                                                                  jstring video_path,
                                                                  jstring audio_path) {
    FFmpegAudioExtractor *ffmpegAudioExtractor = reinterpret_cast<FFmpegAudioExtractor *>(env->GetLongField(
            thiz,
            g_native_extractor_field_id));
    if (ffmpegAudioExtractor) {
        return ffmpegAudioExtractor->extract(env, video_path, audio_path);
    } else {
        LOGE("ffmpegAudioExtractor is null, could not extract");
    }
    return -1;
}

extern "C"
JNIEXPORT void JNICALL
Java_xyz_juncat_ffmpeg_extractor_AudioFileExtractor_nativeRelease(JNIEnv *env, jobject thiz) {
    FFmpegAudioExtractor *ffmpegAudioExtractor = reinterpret_cast<FFmpegAudioExtractor *>(env->GetLongField(
            thiz,
            g_native_extractor_field_id));
    LOGD("nativeRelease audio_file_extractor release");
    if (ffmpegAudioExtractor) {
        LOGD("nativeRelease audio_file_extractor release");
        ffmpegAudioExtractor->release(env);
        delete ffmpegAudioExtractor;
    }
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    LOGD("JNI_OnLoad in ");

    jclass extractor_clazz = env->FindClass("xyz/juncat/ffmpeg/extractor/AudioFileExtractor");
    if (extractor_clazz != nullptr) {
        g_native_extractor_field_id = env->GetFieldID(extractor_clazz, "nativeExtractor", "J");
    }
    javaVM = vm;
    return JNI_VERSION_1_6;
}