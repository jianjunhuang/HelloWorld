package xyz.juncat.ffmpeg.info

import android.util.Log
import org.junit.Assert.*

import org.junit.Test

class FFmpegInfoTest {

    @Test
    fun getFFmpegVersion() {
        val ffmpegInfo = FFmpegInfo()
        val version = ffmpegInfo.getFFmpegVersion()
        Log.i(TAG, "getFFmpegVersion: $version")
        assertNotNull(version)
        assertTrue(version.isNotEmpty())
    }

    companion object {
        private const val TAG = "FFmpegInfoTest"
    }
}