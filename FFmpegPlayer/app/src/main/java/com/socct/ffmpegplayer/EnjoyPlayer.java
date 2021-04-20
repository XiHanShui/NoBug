package com.socct.ffmpegplayer;

/**
 * @author WJ
 * @date 2021/4/17
 * @description 视频播放器
 */
public class EnjoyPlayer {

    static {
        System.loadLibrary("native-lib");
    }

    private long mHandle;


    public EnjoyPlayer() {
        mHandle = nativeInit();
    }

    public void setDataSource(String path) {
        setDataSource(mHandle, path);
    }

    public void onPrepare() {
        onPause(mHandle);
    }


    public void onStart() {
        onStop(mHandle);
    }

    public void onStop() {
        onStop(mHandle);
    }

    public void onPause() {
        onPause(mHandle);
    }

    private native long nativeInit();

    private native void setDataSource(long handle, String path);

    private native void onStop(long handle);

    private native void onPause(long handle);


    private void onError(int code) {

    }


    private void setProgress(int progress) {

    }

    private void setOnComplete() {

    }







}
