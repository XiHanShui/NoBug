//
// Created by WJ on 2022/8/10.
//

#ifndef FFMPEGDEMO_AUDIOGLRENDER_H
#define FFMPEGDEMO_AUDIOGLRENDER_H

#include <thread>
#include "AudioRender.h"
#include <GLES3/gl3.h>
#include <detail/type_mat.hpp>
#include <detail/type_mat4x4.hpp>
#include <render/BaseGLRender.h>

class AudioGLRender : public BaseGLRender {
public:
    static AudioGLRender *getInstance();

    static void releaseInstance();

    virtual void onSurfaceCreated();

    virtual void onSurfaceChanged(int w, int h);

    virtual void onDrawFrame();

    virtual void updateMVPMatrix(int angleX, int angleY, float scaleX, float scaleY) {};

    virtual void updateMVPMatrix(TransformMatrix *pTransformMatrix) {};

    virtual void setTouchLoc(float touchX, float touchY) {};

    void updateAudioFrame(AudioFrame *audioFrame);


private:
    void  init();
    void  unInit();
    AudioGLRender(){
        init();
    };
    ~AudioGLRender(){
        unInit();
    };

    void  updateMesh();

    static  AudioGLRender *m_pInstance;
    static  std::mutex m_Mutex;
    AudioFrame *m_pAudioBuffer= nullptr;

    GLuint  m_ProgramObj=0;
    GLuint  m_VaoId;
    GLuint  m_VboIds[2];
    glm::mat4 m_MVPMatrix;

    glm::vec3 *m_pVerticesCoords= nullptr;
    glm::vec2  *m_pTextureCoords= nullptr;
    int m_RenderDataSize;
};


#endif //FFMPEGDEMO_AUDIOGLRENDER_H
