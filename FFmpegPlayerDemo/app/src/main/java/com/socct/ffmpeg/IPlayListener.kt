package com.socct.ffmpeg

interface IPlayListener {

    fun onError(code: Int);


    fun onPrepare()


    fun onProgress(progress: Int)

}