#include "texture.h"

#include <GL/glew.h>
#include <FreeImagePlus.h>
#include <string>

glc::TexLoader::TexLoader() {}

GLuint glc::TexLoader::load(std::string path)
{
    GLuint id;
    glGenTextures(1, &id);

    m_container.load(path.c_str());
    m_container.convertTo32Bits();
    auto data = m_container.accessPixels();
    auto w = m_container.getWidth();
    auto h = m_container.getHeight();

    auto minSetting = GL_LINEAR_MIPMAP_LINEAR;
    auto magSetting = GL_LINEAR;

    glBindTexture(GL_TEXTURE_2D,id);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_BGRA,GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minSetting);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magSetting);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
    m_container.clear();

    return id;
}