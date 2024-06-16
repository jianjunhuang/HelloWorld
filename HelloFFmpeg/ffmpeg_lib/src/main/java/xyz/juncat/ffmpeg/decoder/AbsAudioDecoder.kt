package xyz.juncat.ffmpeg.decoder

import android.os.Handler
import android.os.HandlerThread
import android.os.Message
import android.os.Process
import android.os.SystemClock

abstract class AbsAudioDecoder : IAudioDecoder {

    protected val mHandlerThread = HandlerThread(
        "FFmpegAudioDecoder--${SystemClock.elapsedRealtime()}",
        Process.THREAD_PRIORITY_AUDIO
    )

    protected val mDecoderHandler: Handler

    init {
        mHandlerThread.start()
        mDecoderHandler = object : Handler(mHandlerThread.looper) {
            override fun handleMessage(msg: Message) {
                handleInternalMessage(msg)
            }
        }
    }

    protected open fun handleInternalMessage(msg: Message) {

    }

}