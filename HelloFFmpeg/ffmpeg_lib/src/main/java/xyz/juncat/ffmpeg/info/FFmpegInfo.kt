package xyz.juncat.ffmpeg.info

class FFmpegInfo {

    private var mNativeContext = 0L

    external fun getFFmpegVersion(): String

    companion object {
        init {
            System.loadLibrary("native-info")
        }
    }


}