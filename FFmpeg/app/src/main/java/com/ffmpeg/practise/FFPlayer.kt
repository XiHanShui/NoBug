package com.ffmpeg.practise

import android.view.Surface

class FFPlayer {

    var nativePlayerHandle = -1L

    private var eventCallback: EventCallback? = null

    fun init(url: String, videoRenderType: Int, surface: Surface) {
        nativePlayerHandle = nativeInit(url, videoRenderType, surface)
    }

    fun play() {
        nativePlay(nativePlayerHandle)
    }

    fun pause() {
        nativePause(nativePlayerHandle)
    }


    fun seekToPosition(position: Float) {
        nativeSeekToPosition(nativePlayerHandle, position)
    }

    fun stop() {
        nativeStop(nativePlayerHandle)
    }


    fun unInit() {
        nativeUnInit(nativePlayerHandle)
    }

    fun getMediaParams(paramType: Int): Long {
        return nativeGetMediaParams(paramType)
    }


    private fun playerEventCallback(msgType: Int, msgValue: Float) {
        eventCallback?.onPlayerEvent(msgType, msgValue)
    }


    /**
     * @param url 文件id
     * @param videoRenderType 渲染方式
     * @param surface  surface
     */
    private external fun nativeInit(url: String, videoRenderType: Int, surface: Surface): Long
    private external fun nativePlay(playerHandle: Long)
    private external fun nativePause(playerHandle: Long)
    private external fun nativeSeekToPosition(playerHandle: Long, position: Float)
    private external fun nativeStop(playerHandle: Long)
    private external fun nativeUnInit(nativePlayerHandle: Long)
    private external fun nativeGetMediaParams(paramType: Int): Long

    companion object {

        /*gl render type*/
        const val VIDEO_GL_RENDER = 0
        const val AUDIO_GL_RENDER = 1
        const val VR_3D_GL_RENDER = 2


        /*error code*/
        const val MSG_DECODER_INIT_ERROR = 0;
        const val MSG_DECODER_READY = 1
        const val MSG_DECODER_DONE = 2
        const val MSG_REQUEST_RENDER = 3
        const val MSG_DECODING_TIME = 4


        const val MEDIA_PARAM_VIDEO_WIDTH = 0x0001
        const val MEDIA_PARAM_VIDEO_HEIGHT = 0x0002
        const val MEDIA_PARAM_VIDEO_DURATION = 0x0003

        const val VIDEO_RENDER_OPENGL = 0
        const val VIDEO_RENDER_ANWINDOW = 1
        const val VIDEO_RENDER_3D_VR = 2

        @JvmStatic
        external fun getFFmpegVersion(): String
        /* GL render*/
        @JvmStatic
        external fun nativeOnSurfaceCreated(renderType: Int);

        @JvmStatic
        external fun nativeOnSurfaceChanged(renderType: Int, width: Int, height: Int);

        @JvmStatic
        external fun nativeOnDrawframe(renderType: Int);

        /*update MVP matrix */
        @JvmStatic
       external fun  nativeSetGesture(renderType: Int,xRotateAngle:Float,yRotateAngle:Float,scale:Float)
       @JvmStatic
       external  fun nativeSetTouchLoc(renderType: Int,touchX:Float,touchY:Float)


        init {
            System.loadLibrary("practise")
        }

    }


}