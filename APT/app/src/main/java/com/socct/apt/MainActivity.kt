package com.socct.apt

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import com.socct.animation.ARouter

@ARouter(value = "aaaaa")
class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }
}