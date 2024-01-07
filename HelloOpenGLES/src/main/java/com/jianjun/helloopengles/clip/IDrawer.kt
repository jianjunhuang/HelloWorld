package com.jianjun.helloopengles.clip

import android.graphics.SurfaceTexture

interface IDrawer {
    fun draw()
    fun setTextureID(id: Int)
    fun release()
    fun getSurfaceTexture(callback: (SurfaceTexture?) -> Unit)
    fun setContentSize(width: Int, height: Int)

    //设置OpenGL窗口宽高
    fun setWorldSize(worldW: Int, worldH: Int)
    fun move(x: Float, y: Float)
    fun scale(scale: Float, focusX: Float, focusY: Float)
    fun rotate(degrees: Float)
    fun flipHorizontal()
    fun getVertexShader(): String
    fun getFragmentShader(): String
}