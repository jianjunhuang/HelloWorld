package com.jianjun.helloopengles.clip

import android.graphics.Bitmap
import android.opengl.GLES20
import android.opengl.GLES32
import android.opengl.GLUtils
import com.jianjun.helloopengles.clip.DefDrawer

class BitmapDrawer(private val bmp: Bitmap) : DefDrawer() {

    override fun bindTexture() {
        if (!bmp.isRecycled) {
            //绑定图片到被激活的纹理单元
            GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bmp, 0)
        }
    }

    override fun activateTexture() {
        super.activateTexture()

        //激活指定纹理单元
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        //绑定纹理ID到纹理单元
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId)
        //将激活的纹理单元传递到着色器里面
        GLES20.glUniform1i(textureHandler, 0)
        //配置边缘过渡参数
        GLES20.glTexParameterf(
            GLES20.GL_TEXTURE_2D,
            GLES20.GL_TEXTURE_MIN_FILTER,
            GLES20.GL_LINEAR.toFloat()
        )
        GLES20.glTexParameterf(
            GLES20.GL_TEXTURE_2D,
            GLES20.GL_TEXTURE_MAG_FILTER,
            GLES20.GL_LINEAR.toFloat()
        )
        GLES20.glTexParameteri(
            GLES20.GL_TEXTURE_2D,
            GLES20.GL_TEXTURE_WRAP_S,
            GLES32.GL_CLAMP_TO_BORDER
        )
        GLES20.glTexParameteri(
            GLES20.GL_TEXTURE_2D,
            GLES20.GL_TEXTURE_WRAP_T,
            GLES32.GL_CLAMP_TO_BORDER
        )
    }

    override fun getVertexShader(): String {
        return "attribute vec4 aPosition;" +
                "precision mediump float;" +
                "uniform mat4 uMatrix;" +
                "attribute vec2 aCoordinate;" +
                "varying vec2 vCoordinate;" +
                "void main() {" +
                "  gl_Position = uMatrix*aPosition;" +
                "  vCoordinate = aCoordinate;" +
                "}"
    }

    override fun getFragmentShader(): String {
        return "precision mediump float;" +
                "uniform sampler2D uTexture;" +
                "varying vec2 vCoordinate;" +
                "void main() {" +
                "  vec4 color = texture2D(uTexture, vCoordinate);" +
                "  gl_FragColor = color;" +
                "}"
    }
}