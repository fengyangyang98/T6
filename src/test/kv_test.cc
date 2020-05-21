#include "kv.hpp"

void kvTest()
{
    KV kvdb;

    std::string k;
    std::string v;
    std::vector<std::string> kl;

    std::cout << "[SET]" << "\n";
    for(int i = 0; i <= 100; i++) {
        k = "number key" + std::to_string(i * i);
        v = "number value" + std::to_string(i);

        kvdb.kvSET(k, v);
    }

    std::cout << "[GET]" << "\n";
    for(int i = 0; i <= 100; i++) {
        k = "number key" + std::to_string(i * i);
        std :: cout << kvdb.kvGET(k) << "\n";
    }

    std::cout << "\n[GET] nil" << "\n";
    for(int i = 100; i <= 120; i++) {
        k = "number key" + std::to_string(i * i);
        std :: cout << kvdb.kvGET(k) << "\n";
    }

    std::cout << "\n[SET] overwrite" << "\n";
    for(int i = 0; i <= 20; i++) {
        k = "number key" + std::to_string(i * i);
        v = "overwrite number value" + std::to_string(i);
        kvdb.kvSET(k, v);
    }

    std::cout << "\n[GET] overwrite" << "\n";
    for(int i = 0; i <= 20; i++) {
        k = "number key" + std::to_string(i * i);

        std :: cout << kvdb.kvGET(k) << "\n";
    }

    std::cout << "\n[DEL]" << "\n";
    for(int i = 50; i <= 120; i++) {
        kl.push_back("number key" + std::to_string(i * i));
    }
    std::cout << "del number: " << kvdb.kvDELArry(kl) << "\n"; 

    std::cout << "\n[GET]" << "\n";
    for(int i = 0; i <= 100; i++) {
        k = "number key" + std::to_string(i * i);
        std :: cout << kvdb.kvGET(k) << "\n";
    }

}