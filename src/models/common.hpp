#pragma once

#ifndef GLC_COMMON_HPP
#define GLC_COMMON_HPP

#include <string>
#include <vector>

namespace glc {
    std::string makeString(std::string path);
    std::string makeString(std::vector<std::string> strs, std::string delim="");
}


#endif
