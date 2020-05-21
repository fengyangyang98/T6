#ifndef KV_HPP_
#define KV_HPP_

#include "core.hpp"
#include "parser.hpp"
#include "phmap.h"

using phmap::parallel_flat_hash_map;

class KV
{
private:
    parallel_flat_hash_map<std::string, std::string> _kvdb;

public:

    ~KV();
    /*
        kvGET command can get the value of key.
        If the key does not exist the special value nil is returned.
    */
    std::string kvGET(std::string & k);

    /*
        The kvDEL is used for removing one or more 
        specified keys (arbitrary number, up to 512 MB message length). 
        A key is ignored if it does not exist.

        @return the number of deleting items
    */
    int kvDEL(std::string & k);
    int kvDELArry(std::vector<std::string> & ks);

    /*
        The function of the kvSET is to set key to hold the string value. 
        If key already holds a value, it is overwritten.
    */
    int kvSET(std::string & k, std::string & v);
    


};

#endif