//
// Created by jianjun huang on 2024/5/29.
//

#ifndef OSPROJECT_FFMPEG_AUDIO_EXTRACTOR_H
#define OSPROJECT_FFMPEG_AUDIO_EXTRACTOR_H



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

class FFmpegAudioExtractor {
public:
    FFmpegAudioExtractor();
    ~FFmpegAudioExtractor();

    int extract(JNIEnv* env, jstring videoPath, jstring audioPath);
    void release(JNIEnv* env);

private:
    std::atomic<bool> interrupt_flag;
};


#endif //OSPROJECT_FFMPEG_AUDIO_EXTRACTOR_H
