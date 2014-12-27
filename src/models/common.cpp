#include "common.h"
#include <FreeImagePlus.h>
#include <iostream>
#include <fstream>

void glc::printErr(int code, const char* desc)
{
    std::cout << desc << "\n";
}

void glc::printShaderStatus(GLuint shader)
{
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (! success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << infoLog << "\n";
    }
}

GLuint glc::makeVShader(std::string path)
{
    auto shader = glCreateShader(GL_VERTEX_SHADER);
    auto shaderText = makeString(path);
    auto shaderRawText = shaderText.c_str();
    glShaderSource(shader, 1, &shaderRawText, nullptr);
    glCompileShader(shader);

    return shader;
}

GLuint glc::makeFShader(std::string path)
{
    auto shader = glCreateShader(GL_FRAGMENT_SHADER);
    auto shaderText = makeString(path);
    auto shaderRawText = shaderText.c_str();
    glShaderSource(shader, 1, &shaderRawText, nullptr);
    glCompileShader(shader);

    return shader;
}

GLuint glc::makeProgram(std::vector<GLuint> shaders)
{
    GLuint program = glCreateProgram();

    for (auto s : shaders)
        glAttachShader(program, s);

    glLinkProgram(program);

    return program;
}

GLuint glc::makeTexture(std::string path)
{
    GLuint id;
    glGenTextures(1, &id);

    fipImage image;
    image.load(path.c_str());
    image.convertTo32Bits();
    auto data = image.accessPixels();
    auto w = image.getWidth();
    auto h = image.getHeight();

    auto minSetting = GL_LINEAR_MIPMAP_LINEAR;
    auto magSetting = GL_LINEAR;

    glBindTexture(GL_TEXTURE_2D,id);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_BGRA,GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minSetting);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magSetting);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    image.clear();

    return id;
}

GLuint glc::makeMesh(std::vector<GLfloat> vertices)
{
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLuint vao;
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    auto vtSize = vertices.size() * sizeof(GLfloat);
    auto vtData = vertices.data();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vtSize, vtData, GL_STATIC_DRAW);

    auto stride = sizeof(GLfloat) * 8;
    auto vertexOff = (GLvoid*)(0);
    auto normalOff = (GLvoid*)(sizeof(GLfloat) * 3);
    auto texOff = (GLvoid*)(sizeof(GLfloat) * 6);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, vertexOff);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, normalOff);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, texOff);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return vao;
}

std::string glc::makeString(std::string path)
{
    std::ifstream file(path);

    try
    {
        file.exceptions(std::ifstream::failbit);
    }
    catch (const std::ios_base::failure& err)
    {
        throw std::ios_base::failure(path + " is not available.");
    }

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    std::string buffer(size, ' ');
    file.seekg(0);
    file.read(&buffer[0], size);
    file.close();
    return buffer;
}