package com.jianjun.helloopengles.utils

import android.opengl.GLES20
import android.util.Log

object ShaderUtils {
    fun compileVertexShader(shaderCode: String): Int {
        return compileShader(GLES20.GL_VERTEX_SHADER, shaderCode)
    }

    fun compileFragmentShader(shaderCode: String): Int {
        return compileShader(GLES20.GL_FRAGMENT_SHADER, shaderCode)
    }

    private fun compileShader(type: Int, shaderCode: String): Int {
        return GLES20.glCreateShader(type).also { shaderId ->//1) create new shader
            if (shaderId == 0) {
                Log.e(
                    TAG,
                    "compileShader: shaderId = $shaderId, ${GLES20.glGetShaderInfoLog(shaderId)}"
                )
                return@also //failed to create shader
            }
            //2) upload shader code to shader
            GLES20.glShaderSource(shaderId, shaderCode)
            //3) compile shader
            GLES20.glCompileShader(shaderId)
            //4) check
            val status = getShaderCompileStatus(shaderId)
            if (status[0] == 0) {
                GLES20.glDeleteShader(shaderId)
                Log.e(
                    TAG,
                    "compileShader: failed to compile shader; shaderId = $shaderId, ${
                        GLES20.glGetShaderInfoLog(shaderId)
                    }"
                )
                return 0
            }
        }
    }

    private fun getShaderCompileStatus(shaderId: Int): IntArray {
        val compileStatus = IntArray(1)
        GLES20.glGetShaderiv(shaderId, GLES20.GL_COMPILE_STATUS, compileStatus, 0)
        return compileStatus
    }

    fun linkProgram(vertexShaderId: Int, fragmentShaderId: Int): Int {
        //1) create program
        val programId = GLES20.glCreateProgram()
        if (programId == 0) {
            Log.e(
                TAG,
                "linkProgram: failed to create program, ${GLES20.glGetProgramInfoLog(programId)}"
            )
            return 0
        }
        //2) link shader
        GLES20.glAttachShader(programId, vertexShaderId)
        GLES20.glAttachShader(programId, fragmentShaderId)
        //3) link program
        GLES20.glLinkProgram(programId)
        //4) check
        if (getProgramLinkStatus(programId)[0] == 0) {
            GLES20.glDeleteProgram(programId)
            Log.e(
                TAG,
                "linkProgram: failed to link program, ${
                    GLES20.glGetProgramInfoLog(programId)
                }"
            )
            return 0
        }

        return programId
    }

    private fun getProgramLinkStatus(programId: Int): IntArray {
        GLES20.glValidateProgram(programId)
        val compileStatus = IntArray(1)
        GLES20.glGetProgramiv(programId, GLES20.GL_LINK_STATUS, compileStatus, 0)
        return compileStatus
    }

    /**
     *
     */
    fun buildProgram(vertexShaderSource: String, fragmentShaderSource: String): Int {
        //compile the shader
        val vertexShader = compileVertexShader(vertexShaderSource)
        val fragmentShader = compileFragmentShader(fragmentShaderSource)

        //link them into a shader program
        val program = linkProgram(vertexShader, fragmentShader)
        return program
    }

    const val TAG = "ShaderUtils"
}