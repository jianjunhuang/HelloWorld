package com.jianjun.helloopengles.program

import android.content.Context
import android.opengl.GLES20
import com.jianjun.helloopengles.R

class TextureShaderProgram(
    context: Context
) : ShaderProgram(context, R.raw.texture_vertex_shader, R.raw.texture_fragment_shader) {

    private val uMatrixLocation = GLES20.glGetUniformLocation(program, U_MATRIX)
    private val uTextureUnitLocation = GLES20.glGetUniformLocation(program, U_TEXTURE_UNIT)
    val aPositionLocation = GLES20.glGetAttribLocation(program, A_POSITION)
    val aTextureCoordinatesLocation = GLES20.glGetAttribLocation(
        program,
        A_TEXTURE_COORDINATES
    )

    /**
     * 设置 uniform 并返回属性位置
     *
     *
     */
    fun setUniforms(matrix: FloatArray, textureId: Int) {

        //传递 matrix 给 uniform
        GLES20.glUniformMatrix4fv(uMatrixLocation, 1, false, matrix, 0)

        /*
        使用纹理进行绘制时，不是直接给着色器传递纹理
        我们使用纹理单元保存纹理，因为一个 GPU 只能同时绘制数量有限的纹理
        他使用这些纹理单元表示当前正在被绘制的活动的纹理。
        切换纹理时，可以在纹理单元中来回切换，但是如果切换得太过频繁，会拖慢渲染速度
         */
        //把活动的纹理单元设置为 0
        GLES20.glActiveTexture(GLES20.GL_TEXTURE0)
        //绑定纹理到这个单元
        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId)
        //把选定的纹理单元传递给片段着色器中的 u_TextureUnit
        GLES20.glUniform1i(uTextureUnitLocation, 0)
    }

}