//
// Created by jianjun huang on 2024/5/29.
//

#include "ffmpeg_audio_extractor.h"
#include <sys/endian.h>
#include "../android_log.h"

FFmpegAudioExtractor::FFmpegAudioExtractor() {
    //构造函数
    LOGD("FFmpegAudioExtractor::FFmpegAudioExtractor");
}

FFmpegAudioExtractor::~FFmpegAudioExtractor() {
    interrupt_flag.store(false);
    LOGD("FFmpegAudioExtractor::~FFmpegAudioExtractor");
}

int FFmpegAudioExtractor::extract(JNIEnv *env, jstring videoPath, jstring audioPath) {

    interrupt_flag.store(false);
    LOGD("FFmpegConfig: %s", avcodec_configuration());
    LOGD("FFmpegVersion: %s", AV_STRINGIFY(LIBAVCODEC_VERSION));

    const char *video_path = env->GetStringUTFChars(videoPath, nullptr);
    const char *audio_path = env->GetStringUTFChars(audioPath, nullptr);

    LOGD("FFmpegAudioExtractor::extract video_path: %s, audio_path: %s", video_path, audio_path);

    AVFormatContext *fmt_ctx = nullptr;
    if (avformat_open_input(&fmt_ctx, video_path, nullptr, nullptr) < 0) {
        LOGE("avformat_open_input failed");
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }
    LOGD("avformat_open_input success");
    //查找流信息
    if (avformat_find_stream_info(fmt_ctx, nullptr) < 0) {
        LOGE("avformat_find_stream_info failed");
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }

    LOGD("avformat_find_stream_info success");
    int audio_stream_index = -1;
    for (int i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audio_stream_index = i;
            break;
        }
    }

    LOGD("audio_stream_index: %d", audio_stream_index);
    if (audio_stream_index == -1) {
        LOGE("audio_stream_index == -1");
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }

    LOGD("audio_stream_index: %d", audio_stream_index);
    AVFormatContext *out_fmt_ctx = nullptr;
    out_fmt_ctx = avformat_alloc_context();
    //根据目标文件名获取最佳容器
    const AVOutputFormat *out_fmt = nullptr;
    out_fmt = av_guess_format(nullptr, audio_path, nullptr);
    if (!out_fmt) {
        LOGE("av_guess_format failed");
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }

    LOGD("out_fmt: %s", out_fmt->name);

    out_fmt_ctx->oformat = out_fmt;
    if (!out_fmt_ctx) {
        LOGE("avformat_alloc_output_context2 failed");
        avformat_close_input(&fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }
    LOGD("avformat_alloc_output_context2 success");

    AVStream *input_stream = fmt_ctx->streams[audio_stream_index];
    if (!input_stream) {
        LOGE("input_stream is null");
        avformat_close_input(&fmt_ctx);
        avformat_free_context(out_fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }

    LOGD("input_stream->codecpar->codec_id: %d", input_stream->codecpar->codec_id);
    AVStream *out_stream = avformat_new_stream(out_fmt_ctx, nullptr);
    if (!out_stream) {
        LOGE("avformat_new_stream failed");
        avformat_close_input(&fmt_ctx);
        avformat_free_context(out_fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }

    LOGD("avformat_new_stream success");
    int ret = avcodec_parameters_copy(out_stream->codecpar, input_stream->codecpar);
    if (ret < 0) {
        LOGE("avcodec_parameters_copy failed");
        avformat_close_input(&fmt_ctx);
        avformat_free_context(out_fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }
    LOGD("avcodec_parameters_copy success");
    ret = avio_open(&out_fmt_ctx->pb, audio_path, AVIO_FLAG_WRITE);
    if (ret < 0) {
        char errbuf[128];
        av_strerror(ret, errbuf, sizeof(errbuf));
        LOGE("avio_open failed, error: %s", errbuf);
        avformat_close_input(&fmt_ctx);
        avformat_free_context(out_fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }

    LOGD("avio_open success");
    if (avformat_write_header(out_fmt_ctx, nullptr) < 0) {
        LOGE("avformat_write_header failed");
        avformat_close_input(&fmt_ctx);
        avformat_free_context(out_fmt_ctx);
        env->ReleaseStringUTFChars(videoPath, video_path);
        env->ReleaseStringUTFChars(audioPath, audio_path);
        return -1;
    }

    LOGD("avformat_write_header success");
    AVPacket packet;
    while (av_read_frame(fmt_ctx, &packet) >= 0) {
        if (interrupt_flag.load()) {
            LOGD("interrupt_flag.load() == true");
            av_packet_unref(&packet);
            break;
        }
        if (packet.stream_index == audio_stream_index) {
            //时间基计算，音频pts和dts一致
            packet.pts = av_rescale_q_rnd(packet.pts, input_stream->time_base, out_stream->time_base, AV_ROUND_UP);
            packet.dts = packet.pts;
            packet.duration = av_rescale_q(packet.duration, input_stream->time_base, out_stream->time_base);
            packet.pos = -1;
            packet.stream_index = 0;
            //将包写到输出媒体文件
            av_interleaved_write_frame(out_fmt_ctx, &packet);
        }
        //减少引用计数，避免内存泄漏
        av_packet_unref(&packet);
    }
    LOGD("av_read_frame end");

    av_write_trailer(out_fmt_ctx);
    avio_close(out_fmt_ctx->pb);

    avformat_close_input(&fmt_ctx);
    avformat_free_context(out_fmt_ctx);

    env->ReleaseStringUTFChars(videoPath, video_path);
    env->ReleaseStringUTFChars(audioPath, audio_path);
    LOGD("FFmpegAudioExtractor::extract end");
    return 0;
}

void FFmpegAudioExtractor::release(JNIEnv *env) {
    LOGD("FFmpegAudioExtractor::release");
    interrupt_flag.store(true);
}