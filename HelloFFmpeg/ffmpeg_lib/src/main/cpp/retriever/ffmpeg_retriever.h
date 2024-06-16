//
// Created by jianjun huang on 2024/5/29.
//

#ifndef OSPROJECT_FFMPEG_RETRIEVER_H
#define OSPROJECT_FFMPEG_RETRIEVER_H


#include <jni.h>
#include <string>
#include <cstdint>
#include <functional>
#include <vector>
#include "android/bitmap.h"

extern "C" {
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
}

class FFmpegRetriever {
public:
    FFmpegRetriever();

    ~FFmpegRetriever();

    int init(JNIEnv *env);

    jobject getFrameAtTime(JNIEnv *env,jstring input_path, int width,int height,long timeUs);


    void release(JNIEnv *env);

private:
    std::atomic<bool> interrupt_flag;
    jobject createBitmap(JNIEnv *env, int width, int height);
};


#endif //OSPROJECT_FFMPEG_RETRIEVER_H
