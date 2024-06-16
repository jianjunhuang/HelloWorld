// ffmpeg_audio_decoder.h

#ifndef FFMPEG_AUDIO_DECODER_H
#define FFMPEG_AUDIO_DECODER_H

#include <jni.h>
#include <string>
#include <cstdint>
#include <functional>
#include <vector>

extern "C" {
    #include "libavutil/opt.h"
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswresample/swresample.h"
}


// ALAC specific config https://github.com/FFmpeg/FFmpeg/blob/master/libavcodec/alac.c
typedef struct ALACSpecificConfig
{
    uint32_t    atomSize;
    uint32_t    tag;
    uint32_t    tagVesion;
    uint32_t	frameLength;
    uint8_t		compatibleVersion;
    uint8_t		bitDepth;
    uint8_t		pb;
    uint8_t		mb;
    uint8_t		kb;
    uint8_t		numChannels;
    uint16_t	maxRun;
    uint32_t	maxFrameBytes;
    uint32_t	avgBitRate;
    uint32_t	sampleRate;

} ALACSpecificConfig;

typedef struct {
    uint32_t magic_signature1;
    uint32_t magic_signature2;
    uint8_t version;
    uint8_t channels;
    uint16_t pre_skip;
    uint32_t sample_rate;
    int16_t gain_db;
    uint8_t stream_map;
} OpusHeader;

extern JavaVM *javaVM;

class FFmpegAudioDecoder {
public:
    FFmpegAudioDecoder();
    ~FFmpegAudioDecoder();

    void init(JNIEnv* env, jobject instance);
    int decode(JNIEnv* env, jstring path);
    void release(JNIEnv* env);

private:
    std::atomic<bool> interrupt_flag;
    // 声道布局
    const AVChannelLayout targetChannelLayout = AV_CHANNEL_LAYOUT_MONO;
    // 采样率
    const int targetSampleRate = 16000;
    // 目标位深
    const enum AVSampleFormat targetSampleFormat = AV_SAMPLE_FMT_S16;

    struct StateCallbackData {
        int state;
    };

    // 回调接口定义
    using LogCallback = std::function<void(int level, const std::string& message)>;
    using AudioDataCallback = std::function<void(JNIEnv* env, const std::vector<uint8_t>&)>;
    using AudioFloatDataCallback = std::function<void(JNIEnv* env, const std::vector<float>)>;
    using StateCallback = std::function<void(JNIEnv* env, const StateCallbackData&)>;

    // 回调函数成员变量
    AudioDataCallback audioDataCallback;
    AudioFloatDataCallback audioFloatDataCallback;
    StateCallback stateCallback;
    LogCallback logCallback;

    void initJavaCallback(JNIEnv* env, jobject instance);
    // 辅助方法
    void invokeLogCallback(int level, const char* format, ...);
    void invokeAudioDataCallback(JNIEnv* env, const std::vector<uint8_t>& data);
    void invokeStateCallback(JNIEnv* env, const StateCallbackData& data);




    // Java层回调函数相关
    jobject audioDecoderInstance;
};

#endif //FFMPEG_AUDIO_DECODER_H
