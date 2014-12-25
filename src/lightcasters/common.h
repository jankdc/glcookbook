#pragma once

#ifndef GLC_COMMON_H
#define GLC_COMMON_H

#include <GL/glew.h>
#include <string>
#include <vector>

namespace glc {
    void printErr(int code, const char* desc);
    void printShaderStatus(GLuint shader);
    std::string makeString(std::string path);
    GLuint makeMesh(std::vector<GLfloat> vertices);
    GLuint makeShader(GLenum shaderType, std::string text);
    GLuint makeVShader(std::string path);
    GLuint makeFShader(std::string path);
    GLuint makeProgram(std::vector<GLuint> shaders);
}


#endif