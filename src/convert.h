#ifndef __CONVERT_H__
#define __CONVERT_H__

#include <string>
#include <sstream>
#include <stdexcept>

namespace blobdetective {

/**
 * Converts a string to an int, a float or a bool.
 */
template<typename T>
T from_string(const std::string& str)
{
    std::istringstream ss(str);
    T ret;
    if ((ss >> ret).fail())
    {
        throw std::runtime_error(str);
    }
    return ret;
}

} // end of namespace

#endif // include guard

