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
    
    std::string msg = "*2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n";
    std::cout<<client.send(msg.c_str(), msg.length(), 100000000) << std::endl;

    char * p = new char[200];
    int len = 200;
    while(client.recvNF(p, len, 100000)) {}

    std::string msg2(p, len);
    std::cout << msg2;
}