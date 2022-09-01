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
                pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(
                        pImage->width * pImage->height * 4));
                pImage->pLineSize[0] = pImage->width * 4;
                pImage->pLineSize[1] = 0;
                pImage->pLineSize[2] = 0;
                break;
            case IMAGE_FORMAT_NV12:
            case IMAGE_FORMAT_NV21:
                pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(
                        pImage->width * pImage->height * 1.5));
                pImage->ppPlane[1] = pImage->ppPlane[0] + pImage->width * pImage->height;
                pImage->pLineSize[0] = pImage->width;
                pImage->pLineSize[1] = pImage->height;
                pImage->pLineSize[2] = 0;
                break;

            case IMAGE_FORMAT_I420:
                pImage->ppPlane[0] = static_cast<uint8_t *>(malloc(
                        pImage->width * pImage->height * 1.5));
                pImage->ppPlane[1] = pImage->ppPlane[0] + pImage->width + pImage->height;
                pImage->ppPlane[2] = pImage->ppPlane[1] + (pImage->width >> 1) * (pImage->height);
                pImage->pLineSize[0] = pImage->width;
                pImage->pLineSize[1] = pImage->width / 2;
                pImage->pLineSize[2] = pImage->width / 2;
                break;
            default:
                // 打印错误日志
                break;
        }
    }

    static void copyNativeImage(NativeImage *pSrcImg, NativeImage *pDstImg) {
        if (pSrcImg == nullptr || pSrcImg->ppPlane[0] == nullptr) {
            return;
        }
        if (pSrcImg->format != pDstImg->format || pSrcImg->width != pDstImg->width ||
            pSrcImg->height != pDstImg->height) {
            //  NativeImageUtil::copyNativeImage invalid params
            return;
        }

        if (pDstImg->ppPlane[0] == nullptr) {
            allocNativeImage(pDstImg);
        }

        switch (pSrcImg->format) {
            case IMAGE_FORMAT_I420:
                // y plane
                if (pSrcImg->pLineSize[0] != pDstImg->pLineSize[0]) {
                    for (int i = 0; i < pSrcImg->height; ++i) {
                        memcpy(pDstImg->ppPlane[0] + i * pDstImg->pLineSize[0],
                               pSrcImg->ppPlane[0] + i * pSrcImg->pLineSize[0], pDstImg->width);
                    }
                } else {
                    memcpy(pDstImg->ppPlane[0], pSrcImg->ppPlane[0],
                           pDstImg->pLineSize[0] * pSrcImg->height);
                }

                // u plane
                if (pSrcImg->pLineSize[1] != pDstImg->pLineSize[1]) {
                    for (int i = 0; i < pSrcImg->height / 2; ++i) {
                        memcpy(pDstImg->ppPlane[1] + i * pDstImg->pLineSize[1],
                               pSrcImg->ppPlane[1] + i * pSrcImg->pLineSize[1], pDstImg->width / 2);
                    }
                } else {
                    memcpy(pDstImg->ppPlane[1], pSrcImg->ppPlane[1],
                           pDstImg->pLineSize[1] * pSrcImg->height / 2);
                }

                // v plane
                if (pSrcImg->pLineSize[2] != pDstImg->pLineSize[2]) {
                    for (int i = 0; i < pSrcImg->height / 2; ++i) {
                        memcpy(pDstImg->ppPlane[2] + i * pDstImg->pLineSize[2],
                               pSrcImg->ppPlane[2] + i * pSrcImg->pLineSize[2], pDstImg->width / 2);
                    }
                } else {
                    memcpy(pDstImg->ppPlane[2], pSrcImg->ppPlane[2],
                           pDstImg->pLineSize[2] * pSrcImg->height / 2);
                }
                break;

            case IMAGE_FORMAT_NV21:
            case IMAGE_FORMAT_NV12:

                // y plane
                if (pSrcImg->pLineSize[0] != pDstImg->pLineSize[0]) {
                    for (int i = 0; i < pSrcImg->height; ++i) {
                        memcpy(pDstImg->ppPlane[0] + i * pDstImg->pLineSize[0],
                               pSrcImg->ppPlane[0] + i * pSrcImg->pLineSize[0], pDstImg->width);
                    }
                } else {
                    memcpy(pDstImg->ppPlane[0], pSrcImg->ppPlane[0],
                           pDstImg->pLineSize[0] * pSrcImg->height);
                }

                // uv plane
                if (pSrcImg->pLineSize[1] != pDstImg->pLineSize[1]) {
                    for (int i = 0; i < pSrcImg->height / 2; ++i) {
                        memcpy(pDstImg->ppPlane[1] + i * pDstImg->pLineSize[1],
                               pSrcImg->ppPlane[1] + i * pSrcImg->pLineSize[1], pDstImg->width);
                    }
                } else {
                    memcpy(pDstImg->ppPlane[1], pSrcImg->ppPlane[1],
                           pDstImg->pLineSize[1] * pSrcImg->height / 2);
                }
                break;
            case IMAGE_FORMAT_RGBA:
                if (pSrcImg->pLineSize[0] != pDstImg->pLineSize[0]) {
                    for (int i = 0; i < pSrcImg->height; ++i) {
                        memcpy(pDstImg->ppPlane[0] + i * pDstImg->pLineSize[0],
                               pSrcImg->ppPlane[0] + i * pSrcImg->pLineSize[0], pDstImg->width * 4);
                    }
                } else {
                    memcpy(pDstImg->ppPlane[0], pSrcImg->ppPlane[0],
                           pSrcImg->pLineSize[0] * pSrcImg->height);
                }
                break;

            default:
                // 发生错误信息
                break;
        }
    }

    static void dumpNativeImage(NativeImage *pSrcImg, const char *pPath, const char *pFileName) {
        if (pSrcImg == nullptr || pPath == nullptr || pFileName == nullptr) {
            return;
        }
        if (access(pPath, 0) == -1) {
            mkdir(pPath, 0666);
        }
        char imgPath[256] = {0};
        const char *pExt = nullptr;
        switch (pSrcImg->format) {
            case IMAGE_FORMAT_I420:
                pExt = IMAGE_FORMAT_I420_EXT;
                break;
            case IMAGE_FORMAT_NV12:
                pExt = IMAGE_FORMAT_NV12_EXT;
                break;
            case IMAGE_FORMAT_NV21:
                pExt = IMAGE_FORMAT_NV21_EXT;
                break;
            case IMAGE_FORMAT_RGBA:
                pExt = IMAGE_FORMAT_RGBA_EXT;
                break;
            default:
                pExt = "Default";
                break;
        }
        static  int  index=0;

        FILE  *fp= fopen(imgPath,"wb");

        if (fp){
            switch (pSrcImg->format) {
                case IMAGE_FORMAT_I420:
                    fwrite(pSrcImg->ppPlane[0],static_cast<size_t>(pSrcImg->width*pSrcImg->height),1,fp);
                    fwrite(pSrcImg->ppPlane[1],static_cast<size_t>((pSrcImg->width>>1)*(pSrcImg->height>>1)),1,fp);
                    fwrite(pSrcImg->ppPlane[2],static_cast<size_t>((pSrcImg->width>>1)*(pSrcImg->height>>1)),1,fp);
                    break;

                case IMAGE_FORMAT_NV21:
                case IMAGE_FORMAT_NV12:
                    fwrite(pSrcImg->ppPlane[0],static_cast<size_t>(pSrcImg->width*pSrcImg->height),1,fp);
                    fwrite(pSrcImg->ppPlane[1],static_cast<size_t>((pSrcImg->width)*(pSrcImg->height>>1)),1,fp);
                    break;

                case IMAGE_FORMAT_RGBA:
                    fwrite(pSrcImg->ppPlane[0],static_cast<size_t>(pSrcImg->width*pSrcImg->height*4),1,fp);
                    break;
                default:
                // 图片保存失败
                    break;

            }

        }

        fclose(fp);
        fp= nullptr;
    }


};


#endif //FFMPEGDEMO_IMAGEDEF_H
