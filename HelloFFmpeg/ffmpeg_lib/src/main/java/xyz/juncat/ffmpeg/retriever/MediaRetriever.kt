package xyz.juncat.ffmpeg.retriever

import android.graphics.Bitmap
import android.text.TextUtils
import android.util.Log

class MediaRetriever {

    private var mNativeContext: Long = 0
    private var mPath = ""

    fun setDataSource(path: String) {
        if (mPath == path) {
            return
        }
        nativeInit()
        this.mPath = path
    }

    private external fun nativeInit(): Int
    private external fun nativeRelease(): Int
    private external fun getFrameAtTime(
        path: String,
        width: Int,
        height: Int,
        timeUs: Long
    ): Bitmap?

    fun getScaledFrameAtTime(width: Int, height: Int, timeUs: Long): Bitmap? {
        if (TextUtils.isEmpty(mPath)) {
            Log.e(TAG, "getScaledFrameAtTime: mPath is empty")
            return null
        }
        return getFrameAtTime(mPath, width, height, timeUs)
    }

    fun release() {
        nativeRelease()
    }

    companion object {
        private const val TAG = "MediaRetriever"

        init {
            System.loadLibrary("native-retriever")
        }
    }
}
