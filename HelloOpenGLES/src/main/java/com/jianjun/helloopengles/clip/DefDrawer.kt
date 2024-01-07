package com.jianjun.helloopengles.clip

import android.graphics.SurfaceTexture
import android.opengl.GLES20
import android.opengl.GLES32
import android.opengl.Matrix
import java.nio.ByteBuffer
import java.nio.ByteOrder
import java.nio.FloatBuffer

abstract class DefDrawer : IDrawer {
    private val sprite2d = Sprite2d()

    protected var surfaceTexture: SurfaceTexture? = null
    private var callback: ((SurfaceTexture?) -> Unit)? = null

    //顶点坐标
    private val vertexCoors = floatArrayOf(
        -1f, -1f,
        1f, -1f,
        -1f, 1f,
        1f, 1f
    )

    //纹理坐标
    private val textureCoors = floatArrayOf(
        0f, 1f,
        1f, 1f,
        0f, 0f,
        1f, 0f
    )

    //纹理ID
    protected var textureId: Int = -1

    //新的纹理接收着
    protected var textureHandler = -1

    //OpenGL程序ID
    private var program: Int = -1

    // 顶点坐标接收者
    private var vertexPosHandler: Int = -1

    // 纹理坐标接收者
    private var texturePosHandler: Int = -1

    private var vertexMatrixHandler: Int = -1
    private lateinit var vertexBuffer: FloatBuffer
    private lateinit var textureBuffer: FloatBuffer

    private var contentWidth = -1
    private var contentHeight = -1

    private var worldWidth = -1
    private var worldHeight = -1

    private var scale = 1f
    private var posX = 0f
    private var posY = 0f
    private var rotation = 0f

    init {
        //初始化顶点坐标
        initPos()
    }

    private fun initPos() {
        val bb = ByteBuffer.allocateDirect(vertexCoors.size * 4)
        bb.order(ByteOrder.nativeOrder())
        //将坐标数据转换为FloatBuffer，用以传入给OpenGL ES程序
        vertexBuffer = bb.asFloatBuffer()
        vertexBuffer.put(vertexCoors)
        vertexBuffer.position(0)

        val cc = ByteBuffer.allocateDirect(textureCoors.size * 4)
        cc.order(ByteOrder.nativeOrder())
        textureBuffer = cc.asFloatBuffer()
        textureBuffer.put(textureCoors)
        textureBuffer.position(0)
    }

    override fun draw() {
        if (textureId != -1) {
            initScale()
            //创建、编译并启动OpenGL着色器
            createGLPrg()
            //激活，绑定纹理单元
            activateTexture()
            bindTexture()
            //开始渲染绘制
            doDraw()
        }
    }

    abstract fun bindTexture()

    private var isInitScale = false
    private fun initScale() {
        if (isInitScale) return
        if (contentWidth == -1 || contentHeight == -1 ||
            worldWidth == -1 || worldHeight == -1
        ) return

        isInitScale = true
        scale = if (contentWidth > contentHeight) {
            worldWidth / contentWidth.toFloat()
        } else {
            worldHeight / contentHeight.toFloat()
        }
        Matrix.orthoM(
            displayProjectionMatrix,
            0,
            0f,
            worldWidth.toFloat(),
            0f,
            worldHeight.toFloat(),
            -1f,
            1f
        )
        posX = worldWidth / 2f
        posY = worldHeight / 2f
        update()
    }

    override fun setTextureID(id: Int) {
        textureId = id
        surfaceTexture = SurfaceTexture(id)
        callback?.invoke(surfaceTexture)
    }

    override fun release() {
        GLES20.glDisableVertexAttribArray(vertexPosHandler)
        GLES20.glDisableVertexAttribArray(texturePosHandler)
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0)
        GLES20.glDeleteTextures(1, intArrayOf(textureId), 0)
        GLES20.glDeleteProgram(program)
    }

    override fun getSurfaceTexture(callback: (SurfaceTexture?) -> Unit) {
        this.callback = callback
    }

    override fun setContentSize(width: Int, height: Int) {
        contentWidth = width
        contentHeight = height
    }

    override fun setWorldSize(worldW: Int, worldH: Int) {
        worldWidth = worldW
        worldHeight = worldH
    }

    protected open fun activateTexture() {

    }

    private fun createGLPrg() {
        if (program == -1) {
            val vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, getVertexShader())
            val fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, getFragmentShader())

            //创建OpenGL ES程序，注意：需要在OpenGL渲染线程中创建，否则无法渲染
            program = GLES20.glCreateProgram()
            //将顶点着色器加入到程序
            GLES20.glAttachShader(program, vertexShader)
            //将片元着色器加入到程序中
            GLES20.glAttachShader(program, fragmentShader)
            //连接到着色器程序
            GLES20.glLinkProgram(program)

            vertexMatrixHandler = GLES20.glGetUniformLocation(program, "uMatrix")
            //Java和GLSL交互的通道，通过属性可以给GLSL设置相关的值。
            vertexPosHandler = GLES20.glGetAttribLocation(program, "aPosition")
            texturePosHandler = GLES20.glGetAttribLocation(program, "aCoordinate")
        }
        //使用OpenGL程序
        GLES20.glUseProgram(program)
    }

    private val displayProjectionMatrix = FloatArray(16)
    private fun doDraw() {
        sprite2d.draw(displayProjectionMatrix)
        //启用顶点的句柄
        GLES20.glEnableVertexAttribArray(vertexPosHandler)
        GLES20.glEnableVertexAttribArray(texturePosHandler)
        GLES20.glUniformMatrix4fv(vertexMatrixHandler, 1, false, sprite2d.scratchMatrix, 0)
        //设置着色器参数
        GLES20.glVertexAttribPointer(
            vertexPosHandler,
            2,
            GLES20.GL_FLOAT,
            false,
            0,
            vertexBuffer
        )
        GLES20.glVertexAttribPointer(
            texturePosHandler,
            2,
            GLES20.GL_FLOAT,
            false,
            0,
            textureBuffer
        )
        //开始绘制
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4)
    }

    override fun move(x: Float, y: Float) {
        posX = x
        posY = worldHeight - y
        update()
    }

    override fun scale(scale: Float, focusX: Float, focusY: Float) {
        this.scale *= scale
        update()
    }

    override fun rotate(degrees: Float) {
        rotation += degrees
        update()
    }

    override fun flipHorizontal() {
        textureCoors.let {
            var tmp = it[0]
            it[0] = it[2]
            it[2] = tmp

            tmp = it[4]
            it[4] = it[6]
            it[6] = tmp
        }
        val cc = ByteBuffer.allocateDirect(textureCoors.size * 4)
        cc.order(ByteOrder.nativeOrder())
        textureBuffer = cc.asFloatBuffer()
        textureBuffer.put(textureCoors)
        textureBuffer.position(0)
    }

    private fun loadShader(type: Int, shaderCode: String): Int {
        //根据type创建顶点着色器或者片元着色器
        val shader = GLES20.glCreateShader(type)
        //将资源加入到着色器中，并编译
        GLES20.glShaderSource(shader, shaderCode)
        GLES20.glCompileShader(shader)

        return shader
    }

    private fun update() {
        sprite2d.setScale(scale * contentWidth / 2, scale * contentHeight / 2)
        sprite2d.setPosition(
            posX, posY
        )
        sprite2d.rotation = rotation
    }
}