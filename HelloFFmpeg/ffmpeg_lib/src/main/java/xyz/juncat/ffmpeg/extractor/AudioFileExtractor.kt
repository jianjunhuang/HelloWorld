package xyz.juncat.ffmpeg.extractor

import android.media.MediaExtractor
import android.media.MediaFormat
import android.text.TextUtils
import android.util.Log
import java.io.OutputStream

class AudioFileExtractor {

    private var videoPath: String? = null
    private var extractor: MediaExtractor? = null
    private var audioTracker = -1
    private var trackFormat: MediaFormat? = null
    private var fileWriter: OutputStream? = null

    private var nativeExtractor = 0L

    fun init(videoPath: String): Boolean {
        audioTracker = -1
        trackFormat = null
        if (TextUtils.isEmpty(videoPath)) {
            return false
        }
        try {
            this.videoPath = videoPath
            extractor = MediaExtractor()
            extractor?.setDataSource(videoPath)

            if (extractor == null) {
                return false
            }

            val numTracks = extractor?.trackCount ?: 0

            for (i in 0 until numTracks) {
                val format = extractor?.getTrackFormat(i)
                val mime = format?.getString(MediaFormat.KEY_MIME)
                if (mime?.startsWith("audio/") == true) {
                    audioTracker = i
                    trackFormat = format
                    extractor?.selectTrack(i)
                    break
                }
            }
            if (audioTracker >= 0) {
                nativeInit()
            }
            return audioTracker >= 0
        } catch (t: Throwable) {
            Log.e(TAG, "init failed: ", t)
            return false
        } finally {
            extractor?.release()
            extractor = null
        }
    }

    fun getMimeType(): String? {
        return trackFormat?.getString(MediaFormat.KEY_MIME)
    }

    fun getFileExtension(): String? {
        return getMimeType()?.let {
            when (it) {
                "audio/mp4a-latm" -> "aac"
                "audio/mpeg" -> "mp3"
                "audio/x-wma" -> "wma"
                "audio/x-wav" -> "wav"
                "audio/x-alac" -> "m4a"
                else -> it.substringAfter("/")
            }
        }
    }

    fun extract(audioFilePath: String): Boolean {
        if (audioTracker < 0) {
            return false
        }
        if (TextUtils.isEmpty(videoPath)) {
            return false
        }
        val rct = nativeExtract(videoPath ?: "", audioFilePath)
        return rct == 0
    }

    private external fun nativeInit()
    private external fun nativeExtract(videoFilePath: String, audioFilePath: String): Int
    private external fun nativeRelease()

    fun release() {
        try {
            Log.d(TAG, "release()")
            if (audioTracker >= 0) {
                nativeRelease()
            }
            audioTracker = -1
            trackFormat = null
            extractor?.release()
            if (fileWriter != null) {
                fileWriter?.flush()
                fileWriter?.close()
                fileWriter = null
            }
        } catch (t: Throwable) {
            Log.e(TAG, "release failed: ", t)
        } finally {
            videoPath = null
            extractor = null
            fileWriter = null
        }
    }

    companion object {
        private const val TAG = "AudioFileExtractor"

        init {
            System.loadLibrary("native-extractor")
        }
    }
}