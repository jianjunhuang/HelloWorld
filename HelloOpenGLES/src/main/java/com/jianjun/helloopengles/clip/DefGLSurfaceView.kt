package com.jianjun.helloopengles.clip

import android.content.Context
import android.graphics.PointF
import android.media.midi.MidiOutputPort
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.ScaleGestureDetector


class DefGLSurfaceView : GLSurfaceView {

    constructor(context: Context) : super(context)

    constructor(context: Context, attrs: AttributeSet) : super(context, attrs)

    private var mPrePoint = PointF()

    private var mDrawer: IDrawer? = null
    private var initScale = -1f
    private val scaleGestureDetector =
        ScaleGestureDetector(context, object : ScaleGestureDetector.SimpleOnScaleGestureListener() {
            override fun onScale(detector: ScaleGestureDetector?): Boolean {
                if (detector == null) return false
                mDrawer?.scale(
                    detector.scaleFactor,
                    detector.focusX / width,
                    detector.focusY / height
                )
                return true
            }
        })

    override fun onTouchEvent(event: MotionEvent): Boolean {

        scaleGestureDetector.onTouchEvent(event)

        when (event.action) {
            MotionEvent.ACTION_DOWN -> {
                mPrePoint.x = event.x
                mPrePoint.y = event.y
            }
            MotionEvent.ACTION_MOVE -> {
                //dx，dy是归一化的距离，范围（0～1）
                val dx = (event.x - mPrePoint.x) / width
                val dy = (event.y - mPrePoint.y) / height
                if (event.pointerCount == 2) {
//                   //(x1+x2)/2, (y1+y2)/2
                    mDrawer?.move(
                        (event.getX(0) + event.getX(1)) / 2,
                        (event.getY(0) + event.getY(1)) / 2
                    )
                } else {
                    mDrawer?.move(event.x, event.y)
                }
                mPrePoint.x = event.x
                mPrePoint.y = event.y
            }
            MotionEvent.ACTION_POINTER_UP -> {
            }
            MotionEvent.ACTION_CANCEL -> {
            }
        }
        return true
    }

    fun addDrawer(drawer: IDrawer) {
        mDrawer = drawer
    }
}