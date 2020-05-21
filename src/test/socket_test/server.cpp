#include "../../include/core.hpp"
#include "../../third_party/ossSocket/ossSocket.hpp"

int main()
{
    int clientSocket;
    sockaddr ip;

    ossSocket server("127.0.0.1", 8001);

    server.initSocket();
    server.bind_listen();
    server.accept((SOCKET *)&clientSocket, NULL, NULL, 10000000);

    ossSocket client((SOCKET *)&clientSocket);

    char * p = new char[10];
    client.recv(p, 10, 10000000);
    std::string msg(p, 10);
    std::cout << msg;

    delete[] p;
}