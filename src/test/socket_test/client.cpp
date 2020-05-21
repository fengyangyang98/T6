#include "../../include/core.hpp"
#include "../../third_party/ossSocket/ossSocket.hpp"

int main()
{
    ossSocket client("127.0.0.1", 8001);
    client.initSocket();
    client.connect();

    std::string msg = "1234567890";

    client.send(msg.c_str(), 10, 10000000);
}