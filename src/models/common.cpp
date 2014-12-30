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

std::string glc::makeString(std::vector<std::string> strs, std::string delim)
{
    std::string str = "";
    for (size_t i = 0; i < strs.size(); i++)
    {
        str += strs[i];
        str += (i != strs.size() - 1) ? delim : "";
    }

    return str;
}
