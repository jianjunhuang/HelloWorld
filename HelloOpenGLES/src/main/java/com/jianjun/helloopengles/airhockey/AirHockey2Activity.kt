package com.jianjun.helloopengles.airhockey

import android.content.Context
import android.graphics.Color
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import android.opengl.Matrix
import android.os.Bundle
import android.text.TextUtils
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.jianjun.helloopengles.R
import com.jianjun.helloopengles.airhockey.objects.Mallet
import com.jianjun.helloopengles.airhockey.objects.Table
import com.jianjun.helloopengles.program.ColorShaderProgram
import com.jianjun.helloopengles.program.TextureShaderProgram
import com.jianjun.helloopengles.utils.ResourceUtils
import com.jianjun.helloopengles.utils.ShaderUtils
import com.jianjun.helloopengles.utils.TextureUtils
import java.nio.ByteBuffer
import java.nio.ByteOrder
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * with texture
 */
class AirHockey2Activity : AppCompatActivity() {

    private lateinit var glSurfaceView: GLSurfaceView
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        glSurfaceView = GLSurfaceView(this)
        glSurfaceView.setEGLContextClientVersion(2)
        glSurfaceView.setRenderer(AirHockeyRender(this))
        glSurfaceView.renderMode = GLSurfaceView.RENDERMODE_WHEN_DIRTY
        setContentView(glSurfaceView)
    }

    class AirHockeyRender(private val context: Context) : GLSurfaceView.Renderer {

        private val projectMatrix = FloatArray(16)
        private val modelMatrix = FloatArray(16)

        /*
        ↓ make sure the code created on onSurfaceCreated method
         */
        private var table: Table? = null
        private var mallet: Mallet? = null
        private var textureShaderProgram: TextureShaderProgram? = null
        private var colorShaderProgram: ColorShaderProgram? = null

        private var texture = 0

        override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
            GLES20.glClearColor(0f, 0f, 0f, 0f)
            table = Table()
            mallet = Mallet()
            textureShaderProgram = TextureShaderProgram(context)
            colorShaderProgram = ColorShaderProgram(context)
            texture = TextureUtils.loadTexture(context, R.drawable.air_hockey_surface)
        }

        override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
            GLES20.glViewport(0, 0, width, height)
            //创建 45度的视野创建透视投影， z 为 -1 开始，-10 结束
            Matrix.perspectiveM(projectMatrix, 0, 45f, width / height.toFloat(), 1f, 10f)

            //move table z -2
            Matrix.setIdentityM(modelMatrix, 0)
            Matrix.translateM(modelMatrix, 0, 0f, 0f, -2f)

            //增加旋转
            Matrix.translateM(modelMatrix, 0, 0f, 0f, -2f)
            Matrix.rotateM(modelMatrix, 0, -60f, 1f, 0f, 0f)

            val tempFloatArray = FloatArray(16)
            Matrix.multiplyMM(tempFloatArray, 0, projectMatrix, 0, modelMatrix, 0)
            System.arraycopy(tempFloatArray, 0, projectMatrix, 0, tempFloatArray.size)
        }

        override fun onDrawFrame(gl: GL10?) {
            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT)

            textureShaderProgram?.let {
                it.useProgram()
                it.setUniforms(projectMatrix, texture)
                table?.bindData(it)
                table?.draw()
            }

            colorShaderProgram?.let {
                it.useProgram()
                it.setUniforms(projectMatrix)
                mallet?.bindData(it)
                mallet?.draw()
            }
        }
    }

}

