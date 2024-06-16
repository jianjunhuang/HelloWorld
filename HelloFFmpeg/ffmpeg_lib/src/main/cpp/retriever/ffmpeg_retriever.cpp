//
// Created by jianjun huang on 2024/5/29.
//

#include "ffmpeg_retriever.h"
#include <sys/endian.h>
#include "../android_log.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libyuv/scale.h"
#include "libyuv/convert_from.h"
#include "jni.h"
#include "android/bitmap.h"

FFmpegRetriever::FFmpegRetriever() {
    //构造函数
    LOGD("FFmpegRetriever::FFmpegRetriever");
}

FFmpegRetriever::~FFmpegRetriever() {
    interrupt_flag.store(false);
    LOGD("FFmpegRetriever::~FFmpegRetriever");
}

int FFmpegRetriever::init(JNIEnv *env) {
    LOGD("FFmpegRetriever::init");
    interrupt_flag.store(false);
    LOGD("FFmpegConfig: %s", avcodec_configuration());
    LOGD("FFmpegVersion: %s", AV_STRINGIFY(LIBAVCODEC_VERSION));
    return 0;
}

jobject FFmpegRetriever::getFrameAtTime(JNIEnv *env, jstring input_path, int width, int height,
                                        long timeUs) {
    const char *inputFilePath = env->GetStringUTFChars(input_path, 0);

    AVFormatContext *formatCtx = nullptr;
    AVCodecContext *codecCtx = nullptr;
    AVFrame *frame = nullptr, *scaledFrame = nullptr;
    AVPacket packet;
    SwsContext *swsCtx = nullptr;
    int videoStreamIndex = -1;

    // 打开输入文件并获取格式上下文
    if (avformat_open_input(&formatCtx, inputFilePath, nullptr, nullptr) != 0) {
        LOGE("can not open '%s'", inputFilePath);
        env->ReleaseStringUTFChars(input_path, inputFilePath);
        return nullptr;
    }

    LOGD("open input file success");

    // 检索流信息
    if (avformat_find_stream_info(formatCtx, nullptr) < 0) {
        LOGE("cant find stream information");
        env->ReleaseStringUTFChars(input_path, inputFilePath);
        return nullptr;
    }

    // 查找第一个视频流
    for (int i = 0; i < formatCtx->nb_streams; i++) {
        if (formatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }
    if (videoStreamIndex == -1) {
        LOGE("cant find video stream");
        env->ReleaseStringUTFChars(input_path, inputFilePath);
        return nullptr;
    }

    // 获取编解码器参数并查找解码器
    AVCodecParameters *codecPar = formatCtx->streams[videoStreamIndex]->codecpar;
    const AVCodec *codec = avcodec_find_decoder(codecPar->codec_id);
    if (!codec) {
        LOGE("unsupported codec!");
        return nullptr;
    }

    // 分配编解码器上下文
    codecCtx = avcodec_alloc_context3(codec);
    if (!codecCtx) {
        LOGE("cant allocate codec context");
        return nullptr;
    }
    if (avcodec_parameters_to_context(codecCtx, codecPar) < 0) {
        LOGE("cant copy codec parameters to context");
        return nullptr;
    }

    // 打开编解码器
    if (avcodec_open2(codecCtx, codec, nullptr) < 0) {
        LOGE("cant open codec");
        return nullptr;
    }

    // 分配帧
    frame = av_frame_alloc();
    scaledFrame = av_frame_alloc();
    if (!frame || !scaledFrame) {
        LOGE("cant allocate frame");
        return nullptr;
    }

    LOGD("width: %d, height: %d", width, height);


    if (timeUs > -1) {
        // 跳转到指定的时间戳
        int64_t seekTarget = av_rescale_q(timeUs, AV_TIME_BASE_Q,
                                          formatCtx->streams[videoStreamIndex]->time_base);
        int64_t duration = formatCtx->streams[videoStreamIndex]->duration;
        if (seekTarget > 0 && seekTarget > duration) {
            LOGE("seek target is greater than duration");
            seekTarget = duration;
        }
        LOGD("duration: %ld, seek target is %ld", duration, seekTarget);
        if (av_seek_frame(formatCtx, videoStreamIndex, seekTarget, AVSEEK_FLAG_BACKWARD) < 0) {
            LOGE("cant seek frame, timeUs: %ld", timeUs);
            return nullptr;
        }
    }

    scaledFrame->width = width;
    scaledFrame->height = height;
    scaledFrame->format = AV_PIX_FMT_YUV420P;

    // 分配缩放后的帧缓冲区
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, width, height, 1);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(scaledFrame->data, scaledFrame->linesize, buffer, AV_PIX_FMT_YUV420P,
                         width, height, 1);

    // 初始化SWS上下文用于图像缩放
    swsCtx = sws_getContext(codecCtx->width, codecCtx->height, codecCtx->pix_fmt,
                            width, height, AV_PIX_FMT_YUV420P,
                            SWS_BILINEAR, nullptr, nullptr, nullptr);

    LOGD("seek frame success");

    while (av_read_frame(formatCtx, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            avcodec_send_packet(codecCtx, &packet);
            while (avcodec_receive_frame(codecCtx, frame) >= 0) {

                LOGD("frame width: %d, height: %d", frame->width, frame->height);
                // 使用SWS进行图像缩放
                sws_scale(swsCtx, frame->data, frame->linesize, 0, codecCtx->height,
                          scaledFrame->data, scaledFrame->linesize);

                if (!scaledFrame) {
                    LOGE("cant allocate scaled frame");
                    return nullptr;
                }
                jobject bmp;
                bmp = createBitmap(env, scaledFrame->width, scaledFrame->height);
                if (bmp == nullptr) {
                    LOGE("create bitmap failed");
                    return nullptr;
                }
                void *addr_pixels;
                int rct = AndroidBitmap_lockPixels(env, bmp, &addr_pixels);
                if (rct < 0) {
                    LOGE("lock pixels failed");
                    return nullptr;
                }


                LOGD("scaledFrame width: %d, height: %d", scaledFrame->width, scaledFrame->height);
                if (scaledFrame->format != AV_PIX_FMT_YUV420P) {
                    LOGE("unsupported pixel format");
                    return nullptr;
                }
                // 使用libyuv将YUV转换为ARGB
                libyuv::I420ToABGR(scaledFrame->data[0], scaledFrame->linesize[0],
                                   scaledFrame->data[1], scaledFrame->linesize[1],
                                   scaledFrame->data[2], scaledFrame->linesize[2],
                                   (uint8_t *) addr_pixels, scaledFrame->width * 4,
                                   scaledFrame->width, scaledFrame->height);

                AndroidBitmap_unlockPixels(env, bmp);


                av_packet_unref(&packet);
                av_frame_free(&frame);
                av_frame_free(&scaledFrame);

                return bmp;
            }
            av_packet_unref(&packet);
        }
    }

    // 释放已分配的内存
    av_frame_free(&frame);
    av_frame_free(&scaledFrame);
    avcodec_free_context(&codecCtx);
    avformat_close_input(&formatCtx);

    env->ReleaseStringUTFChars(input_path, inputFilePath);

    return nullptr;
}

void FFmpegRetriever::release(JNIEnv *env) {
    LOGD("FFmpegRetriever::release");
    interrupt_flag.store(true);
}

jobject FFmpegRetriever::createBitmap(JNIEnv *env, int width, int height) {

    jclass bitmapCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapFunction = env->GetStaticMethodID(bitmapCls,
                                                            "createBitmap",
                                                            "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jstring configName = env->NewStringUTF("ARGB_8888");
    jclass bitmapConfigClass = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID valueOfBitmapConfigFunction = env->GetStaticMethodID(bitmapConfigClass,
                                                                   "valueOf",
                                                                   "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");

    jobject bitmapConfig = env->CallStaticObjectMethod(bitmapConfigClass,
                                                       valueOfBitmapConfigFunction,
                                                       configName);

    jobject newBitmap = env->CallStaticObjectMethod(bitmapCls,
                                                    createBitmapFunction,
                                                    width, height,
                                                    bitmapConfig);

    return newBitmap;
}

