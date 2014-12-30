#include "shader.hpp"
#include "common.hpp"
#include "error.hpp"

#include <glm/gtc/type_ptr.hpp>

namespace {
    const auto shaderType = std::unordered_map<std::string, GLenum>
    {
        {"vt", GL_VERTEX_SHADER   },
        {"fm", GL_FRAGMENT_SHADER }
    };

    GLuint makeShader(std::string path);
    GLenum makeShaderType(std::string path);
    GLvoid makeProgram(GLuint handle);
}

glc::Shader::Shader(std::vector<std::string> paths)
: mHandle(glCreateProgram()),
  mUniformCache(),
  mPaths(paths)
{
    auto srcHandles = std::vector<GLuint>();

    for (auto p : paths)
    {
        auto srcHandle = ::makeShader(p);
        glAttachShader(mHandle, srcHandle);
        srcHandles.emplace_back(srcHandle);
    }

    ::makeProgram(mHandle);

    for (auto sh : srcHandles)
    {
        glDeleteShader(sh);
    }
}

glc::Shader::~Shader()
{
    glDeleteProgram(mHandle);
}

void glc::Shader::use()
{
    glUseProgram(mHandle);
}

void glc::Shader::setUniform(std::string name, glm::vec3 value)
{
    const auto handle = this->getUniform(name);
    glUniform3f(handle, value.x, value.y, value.z);
}

void glc::Shader::setUniform(std::string name, glm::mat4 value)
{
    const auto handle = this->getUniform(name);
    glUniformMatrix4fv(handle, 1, GL_FALSE, glm::value_ptr(value));
}

void glc::Shader::setUniform(std::string name, glm::mat3 value)
{
    const auto handle = this->getUniform(name);
    glUniformMatrix3fv(handle, 1, GL_FALSE, glm::value_ptr(value));
}

void glc::Shader::setUniform(std::string name, GLfloat value)
{
    const auto handle = this->getUniform(name);
    glUniform1f(handle, value);
}

void glc::Shader::setUniform(std::string name, GLint value)
{
    const auto handle = this->getUniform(name);
    glUniform1i(handle, value);
}

GLuint glc::Shader::getUniform(std::string name)
{
    if (mUniformCache.count(name)) {
        return mUniformCache[name];
    }

    auto uniformHandle = glGetUniformLocation(mHandle, name.c_str());

    if (uniformHandle == -1) {
        throw glc::MalformedUniform(mPaths, name);
    }

    mUniformCache.emplace(name, uniformHandle);

    return uniformHandle;
}


namespace {
    GLuint makeShader(std::string path)
    {
        // Compile the shader.
        auto srcText = glc::makeString(path);
        auto srcHandle = glCreateShader(::makeShaderType(path));
        auto srcRawText = srcText.c_str();
        glShaderSource(srcHandle, 1, &srcRawText, nullptr);
        glCompileShader(srcHandle);

        // Check if shader content is not malformed.
        GLint success;
        glGetShaderiv(srcHandle, GL_COMPILE_STATUS, &success);
        if (! success)
        {
            GLchar errMsg[512];
            glGetShaderInfoLog(srcHandle, sizeof(errMsg), nullptr, errMsg);
            throw glc::MalformedShaderText(path, errMsg);
        }

        return srcHandle;
    }

    GLvoid makeProgram(GLuint handle)
    {
        glLinkProgram(handle);

        // Check if shader program has linked successfully.
        GLint success;
        glGetProgramiv(handle, GL_LINK_STATUS, &success);
        if (! success)
        {
            GLchar errMsg[512];
            glGetProgramInfoLog(handle, sizeof(errMsg), nullptr, errMsg);
            throw glc::MalformedShader(errMsg);
        }
    }

    GLenum makeShaderType(std::string path)
    {
        for (const auto& t : shaderType)
        {
            auto extension = path.substr(path.find_last_of("-"), path.size());
            auto toBeMatched = "-" + t.first + ".glsl";

            if (extension == toBeMatched)
            {
                return t.second;
            }
        }

        throw glc::MalformedShaderName(path);
    }
}
