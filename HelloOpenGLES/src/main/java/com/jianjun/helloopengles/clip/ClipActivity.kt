package com.jianjun.helloopengles.clip

import android.content.res.AssetFileDescriptor
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.SurfaceTexture
import android.media.MediaPlayer
import android.opengl.GLES11Ext
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.opengl.GLUtils
import android.os.Bundle
import android.view.MotionEvent
import android.view.Surface
import android.view.SurfaceHolder
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.jianjun.helloopengles.R
import java.lang.Exception
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer
import java.nio.channels.FileLock
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10


class ClipActivity : AppCompatActivity() {

    private lateinit var glSurfaceView: DefGLSurfaceView
    private lateinit var drawer: IDrawer
    private var mediaPlayer: MediaPlayer? = null
    var lastX = 0f
    var lastY = 0f
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_clip)

        glSurfaceView = findViewById(R.id.gl_surface_view)

        drawer = getVideoDrawer()
//        drawer = getBitmapDrawer()
        //init render
        glSurfaceView.setEGLContextClientVersion(2)
        glSurfaceView.addDrawer(drawer)
        glSurfaceView.setRenderer(SimpleRender(drawer))

        findViewById<Button>(R.id.btn_rotate).setOnClickListener {
            drawer.rotate(-90f)
        }
        findViewById<Button>(R.id.btn_flip).setOnClickListener {
            drawer.flipHorizontal()
        }
    }

    private fun getBitmapDrawer(): IDrawer {
        val bmp = BitmapFactory.decodeResource(resources, R.drawable.pic)
        return BitmapDrawer(bmp).apply {
            setContentSize(bmp.width, bmp.height)
        }
    }

    private fun getVideoDrawer(): IDrawer {

        return VideoDrawer().apply {
            getSurfaceTexture {
                try {
                    mediaPlayer = MediaPlayer()
                    val afd: AssetFileDescriptor =
                        getResources().openRawResourceFd(R.raw.pixel) ?: return@getSurfaceTexture
                    mediaPlayer?.setDataSource(afd.fileDescriptor, afd.startOffset, afd.length)
                    val surface = Surface(it)
                    mediaPlayer?.setSurface(surface)
                    surface.release()
                    afd.close()
                    mediaPlayer?.isLooping = true
                    mediaPlayer?.prepare()
                    drawer.setContentSize(
                        mediaPlayer?.videoWidth ?: 0,
                        mediaPlayer?.videoHeight ?: 0
                    )
                    mediaPlayer?.start()
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }
        }

    }

    override fun onDestroy() {
        super.onDestroy()
        mediaPlayer?.release()
//        drawer.release()
    }

    class SimpleRender(private val drawer: IDrawer) : GLSurfaceView.Renderer {
        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            //清屏
            GLES20.glClearColor(0f, 0f, 0f, 0f)
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)
            val texture = IntArray(1)
            GLES20.glGenTextures(1, texture, 0) //生成纹理
            drawer.setTextureID(texture[0])
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            //设置绘制区域宽高
            GLES20.glViewport(0, 0, width, height)
            drawer.setWorldSize(width, height)
        }

        override fun onDrawFrame(gl: GL10?) {
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT or GLES20.GL_DEPTH_BUFFER_BIT)
            drawer.draw()
        }

    }

}