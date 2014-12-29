#pragma once

#ifndef GLC_ERROR_HPP
#define GLC_ERROR_HPP

#include <stdexcept>

namespace glc {
    class MalformedShaderName : public std::runtime_error
    {
    public:
        explicit MalformedShaderName(const std::string& path);
    };

    class MalformedShaderText : public std::runtime_error
    {
    public:
        explicit MalformedShaderText(const std::string& path, const std::string& msg);
    };

    class MalformedShader : public std::runtime_error
    {
    public:
        explicit MalformedShader(const std::string& msg);
    };

    class MalformedFilePath : public std::runtime_error
    {
    public:
        explicit MalformedFilePath(const std::string& path);
    };

    class MalformedModel : public std::runtime_error
    {
    public:
        explicit MalformedModel(const std::string& path, const std::string& msg);
    };
}

#endif
