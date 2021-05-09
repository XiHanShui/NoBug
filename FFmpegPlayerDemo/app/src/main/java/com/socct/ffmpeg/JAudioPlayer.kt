package com.socct.ffmpeg

class JAudioPlayer {

    private var nativeHandle: Long? = null


    var listener: IPlayListener? = null

    fun prepare() {
        nativeHandle = nativeInit()
    }

    fun setDataSource(path: String) {
        nativeHandle?.let {
            setDataSource(path, it)
        }
    }

    fun onStart() {
        nativeHandle?.let {
            onStart(it)
        }
    }

    fun onPause() {
        nativeHandle?.let {
            onPause(it)
        }
    }

    fun onStop() {
        nativeHandle?.let {
            onStop(it)
        }
    }


    private  fun onError(code:Int){

    }
    private  fun onPrepare(){

    }
    private  fun onProgress(progress:Int){

    }


    private external fun nativeInit(): Long
    private external fun prepare(nativeHandle: Long);
    private external fun setDataSource(path: String, nativeHandle: Long);
    private external fun onStart(nativeHandle: Long);
    private external fun onPause(nativeHandle: Long);
    private external fun onStop(nativeHandle: Long);


    companion object {
        init {
            System.loadLibrary("ffmpeg")
        }

    }


}