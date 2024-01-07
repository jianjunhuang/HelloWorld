package com.jianjun.helloopengles.program

import android.content.Context
import android.opengl.GLES20
import com.jianjun.helloopengles.utils.ResourceUtils
import com.jianjun.helloopengles.utils.ShaderUtils
import com.jianjun.helloopengles.utils.TextureUtils

open class ShaderProgram(
    context: Context,
    vertexShaderResourceId: Int,
    fragmentShaderResourceId: Int
) {

    //shader program
    var program = -1

    init {
        program = ShaderUtils.buildProgram(
            ResourceUtils.raw2String(context, vertexShaderResourceId),
            ResourceUtils.raw2String(context, fragmentShaderResourceId)
        )
    }

    fun useProgram() {
        GLES20.glUseProgram(program)
    }

    companion object {
        //Uniform constants
        const val U_MATRIX = "u_Matrix"
        const val U_TEXTURE_UNIT = "u_TextureUnit"

        //Attribute constants
        const val A_POSITION = "a_Position"
        const val A_COLOR = "a_Color"
        const val A_TEXTURE_COORDINATES = "a_TextureCoordinates"

    }
}