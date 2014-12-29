#include "common.hpp"
#include "error.hpp"

#include <fstream>

std::string glc::makeString(std::string path)
{
    std::ifstream file(path);

    try
    {
        file.exceptions(std::ifstream::failbit);
    }
    catch (const std::ios_base::failure& err)
    {
        throw glc::MalformedFilePath(path);
    }

    file.seekg(0, std::ios::end);
    auto size = file.tellg();
    std::string buffer(size, ' ');
    file.seekg(0);
    file.read(&buffer[0], size);
    file.close();
    return buffer;
}
