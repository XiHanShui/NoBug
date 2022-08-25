//
// Created by WJ on 2022/8/23.
//

#ifndef FFMPEGDEMO_IMAGEDEF_H
#define FFMPEGDEMO_IMAGEDEF_H

#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdint.h>
#include "LogUtil.h"

#define  IMAGE_FORMAT_RGBA 0x01
#define  IMAGE_FORMAT_NV21 0x02
#define  IMAGE_FORMAT_NV12 0x03
#define  IMAGE_FORMAT_I420 0x04

#define  IMAGE_FORMAT_RGBA_EXT  "RGB32"
#define  IMAGE_FORMAT_NV21_EXT  "NV21"
#define  IMAGE_FORMAT_NV12_EXT  "NV12"
#define  IMAGE_FORMAT_I420_EXT  "I420"

typedef struct _tag_NativeRectF {
    float left;
    float top;
    float right;
    float bottom;

    _tag_NativeRectF() {
        left = top = right = bottom = 0.0F;
    }
} RectF;

typedef struct _tag_NativeImage {
    int width;
    int height;
    int format;
    uint8_t *ppPlane[3];
    int pLineSize[3];

    _tag_NativeImage() {
        width = height = format = 0;
        ppPlane[0] = nullptr;
        ppPlane[1] = nullptr;
        ppPlane[2] = nullptr;
        pLineSize[0] = 0;
        pLineSize[1] = 0;
        pLineSize[2] = 0;

    }
} NativeImage;

class NativeImageUtil {
public:
    static void allocNativeImage(NativeImage *pImage) {
        if (pImage->height == 0 || pImage->width == 0) {
            return;
        }
        switch (pImage->format) {
            case IMAGE_FORMAT_RGBA:
                pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(pImage->width * pImage->height * 4));
                pImage->pLineSize[0] = pImage->width * 4;
                pImage->pLineSize[1] = 0;
                pImage->pLineSize[2] = 0;
                break;
            case IMAGE_FORMAT_NV12:
            case IMAGE_FORMAT_NV21:
                pImage->ppPlane[0]=static_cast<uint8_t *>(malloc(pImage->width*pImage->height*1.5));
                pImage->ppPlane[1]=pImage->ppPlane[0]+pImage->width*pImage->height;
                pImage->pLineSize[0]=pImage->width;
                pImage->pLineSize[1]=pImage->height;
                pImage->pLineSize[2]=0;
                break;

            case IMAGE_FORMAT_I420:
                pImage->ppPlane[0]=static_cast<uint8_t *>(malloc(pImage->width*pImage->height*1.5));
                pImage->ppPlane[1]=pImage->ppPlane[0]+pImage->width+pImage->height;
                pImage->ppPlane[2]=pImage->ppPlane[1]+(pImage->width>>1)*(pImage->height);
                pImage->pLineSize[0]=pImage->width;
                pImage->pLineSize[1]=pImage->width/2;
                pImage->pLineSize[2]=pImage->width/2;
                break;
            default:
            // 打印错误日志
                break;
        }
    }


};


#endif //FFMPEGDEMO_IMAGEDEF_H
