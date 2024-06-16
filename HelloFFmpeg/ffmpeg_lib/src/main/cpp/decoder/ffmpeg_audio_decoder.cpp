// audio_decoder.cpp

#include <sys/endian.h>
#include "ffmpeg_audio_decoder.h"
#include "../android_log.h"

FFmpegAudioDecoder::FFmpegAudioDecoder() {
    // 构造函数实现
    LOGD("FFmpegAudioDecoder::FFmpegAudioDecoder");
}

FFmpegAudioDecoder::~FFmpegAudioDecoder() {

    interrupt_flag.store(false);
    LOGD("FFmpegAudioDecoder::~FFmpegAudioDecoder");

}

void my_log_callback(void *ptr, int level, const char *fmt, va_list vl) {
    // 根据日志级别输出不同颜色的日志信息
    switch (level) {
        case AV_LOG_FATAL:
        case AV_LOG_ERROR:
            LOGD(fmt, vl);
            break;
        case AV_LOG_WARNING:
            LOGW(fmt, vl);
            break;
        case AV_LOG_INFO:
            LOGI(fmt, vl);
            break;
        case AV_LOG_DEBUG:
        case AV_LOG_TRACE:
            LOGD(fmt, vl);
            break;
        default:
            LOGD(fmt, vl);
            break;
    }
}

void FFmpegAudioDecoder::init(JNIEnv *env, jobject instance) {
    initJavaCallback(env, instance);
    //设置ffmpeg 日志回调
//    av_log_set_level(AV_LOG_DEBUG);
//    av_log_set_callback(my_log_callback);
}


void FFmpegAudioDecoder::initJavaCallback(JNIEnv *env, jobject instance) {
    // 初始化回调函数
    jclass audioDecoderClass = env->GetObjectClass(instance);
    jmethodID onLogMethodId = env->GetMethodID(audioDecoderClass, "onLog",
                                               "(ILjava/lang/String;)V");
    jmethodID onAudioDataMethodId = env->GetMethodID(audioDecoderClass, "onAudioData", "([B)V");
    jmethodID onStateChangedMethodId = env->GetMethodID(audioDecoderClass, "onStateChanged",
                                                        "(I)V");
    jmethodID onAudioFloatDataMethodId = env->GetMethodID(audioDecoderClass, "onAudioFloatData",
                                                          "([F)V");

    if (!onLogMethodId || !onAudioDataMethodId || !onStateChangedMethodId) {
        LOGE("Failed to get method id");
        env->DeleteLocalRef(audioDecoderClass);
        return;
    }

    audioDecoderInstance = env->NewGlobalRef(instance);
    logCallback = [this, onLogMethodId](int level, const std::string &message) {
        //获取对应线程的env
        JNIEnv *env = nullptr;
        if (javaVM->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            LOGE("Failed to get the environment using AttachCurrentThread()");
            return;
        }
        jstring messageStr = env->NewStringUTF(message.c_str());
        env->CallVoidMethod(audioDecoderInstance, onLogMethodId, level, messageStr);
        env->DeleteLocalRef(messageStr);
    };

    audioDataCallback = [this, onAudioDataMethodId](JNIEnv *env, const std::vector<uint8_t> &data) {
        if (audioDecoderInstance) {
            // 将 C++ 的 byte 数组转换为 Java 的 byte 数组
            jbyteArray byteArray = env->NewByteArray(data.size());
            env->SetByteArrayRegion(byteArray, 0, data.size(),
                                    reinterpret_cast<const jbyte *>(data.data()));

            // 调用 Java 层的回调方法
            env->CallVoidMethod(audioDecoderInstance, onAudioDataMethodId, byteArray);

            // 释放局部引用
            env->DeleteLocalRef(byteArray);
        }
    };

    audioFloatDataCallback = [this, onAudioFloatDataMethodId](JNIEnv *env,
                                                              const std::vector<float> &data) {
        // 将 C++ 的 float 数组转换为 Java 的 float 数组
        jfloatArray floatArray = env->NewFloatArray(data.size());
        env->SetFloatArrayRegion(floatArray, 0, data.size(),
                                 reinterpret_cast<const jfloat *>(data.data()));

        // 调用 Java 层的回调方法
        env->CallVoidMethod(audioDecoderInstance, onAudioFloatDataMethodId, floatArray);

        // 释放局部引用
        env->DeleteLocalRef(floatArray);
    };

    stateCallback = [this, onStateChangedMethodId](JNIEnv *env, const StateCallbackData &data) {
        env->CallVoidMethod(audioDecoderInstance, onStateChangedMethodId, data.state);
    };
}


