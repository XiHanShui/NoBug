package com.ffmpeg.practise

interface EventCallback {

    fun onPlayerEvent(msgType: Int, msgValue: Float)
}