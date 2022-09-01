//
// Created by WJ on 2022/8/30.
//

#include "GLUtils.h"
#include <cstdlib>
#include <cstring>
#include <GLES2/gl2ext.h>
#include "LogUtil.h"


GLuint GLUtils::loadShader(GLenum shaderType, const char *pSource) {
    GLuint shader = 0;
    FUN_BEGIN_TIME("GLUtils::loadShader")
        shader = glCreateShader(shaderType);
        if (shader) {
            glShaderSource(shader, 1, &pSource, nullptr);
            glCompileShader(shader);
            GLint compiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
            if (!compiled) {
                GLint infoLen = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
                if (infoLen) {
                    char *buf = (char *) malloc((size_t) infoLen);
                    if (buf) {
                        glGetShaderInfoLog(shader, infoLen, nullptr, buf);
                        free(buf);
                    }
                    glDeleteShader(shader);
                    shader = 0;
                }
            }
        }

    FUN_END_TIME("GLUtils::loadShader")
    return shader;
}

GLuint GLUtils::createProgram(const char *pVertexShaderSource, const char *pFragShaderSource,
                              GLuint &vertexShaderHandle, GLuint &fragShaderHandle) {
    GLuint program = 0;
//    FUN_BEGIN_TIME("GLUtils::createProgram");
    vertexShaderHandle = loadShader(GL_VERTEX_SHADER, pVertexShaderSource);
    if (!vertexShaderHandle) {
        return program;
    }
    fragShaderHandle = loadShader(GL_FRAGMENT_SHADER, pFragShaderSource);
    if (!fragShaderHandle) {
        return program;
    }
    program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShaderHandle);
        checkGLError("glAttachShader");
        glAttachShader(program, fragShaderHandle);
        checkGLError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        glDetachShader(program, vertexShaderHandle);
        glDeleteShader(vertexShaderHandle);
        vertexShaderHandle = 0;
        glDetachShader(program, fragShaderHandle);
        glDeleteShader(fragShaderHandle);
        fragShaderHandle = 0;
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char *buf = (char *) malloc((size_t) bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, nullptr, buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint GLUtils::createProgram(const char *pVertexShaderSource, const char *pFragShaderSource) {
    GLuint vertexShaderHandle, fragShaderHandle;
    return createProgram(pVertexShaderSource, pFragShaderSource, vertexShaderHandle, fragShaderHandle);
}

GLuint
GLUtils::createProgramWithFeedback(const char *pVertexShaderSource, const char *pFragShaderSource,
                                   GLuint &vertexShaderHandle, GLuint &fragShaderHandle,
                                   const GLchar **varying, int varyingCount) {
    GLuint program = 0;
    vertexShaderHandle = loadShader(GL_VERTEX_SHADER, pVertexShaderSource);
    if (!vertexShaderHandle) {
        return program;
    }
    fragShaderHandle = loadShader(GL_FRAGMENT_SHADER, pFragShaderSource);
    if (!fragShaderHandle) {
        return program;
    }
    program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShaderHandle);
        checkGLError("glAttachShader");
        glAttachShader(program, fragShaderHandle);
        checkGLError("glAttachShader");

        glTransformFeedbackVaryings(program, varyingCount, varying, GL_INTERLEAVED_ATTRIBS);
        GO_CHECK_GL_ERROR();

        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        glDetachShader(program,vertexShaderHandle);
        glDeleteShader(vertexShaderHandle);
        vertexShaderHandle=0;
        glDetachShader(program,fragShaderHandle);
        glDeleteShader(fragShaderHandle);
        fragShaderHandle=0;
        if (linkStatus!=GL_TRUE){
            GLint  bufLength=0;
            glGetProgramiv(program,GL_INFO_LOG_LENGTH,&bufLength);
            if (bufLength){
                char  *buf=(char *) malloc((size_t)bufLength);
                if (buf){

                    glGetProgramInfoLog(program,bufLength, nullptr,buf);
                    LOGCATE("GLUtils::CreateProgramWithFeedback Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }

            glDeleteProgram(program);
            program=0;
        }
    }
    LOGCATE("GLUtils::CreateProgramWithFeedback program = %d", program);
    return program;
}

void GLUtils::deleteProgram(GLuint &program) {
    LOGCATE("GLUtils::DeleteProgram");
    if (program){
        glUseProgram(0);
        glDeleteProgram(program);
        program=0;
    }
}

void GLUtils::checkGLError(const char *pGLOperation) {
    for(GLint error=glGetError();error;error=glGetError()){
        LOGCATE("GLUtils::CheckGLError GL Operation %s() glError (0x%x)\n", pGLOperation, error);
    }
}

