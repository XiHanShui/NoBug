package com.socct.porterduffdemo

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.View

class PorterDuffView @JvmOverloads constructor(
    context: Context,
    attributeSet: AttributeSet? = null,
    def: Int = 0
) : View(context, attributeSet, def) {


    val model = arrayListOf(
        PorterDuff.Mode.CLEAR,
        PorterDuff.Mode.SRC,
        PorterDuff.Mode.DST,
        PorterDuff.Mode.SRC_OVER,
        PorterDuff.Mode.DST_OVER,
        PorterDuff.Mode.SRC_IN,
        PorterDuff.Mode.DST_IN,
        PorterDuff.Mode.SRC_OUT,
        PorterDuff.Mode.DST_OUT,
        PorterDuff.Mode.SRC_ATOP,
        PorterDuff.Mode.DST_ATOP,
        PorterDuff.Mode.XOR,
        PorterDuff.Mode.DARKEN,
        PorterDuff.Mode.LIGHTEN,
        PorterDuff.Mode.MULTIPLY,
        PorterDuff.Mode.SCREEN,
        PorterDuff.Mode.ADD,
        PorterDuff.Mode.OVERLAY,
    )


    private val rect by lazy { Rect() }
    private val rectF by lazy { RectF() }


    private var dstBitmap: Bitmap? = null

    private var srcBitmap: Bitmap? = null

    private val paint by lazy {
        Paint().apply {
            style = Paint.Style.FILL
            isDither = true
            isAntiAlias = true
        }
    }


    override fun onDraw(canvas: Canvas) {
        makeDst()
        makeSrc()
        if (dstBitmap == null || srcBitmap == null) {
            return
        }



    }


    private fun makeDst() {


    }

    private fun makeSrc() {

    }

}