void FFmpegAudioDecoder::release(JNIEnv *env) {
    LOGD("FFmpegAudioDecoder::release");
    interrupt_flag.store(true);
    if (audioDecoderInstance) {
        env->DeleteGlobalRef(audioDecoderInstance);
        audioDecoderInstance = nullptr;
        LOGD("FFmpegAudioDecoder::release1");
    }
    if (audioDataCallback) {
        audioDataCallback = nullptr;
        LOGD("FFmpegAudioDecoder::release2");
    }
}


void FFmpegAudioDecoder::invokeAudioDataCallback(JNIEnv *env, const std::vector<uint8_t> &data) {
    if (audioDataCallback) {
        audioDataCallback(env, data);
    }
}


void FFmpegAudioDecoder::invokeStateCallback(JNIEnv *env,
                                             const FFmpegAudioDecoder::StateCallbackData &data) {
    if (stateCallback) {
        stateCallback(env, data);
    }
}


void FFmpegAudioDecoder::invokeLogCallback(int level, const char *format, ...) {
//    if (logCallback) {
//        char buf[1024];
//        va_list args;
//        va_start(args, format);
//        vsprintf(buf, format, args);
//        va_end(args);
//        logCallback(level, buf);
//    }
}

int FFmpegAudioDecoder::decode(JNIEnv *env, jstring path) {

    interrupt_flag.store(false);
    LOGD("FFmpegConfig: %s", avcodec_configuration());
    LOGD("FFmpegVersion: %s", AV_STRINGIFY(LIBAVCODEC_VERSION));

    AVFormatContext *fmt_ctx = nullptr;
    AVCodecContext *codec_ctx = nullptr;
    AVPacket *packet = nullptr;
    AVFrame *frame = nullptr;
    const AVCodec *fcodec = nullptr;

    const char *pathStr = env->GetStringUTFChars(path, nullptr);

    int ret = avformat_open_input(&fmt_ctx, pathStr, nullptr, nullptr);
    if (ret < 0) {
        char errbuf[128];
        av_strerror(ret, errbuf, sizeof(errbuf));
        LOGE("Could not open source file %d, %s, %s\n", ret, pathStr, errbuf);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }
    LOGD("avformat_open_input success");

    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        LOGE("Could not find stream information\n");
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        avformat_free_context(fmt_ctx);
        return -1;
    }

    LOGD("avformat_find_stream_info success");
    int audio_stream_index = -1;
    AVStream *audio_stream = nullptr;
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            audio_stream = fmt_ctx->streams[i];
            break;
        }
    }

    LOGD("audio_stream_index: %d", audio_stream_index);
    if (audio_stream_index == -1) {
        LOGE("Could not find audio stream\n");
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }

    LOGD("audio_stream_index: %d", audio_stream_index);
    fcodec = avcodec_find_decoder(audio_stream->codecpar->codec_id);
    if (!fcodec) {
        LOGE("Could not find decoder\n");
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }
    LOGE("Decoder selected %s\n", fcodec->name);

    //分配解码器上下文
    codec_ctx = avcodec_alloc_context3(fcodec);
    if (!codec_ctx) {
        LOGE("Could not allocate codec context\n");
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }

    LOGD("codec_ctx: sample_fmt:%d, sample_rate:%d, channel:%d", codec_ctx->sample_fmt,
         codec_ctx->sample_rate, codec_ctx->ch_layout.nb_channels);
    //将流参数拷贝到解码器上下文
    if (avcodec_parameters_to_context(codec_ctx, audio_stream->codecpar) < 0) {
        LOGE("Could not copy codec parameters to context\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }

    codec_ctx->request_sample_fmt = AV_SAMPLE_FMT_S16;

    LOGD("codec_ctx: sample_fmt:%d, sample_rate:%d, channel:%d", codec_ctx->request_sample_fmt,
         codec_ctx->sample_rate, codec_ctx->ch_layout.nb_channels);
    //打开解码器
    if (avcodec_open2(codec_ctx, fcodec, nullptr) < 0) {
        LOGE("Could not open codec\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }

    LOGD("avcodec_open2 success");
    SwrContext *swr_ctx = nullptr;
    swr_ctx = swr_alloc();
    if (!swr_ctx) {
        LOGE("Failed to allocate SwrContext");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }

    av_opt_set_chlayout(swr_ctx, "in_chlayout", &codec_ctx->ch_layout, 0);
    av_opt_set_int(swr_ctx, "in_sample_rate", codec_ctx->sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", codec_ctx->sample_fmt, 0);

    LOGD("codec_ctx: sample_fmt:%d, sample_rate:%d, channel:%d", codec_ctx->sample_fmt,
         codec_ctx->sample_rate, codec_ctx->ch_layout.nb_channels);
    av_opt_set_chlayout(swr_ctx, "out_chlayout", &targetChannelLayout, 0);
    av_opt_set_int(swr_ctx, "out_sample_rate", targetSampleRate, 0);
    av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", targetSampleFormat, 0);

    if (swr_init(swr_ctx) < 0) {
        LOGE("Failed to initialize SwrContext");
        swr_free(&swr_ctx);
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }

    //分配AVPacket和AVFrame
    packet = av_packet_alloc();
    frame = av_frame_alloc();
    if (!packet || !frame) {
        LOGE("Could not allocate packet or frame\n");
        avcodec_free_context(&codec_ctx);
        avformat_close_input(&fmt_ctx);
        swr_free(&swr_ctx);
        env->ReleaseStringUTFChars(path, pathStr);
        return -1;
    }

    LOGD("FFmpegAudioDecoder::decode start");
    while (av_read_frame(fmt_ctx, packet) >= 0) {
        if (interrupt_flag.load()) {
            LOGW("Interrupted after reading frame");
            av_packet_unref(packet);
            break;
        }
        if (packet->stream_index == audio_stream_index) {
            if (avcodec_send_packet(codec_ctx, packet) < 0) {
                LOGE("Error sending a packet for decoding\n");
                break;
            }

            while (avcodec_receive_frame(codec_ctx, frame) >= 0) {
                if (interrupt_flag.load()) {
                    LOGW("Interrupted after receiving frame");
                    break;
                }
                int bytePerSample = av_get_bytes_per_sample(
                        static_cast<AVSampleFormat>(frame->format));
//                LOGD("FFmpegAudioDecoder::decode sample_fmt: %d nb_channel: %d samples: %d, bytePerSample: %d",
//                     frame->format, frame->ch_layout.nb_channels, frame->nb_samples, bytePerSample);

                int64_t delay = swr_get_delay(swr_ctx, codec_ctx->sample_rate);
                const int out_samples = av_rescale_rnd(delay + frame->nb_samples, targetSampleRate,
                                                       codec_ctx->sample_rate, AV_ROUND_UP);

                const int outputBufferSize = av_samples_get_buffer_size(nullptr,
                                                                        targetChannelLayout.nb_channels,
                                                                        out_samples,
                                                                        targetSampleFormat, 0);

                if (outputBufferSize < 0) {
                    LOGE("Failed to calculate output buffer size. size: %d", outputBufferSize);
                    swr_free(&swr_ctx);
                    avformat_close_input(&fmt_ctx);
                    avcodec_free_context(&codec_ctx);
                    env->ReleaseStringUTFChars(path, pathStr);
                    return -1;
                }
                if (interrupt_flag.load()) {
                    LOGW("Interrupted after calculating output buffer size");
                    break;
                }
                std::vector<uint8_t> resampledData(outputBufferSize);
                uint8_t *outData[1] = {resampledData.data()};
                int numSamplesConverted = swr_convert(swr_ctx, outData, out_samples,
                                                      const_cast<const uint8_t **>(frame->data),
                                                      frame->nb_samples);
                if (numSamplesConverted < 0) {
                    LOGE("Failed to convert audio samples");
                    swr_free(&swr_ctx);
                    avformat_close_input(&fmt_ctx);
                    avcodec_free_context(&codec_ctx);
                    env->ReleaseStringUTFChars(path, pathStr);
                    invokeLogCallback(ANDROID_LOG_ERROR, "Failed to convert audio samples");
                    return -1;
                }

                int actual_buf_size = numSamplesConverted * targetChannelLayout.nb_channels *
                                      av_get_bytes_per_sample(targetSampleFormat);
                std::vector<uint8_t> decodedData(resampledData.begin(),
                                                 resampledData.begin() + actual_buf_size);
                invokeAudioDataCallback(env, decodedData);

            }
        }
        av_packet_unref(packet);
        if (interrupt_flag.load()) {
            LOGW("Interrupted after processing frame");
            break;
        }
    }

    LOGD("FFmpegAudioDecoder::decode end");

    //释放资源
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);

    LOGD("FFmpegAudioDecoder::decode end");
    return 0;
}

