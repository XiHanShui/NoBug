//
// Created by WJ on 2021/5/9.
//

#ifndef FFMPEGDEMO_VIDEOCHANNEL_H
#define FFMPEGDEMO_VIDEOCHANNEL_H

#include <android/native_window.h>
#include "BaseChannel.h"
#include "AudioChannel.h"

class VideoChannel : public BaseChannel {

    friend void videoPlay_t(void *args);

public:


};


#endif //FFMPEGDEMO_VIDEOCHANNEL_H
