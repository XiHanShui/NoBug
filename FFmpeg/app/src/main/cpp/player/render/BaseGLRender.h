//
// Created by WJ on 2022/8/10.
//

#ifndef FFMPEGDEMO_BASEGLRENDER_H
#define FFMPEGDEMO_BASEGLRENDER_H

#include "ImageDef.h"

#define  VIDEO_GL_RENDER  0
#define  AUDIO_GL_RENDER  1
#define  VR_3D_RENDER     2

struct TransformMatrix{
    int degree;
    int mirror;
    float  translateX;
    float  translateY;
    float  scaleX;
    float  scaleY;
    float  angleX;
    float  angleY;

    TransformMatrix():translateX(0),translateY(0),scaleX(1.0),scaleY(1.0),degree(0),mirror(0),angleX(0),angleY(0){

    }
    void reset(){
        degree=0;
        mirror=0;
        translateX=0;
        translateY=0;
        scaleX=1.0;
        scaleY=1.0;
        angleX=0;
        angleY=0;
    }

};

class  BaseGLRender{

public:
    virtual  ~BaseGLRender(){};
    virtual  void  onSurfaceCreated()=0;
    virtual  void  onSurfaceChanged(int w,int h)=0;
    virtual  void  onDrawFrame()=0;
    virtual  void  updateMVPMatrix(int angleX,int angleY,float  scaleX,float  scaleY)=0;
    virtual  void  updateMVPMatrix(TransformMatrix *pTransformMatrix){};
    virtual  void  setTouchLoc(float  touchX,float  touchY)=0;


};



#endif //FFMPEGDEMO_BASEGLRENDER_H
