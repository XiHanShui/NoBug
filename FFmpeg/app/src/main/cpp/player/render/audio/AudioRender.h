//
// Created by WJ on 2022/8/10.
//

#ifndef FFMPEGDEMO_AUDIORENDER_H
#define FFMPEGDEMO_AUDIORENDER_H

class AudioFrame {
public:
    AudioFrame(uint8_t *data, int dataSize, bool hardCopy = true) {
        this->dataSize = dataSize;
        this->data = data;
        this->hardCopy = hardCopy;
        if (hardCopy) {
            this->data = static_cast<uint8_t *>(malloc(this->dataSize));
            memcpy(this->data, data, dataSize);
        }
    }


    ~AudioFrame() {
        if (hardCopy && this->data) {
            free(this->data);
            this->data = nullptr;
        }
    }

    uint8_t *data = nullptr;
    int dataSize = 0;
    bool hardCopy = true;

};

class  AudioRender{

public:
    virtual  ~AudioRender(){};
    virtual  void  init()=0;
    virtual  void  clearAudioCache()=0;
    virtual  void  renderAudioFrame(uint8_t *pData,int dataSize)=0;
    virtual  void  unInit()=0;
};




#endif //FFMPEGDEMO_AUDIORENDER_H
