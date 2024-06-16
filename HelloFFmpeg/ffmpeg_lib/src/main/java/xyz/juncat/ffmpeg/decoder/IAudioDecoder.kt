package xyz.juncat.ffmpeg.decoder

interface IAudioDecoder {

    fun initialize()

    fun stop()

    fun decode(path: String): Int

    fun setOnDecoderCallback(callback: OnDecoderCallback?)

    interface OnDecoderCallback {
        fun onPCMDataAvailable(data: ByteArray)
    }
}