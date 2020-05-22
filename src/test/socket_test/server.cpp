#include "../../include/core.hpp"
#include "../../third_party/ossSocket/ossSocket.hpp"

int main()
{
    int clientSocket;
    sockaddr ip;
    int rc;

    ossSocket server(8001);

    server.initSocket();
    server.bind_listen();
    while(1) 
    {
        rc = server.accept((SOCKET *)&clientSocket, NULL, NULL, 10000);
        if(rc == KV_TIMEOUT) continue;
        else break;
    }

    server.close();
    ossSocket * client;
    client = new ossSocket((SOCKET *)&clientSocket);

    char * a = new char[30];
    client->getPeerAddress(a, 30);
    std::cout << a << " : "<< client->getPeerPort() << std::endl;
    delete[] a;

    char * p = new char[100];
    memset(p, 0, 100);
    int len = 100;
    std::cout << client->recvNF(p, len, 100000) << std::endl;
    std::string msg(p, len);

    std::cout << msg << " " << len << std::endl;

    std::cout<<client->send(msg.c_str(), msg.length(), 100000) << std::endl;
    
    delete client;
    // delete[] p;
}