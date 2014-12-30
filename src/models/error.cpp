#include "error.hpp"
#include "common.hpp"

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

glc::MalformedUniform::MalformedUniform(
    const std::vector<std::string>& paths,
    const std::string& name)
: std::runtime_error("Uniform doesn't exist: "+name+"\n  In: "+glc::makeString(paths, ", "))
{

}
