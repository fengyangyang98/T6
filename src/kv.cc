#include "core.hpp"
#include "kv.hpp"

KV:: ~KV()
{
    _kvdb.clear();
}

/*
    The function of the kvSET is to set key to hold the string value. 
    If key already holds a value, it is overwritten.
*/
int KV::kvSET(std::string & k, std::string & v)
{
    _kvdb[k] = v;
    return KV_OK;
}

/*
    The kvDEL is used for removing one or more 
    specified keys (arbitrary number, up to 512 MB message length). 
    A key is ignored if it does not exist.

    @return the number of deleting items
*/
int KV::kvDEL(std::string & k)
{
    auto iter = _kvdb.find(k);
    if(iter != _kvdb.end()) {
        _kvdb.erase(iter);
        return KV_OK;
    } else {
        return KV_DEL;
    }
}

int KV::kvDELArry(std::vector<std::string> & ks)
{
    int number = 0;
    for(auto & k : ks)
    {
        if(kvDEL(k) == KV_OK)
            number ++;
    }
    return number;
}

/*
    kvGET command can get the value of key.
    If the key does not exist the special value nil is returned.
*/
std::string KV::kvGET(std::string & k)
{
    auto iter = _kvdb.find(k);
    if(iter == _kvdb.end()) return "nil";
    else return iter->second;
}