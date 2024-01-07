package com.jianjun.helloopengles.utils

import android.opengl.GLES20
import java.nio.ByteBuffer
import java.nio.ByteOrder

class VertexArray(private val vertexData: FloatArray) {

    private val floatBuffer = ByteBuffer
        .allocateDirect(vertexData.size * BYTES_PER_FLOAT)
        .order(ByteOrder.nativeOrder())
        .asFloatBuffer()
        .put(vertexData)

    fun setVertexAttributePointer(
        dataOffset: Int, attributeLocation: Int,
        componentCount: Int, stride: Int
    ) {
        floatBuffer.position(dataOffset)
        GLES20.glVertexAttribPointer(
            attributeLocation, componentCount,
            GLES20.GL_FLOAT, false, stride, floatBuffer
        )
        GLES20.glEnableVertexAttribArray(attributeLocation)
        floatBuffer.position(0)
    }

    companion object {
        const val BYTES_PER_FLOAT = 4
    }
}