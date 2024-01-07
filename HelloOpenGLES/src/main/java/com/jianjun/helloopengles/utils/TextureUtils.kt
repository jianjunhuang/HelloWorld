package com.jianjun.helloopengles.utils

import android.content.Context
import android.graphics.BitmapFactory
import android.opengl.GLES20
import android.opengl.GLUtils
import android.util.Log

object TextureUtils {
    private const val TAG = "TextureUtils"

    fun loadTexture(context: Context, resId: Int): Int {
        val textureObjIds = IntArray(1)
        GLES20.glGenTextures(1, textureObjIds, 0)

        if (textureObjIds[0] == 0) {
            Log.e(TAG, "loadTexture: failed to generate new OpenGL texture object")
            return 0
        }
        val bmpOption = BitmapFactory.Options()
        bmpOption.inScaled = false

        val bmp = BitmapFactory.decodeResource(context.resources, resId, bmpOption)
        if (bmp == null) {
            GLES20.glDeleteTextures(1, textureObjIds, 0)
            return 0
        }
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureObjIds[0])
        //set filter
        //缩小时用 三线性过滤( minpmap 确保纹理元素显示正常，linear 确保贴图切换时不会有明显跳跃)
        GLES20.glTexParameteri(
            GLES20.GL_TEXTURE_2D,
            GLES20.GL_TEXTURE_MIN_FILTER,
            GLES20.GL_LINEAR_MIPMAP_LINEAR
        )
        //放大 双线性过滤，使用四个邻接像素的纹理元素，利用线性插值算法做插值，使得放大后边缘也可以更加平滑
        GLES20.glTexParameteri(
            GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER,
            GLES20.GL_LINEAR
        )
        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bmp, 0)
        bmp.recycle()
        GLES20.glGenerateMipmap(GLES20.GL_TEXTURE_2D)
        //加载完后与这个纹理解绑，避免用到其他纹理方法意外改变这个纹理
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0)
        return textureObjIds[0]
    }

}