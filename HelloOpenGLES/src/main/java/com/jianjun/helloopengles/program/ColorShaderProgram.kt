package com.jianjun.helloopengles.program

import android.content.Context
import android.opengl.GLES20
import com.jianjun.helloopengles.R

class ColorShaderProgram(
    context: Context
) : ShaderProgram(context, R.raw.simple_vertex_shader, R.raw.simple_fragment_shader) {

    private val uMatrixLocation = GLES20.glGetUniformLocation(program, U_MATRIX)
    val aPositionLocation: Int = GLES20.glGetAttribLocation(program, A_POSITION)
    val aColorLocation: Int = GLES20.glGetAttribLocation(program, A_COLOR)

    fun setUniforms(matrix: FloatArray) {
        GLES20.glUniformMatrix4fv(uMatrixLocation, 1, false, matrix, 0)
    }

}