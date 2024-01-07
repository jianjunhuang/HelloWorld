package com.jianjun.helloopengles.airhockey.objects

import android.opengl.GLES20
import com.jianjun.helloopengles.program.ColorShaderProgram
import com.jianjun.helloopengles.utils.VertexArray

class Mallet {

    val vertexArray = VertexArray(VERTEX_DATA)

    fun bindData(colorShaderProgram: ColorShaderProgram) {
        vertexArray.setVertexAttributePointer(
            0,
            colorShaderProgram.aPositionLocation,
            POSITION_COMPONENT_COUNT,
            STRIDE
        )

        vertexArray.setVertexAttributePointer(
            POSITION_COMPONENT_COUNT,
            colorShaderProgram.aColorLocation,
            COLOR_COMPONENT_COUNT,
            STRIDE
        )
    }

    fun draw() {
        GLES20.glDrawArrays(GLES20.GL_POINTS, 0, 2)
    }

    companion object {
        private const val POSITION_COMPONENT_COUNT = 2
        private const val COLOR_COMPONENT_COUNT = 3
        private const val STRIDE =
            (POSITION_COMPONENT_COUNT + COLOR_COMPONENT_COUNT) * VertexArray.BYTES_PER_FLOAT

        private val VERTEX_DATA = floatArrayOf(
            //x , y , r , g , b
            0f, -0.4f, 0f, 0f, 1f,
            0f, 0.4f, 1f, 0f, 0f
        )
    }
}