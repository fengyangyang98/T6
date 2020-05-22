#ifndef UTIL_HPP_
#define UTIL_HPP_

#include "core.hpp"

template<class T>
inline T btonum(std::string str, size_t offset)
{
    T rc;
    memcpy((char *)&rc, &( ((char*)str.c_str()) [offset]), sizeof(T));
    return rc;
}

template<class T>
inline std::string numtob(T number)
{
    std::string rc;
    char * b = new char[sizeof(T)];
    memcpy((char*)b, (char *)&number, sizeof(T));
    
    rc.assign(b, sizeof(T));
    delete[] b;

    return rc;
}

#endif