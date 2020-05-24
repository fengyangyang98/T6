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

inline size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;

        pos = txt.find( ch, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

inline std::string getTime()
{
    time_t timep;
    time (&timep); 
    char tmp[64];
    strftime(tmp, sizeof(tmp), "[%Y-%m-%d %H:%M:%S] ",localtime(&timep) );
    return tmp;
}


#endif