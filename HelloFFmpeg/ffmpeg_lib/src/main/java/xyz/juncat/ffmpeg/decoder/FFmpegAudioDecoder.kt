package xyz.juncat.ffmpeg.decoder

import android.util.Log

class FFmpegAudioDecoder : AbsAudioDecoder() {

    private var nativeDecoder = 0L
    private var audioCallback: IAudioDecoder.OnDecoderCallback? = null

    fun onLog(level: Int, message: String) {
        when (level) {
            Log.DEBUG -> Log.d(TAG, "message: $message")
            Log.INFO -> Log.i(TAG, "message: $message")
            Log.WARN -> Log.w(TAG, "message: $message")
            Log.ERROR -> Log.e(TAG, "message: $message")
            else -> Log.d(TAG, "message: $message")
        }
    }

    fun onAudioData(audioData: ByteArray) {
        audioCallback?.onPCMDataAvailable(audioData)
    }

    fun onAudioFloatData(audioData: FloatArray) {

    }

    fun onStateChanged(state: Int) {
        Log.d(TAG, "onState() state: $state")
    }

    private external fun nativeInit()
    private external fun nativeDecodeWithPath(path: String): Int
    private external fun nativeRelease()

    companion object {
        private const val TAG = "FFmpegAudioDecoder"

        init {
            System.loadLibrary("native-codec")
        }
    }


    override fun stop() {
        nativeRelease()
    }

    override fun decode(path: String): Int {
        return nativeDecodeWithPath(path)
    }

    override fun setOnDecoderCallback(callback: IAudioDecoder.OnDecoderCallback?) {
        audioCallback = callback
    }

    override fun initialize() {
        nativeInit()
    }


}