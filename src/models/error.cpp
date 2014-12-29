#include "error.hpp"

#include <string>

glc::MalformedShaderName::MalformedShaderName(const std::string& path)
: std::runtime_error(path)
{

}

glc::MalformedShaderText::MalformedShaderText(const std::string& path, const std::string& msg)
: std::runtime_error(path + std::string {"\n"} + msg)
{

}

glc::MalformedShader::MalformedShader(const std::string& msg)
: std::runtime_error(msg)
{

}

glc::MalformedFilePath::MalformedFilePath(const std::string& path)
: std::runtime_error(std::string {"Path doesn't exist: "} + path)
{

}

glc::MalformedModel::MalformedModel(const std::string& path, const std::string& msg)
: std::runtime_error(path + std::string {": "} + msg)
{

}
