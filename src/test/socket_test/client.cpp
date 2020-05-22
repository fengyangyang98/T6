#include "../../include/core.hpp"
#include "../../third_party/ossSocket/ossSocket.hpp"

int main()
{
    std::string add = "127.0.0.1";
    ossSocket client;
    client = ossSocket(add.c_str(), 8001);
    client.initSocket();
    while( client.connect() ) {

    }
    
    std::string msg = "1234560";
    std::cout<<client.send(msg.c_str(), msg.length(), 100000000) << std::endl;
    sleep(3);
    char * p = new char[10];
    while(client.recv(p, 10, 100000) != KV_NETWORK_CLOSE) {
        
        std::cout<<client.recv(p, 10, 100000) << std::endl;
        std::string msg2(p, 10);
        std::cout << msg2;
    }
}