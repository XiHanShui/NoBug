package com.socct.nestripple

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.socct.nestripple.nestRipple.RippleAnimationView

class NestRippleMainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_nest_ripple_main)
        val rippleView = findViewById<RippleAnimationView>(R.id.nest_ripple)
        rippleView.post {
            rippleView.startAnimation()
        }
    }
}