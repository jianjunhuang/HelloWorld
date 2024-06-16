package xyz.juncat.ffmpeg.retriever

import android.graphics.Bitmap
import androidx.test.platform.app.InstrumentationRegistry
import org.junit.Assert.*

import org.junit.Test
import xyz.juncat.ffmpeg.FileTestUtils
import java.io.FileOutputStream

class MediaRetrieverTest {

    @Test
    fun getScaledFrameAtTime() {
        val mediaRetriever = MediaRetriever()
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        val testMp4 = FileTestUtils.getTestMp4(appContext)
        mediaRetriever.setDataSource(testMp4)
        val bmp = mediaRetriever.getScaledFrameAtTime(1280, 720, 0)
        assertNotNull(bmp)
        assertTrue(bmp?.width == 1280)
        assertTrue(bmp?.height == 720)
        val output = FileOutputStream(FileTestUtils.getOutputFile(appContext, "test_frame.png"))
        bmp?.compress(Bitmap.CompressFormat.PNG, 100, output)
        mediaRetriever.release()
    }
}