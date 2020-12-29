package com.socct.nestripple.nestRipple

import android.animation.Animator
import android.animation.AnimatorSet
import android.animation.ObjectAnimator
import android.content.Context
import android.graphics.Color
import android.util.AttributeSet
import android.view.View
import android.view.animation.AccelerateDecelerateInterpolator
import android.widget.FrameLayout
import android.widget.RelativeLayout
import com.socct.nestripple.R

class RippleAnimationView @JvmOverloads constructor(
    context: Context,
    attributeSet: AttributeSet? = null,
    def: Int = 0
) : RelativeLayout(context, attributeSet, def) {


    private var animType: Int
    private var circleStrokeWidth: Int
    private var circleRadius: Int
    private var animDuration: Int
    private var animAmount: Int
    private var animColor: Int
    private var animScale: Float

    private var viewList: MutableList<View>

    private val animatorSet by lazy { AnimatorSet() }
    private val animatorList by lazy { ArrayList<Animator>() }
    private var isRunning = false


    init {
        val osa =
            context.obtainStyledAttributes(attributeSet, R.styleable.RippleAnimationView)
        animScale = osa.getFloat(R.styleable.RippleAnimationView_ripple_anim_scale, 4F)
        animColor = osa.getColor(R.styleable.RippleAnimationView_ripple_anim_color, Color.RED)
        animAmount = osa.getInt(R.styleable.RippleAnimationView_ripple_anim_amount, 5)
        animDuration = osa.getInt(R.styleable.RippleAnimationView_ripple_anim_duration, 4500)
        circleRadius =
            osa.getDimensionPixelSize(R.styleable.RippleAnimationView_ripple_anim_radius, 40)
        circleStrokeWidth =
            osa.getDimensionPixelSize(R.styleable.RippleAnimationView_ripple_anim_strokeWidth, 20)
        animType = osa.getInt(R.styleable.RippleAnimationView_ripple_anim_type, 0)
        osa.recycle()
        viewList = ArrayList(animAmount)


        val fl = (animDuration / animAmount).toLong()
        val w = 2 * (circleRadius + circleStrokeWidth)
        val layoutParams = LayoutParams(w, w)
        layoutParams.addRule(CENTER_IN_PARENT, TRUE)

        for (i in 0 until animAmount) {
            val rippleCircleView = RippleCircleView(context)
            rippleCircleView.setPaintParameter(
                animColor,
                animType != 0,
                circleStrokeWidth.toFloat()
            )
            addView(rippleCircleView, layoutParams)
            viewList.add(rippleCircleView)
            /*x 轴动画*/
            val scaleX = ObjectAnimator.ofFloat(rippleCircleView, "ScaleX", 1F, animScale)
            scaleX.repeatMode = ObjectAnimator.RESTART
            scaleX.repeatCount = ObjectAnimator.INFINITE
            scaleX.startDelay = fl * i
            scaleX.duration = fl
            animatorList.add(scaleX)

            val scaleY = ObjectAnimator.ofFloat(rippleCircleView, "ScaleY", 1F, animScale)
            scaleY.repeatMode = ObjectAnimator.RESTART
            scaleY.repeatCount = ObjectAnimator.INFINITE
            scaleY.startDelay = fl * i
            scaleY.duration = fl
            animatorList.add(scaleY)
            val alpha = ObjectAnimator.ofFloat(rippleCircleView, "Alpha", 1F, 0F)
            alpha.repeatMode = ObjectAnimator.RESTART
            alpha.repeatCount = ObjectAnimator.INFINITE
            alpha.startDelay = fl * i
            alpha.duration = fl
            animatorList.add(alpha)
        }
        animatorSet.interpolator = AccelerateDecelerateInterpolator()
        animatorSet.playTogether(animatorList)
    }


    fun startAnimation() {
        isRunning = true
        animatorSet.start()
    }

    fun stopAnimation() {
        animatorSet.end()
        isRunning = false
    }


    override fun onDetachedFromWindow() {
        super.onDetachedFromWindow()
        stopAnimation()
    }


}