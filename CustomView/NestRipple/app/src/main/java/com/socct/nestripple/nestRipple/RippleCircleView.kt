package com.socct.nestripple.nestRipple

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.View
import androidx.annotation.ColorInt

class RippleCircleView @JvmOverloads constructor(
    context: Context,
    attributeSet: AttributeSet? = null,
    def: Int = 0
) : View(context, attributeSet, def) {

    var circleColor: Int = Color.RED

    var w: Int = 0
    var radius: Float = 0F


    private val paint by lazy {
        Paint().apply {
            this.isAntiAlias = true
            this.isDither = true
            this.color = circleColor
            this.style = Paint.Style.FILL
        }
    }


    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val min = MeasureSpec.getSize(widthMeasureSpec)
            .coerceAtMost(MeasureSpec.getSize(heightMeasureSpec))
        setMeasuredDimension(min, min)
        radius = min / 2F
    }


    /**
     * parameter
     */
    fun setPaintParameter(@ColorInt color: Int, isStork: Boolean, storkWidth: Float = 0F) {
        if (isStork) {
            paint.style = Paint.Style.STROKE
            paint.strokeWidth = storkWidth
        } else {
            paint.style = Paint.Style.FILL
        }
        paint.color = color
    }


    override
    fun onDraw(canvas: Canvas) {
        canvas.drawCircle(radius, radius, radius, paint)
    }


}