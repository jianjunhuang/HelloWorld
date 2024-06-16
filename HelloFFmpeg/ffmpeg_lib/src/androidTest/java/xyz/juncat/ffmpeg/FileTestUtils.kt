package xyz.juncat.ffmpeg

import android.content.Context
import java.io.File

object FileTestUtils {

    private const val TAG = "FileTestUtils"
    private const val TEST_FILE = "NowInAndroid.mp4"

    fun getTestMp4(context: Context): String {
        val externalMp4 = File(context.externalCacheDir?.absolutePath, "test_src.mp4")
        if (externalMp4.exists()) {
            return externalMp4.absolutePath
        }
        context.assets.open(TEST_FILE).use { input ->
            externalMp4.outputStream().use { output ->
                input.copyTo(output)
            }
        }
        return externalMp4.absolutePath
    }

    fun getOutputFile(appContext: Context?, name: String): File {
        return File(appContext?.externalCacheDir?.absolutePath, name)
    }
}