#pragma once

#ifndef GLC_SHADER_HPP
#define GLC_SHADER_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace glc
{
    class Shader
    {
    public:
         Shader(std::vector<std::string> paths);
        ~Shader();

        void use();
        void setUniform(std::string name, glm::vec3 value);
        void setUniform(std::string name, glm::mat4 value);
        void setUniform(std::string name, GLfloat value);
        void setUniform(std::string name, GLint value);
    private:
        const GLuint mHandle;
        std::unordered_map<std::string, GLuint> mUniformCache;

        // Helper Methods
        GLuint getUniform(std::string name);
    };
}

#endif