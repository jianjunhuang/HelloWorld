package com.jianjun.helloopengles.utils

import android.content.Context
import java.io.BufferedReader
import java.io.InputStreamReader

object ResourceUtils {

    fun raw2String(context: Context, rawId: Int): String {
        try {
            val inputStream = context.resources.openRawResource(rawId)
            val inputReader = InputStreamReader(inputStream)
            val bufferedReader = BufferedReader(inputReader)
            return bufferedReader.readText()
        } catch (e: Exception) {
            e.printStackTrace()
        }
        return ""
    }

}