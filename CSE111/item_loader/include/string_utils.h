
#pragma once

#include <string>

inline bool ends_with(std::string const & str, std::string const & en)
{
    if (en.size() > str.size()) return false;
    return std::equal(en.rbegin(), en.rend(), str.rbegin());
}
