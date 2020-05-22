#include "core.hpp"
#include "network.hpp"
#include "util.hpp"
/*
    The functions below are designed for the participant.
    Cause the participant behavior is that:
        1. recv from the coordinator
        2. excute the content
        3. send the result
    
    Therefore, the participant will rev until the coordinator close
    the socket and ignore the timeout and network error, focusing on
    the nework_close error only.
*/

int Network::reaccept()
{
    int rc = KV_OK;
    int clientSocket;

    // bind the port
    ossSocket server(_port);
    rc = server.initSocket();
    if(rc) goto done;
    rc = server.bind_listen();
    if(rc) goto done;

    // accept a new socket per 1 second
    while(1) {
        rc = server.accept((SOCKET *)&clientSocket, NULL, NULL, NETWORK_TIMEOUT);
        if(rc == KV_TIMEOUT) continue;
        else break;
    }

    // close the bind
    server.close();

    if(rc) goto done;

    // packet the socket
    _peer = new ossSocket((SOCKET *)&clientSocket);
   
done:
    return rc;
}

int Network::send(std::string msg)
{
    int rc = KV_OK;
    // pack the msg

    std::string pac = "~";
    uint64_t length = msg.length();
    pac += numtob(length);
    pac += msg;

    rc = _peer->send(pac.c_str(), pac.length());

    return rc;
}

/*
    recv:
        the participant does not handle the timeout and network problem,
        when network_close occurs, break.
*/
int Network::recv(std::string & content)
{
    int rc = KV_OK;

    std::string headerBuf;

    std::string header = "";
    int headerLen = 0;
    content.clear();
    content = "";

    uint64_t contentLen = 0;

    char * headerptr = new char[NETWORK_HEADER_LEN];
    memset(headerptr, 0, NETWORK_HEADER_LEN);
    char * contentptr = nullptr;

    // recv the header of the pck
    while(headerLen != NETWORK_HEADER_LEN)
    {
        size_t pos = 0;
        int recvLen = NETWORK_HEADER_LEN - headerLen;

        rc = _peer->recvNF(headerptr, recvLen);
        if(rc == KV_NETWORK_CLOSE) goto done;
        
        if(rc) continue;

        std::cout << rc << std::endl;
        
        if(headerLen == 0) {
            headerBuf.assign(headerptr, recvLen);
            if(headerBuf.find_first_of('~') != std::string::npos) {
                pos = headerBuf.find_first_of('~');
                header = headerBuf.substr(pos, recvLen - pos);
                headerLen += recvLen - pos;
            }
            headerBuf.clear();
        } else {
            header += std::string(headerptr, recvLen);
            headerLen += recvLen;
        }
    }

    contentLen = btonum<uint64_t>(header, 1);
    contentptr = new char[contentLen];

    // recv the rest content
    while(content.length() != contentLen)
    {
        size_t recvLen = contentLen - content.length();

        rc = _peer->recv(contentptr, recvLen);
        if(rc == KV_NETWORK_CLOSE) goto done;
        
        if(rc) continue;
        
        content += std::string(contentptr, recvLen);
    }

done:
    delete[] headerptr;
    if(contentptr) delete[] contentptr;
    return rc;
}


/*
    The functions below are designed for the coordinator.
    Cause the coordinator's behavior is like that:
        1. send the msg
        2. recv the msg
    
    Therefor the coofdinator should be responsible for the error
    handling. If there is a timeout or the network_close error, the
    coordinator will judge the participant breaks down. The handle 
    process is like this:
        1. close the previous socket
        2. try to rebuild a new socket
*/
int Network::reconnect()
{
    int rc = KV_OK;

    _peer = new ossSocket(_peerAddress.c_str(), _peerPort);  
    rc = _peer->initSocket();

    // try to connect the coordinator
    while(1) { 
        rc = _peer->connect();
        if(!rc) break;
    }

    return rc;
}

int Network::sendAndRecv(std::string smsg, std::string & rmsg)
{
    int rc = KV_OK;

    if(!_peer->isConnected()) {
        std::cout << "Lost the connection\n";
        return KV_NETWORK;
    }

    std::string headerBuf;
    std::string header = "";
    uint64_t contentLen = 0;

    char * headerptr = new char[NETWORK_HEADER_LEN];
    int headerLen = 0;
    memset(headerptr, 0, NETWORK_HEADER_LEN);
    char * contentptr = nullptr;

    // send 
    uint64_t length = smsg.length();
    std::string pac = "~";
    pac += numtob(length);
    pac += smsg;

    rc = _peer->send(pac.c_str(), pac.length());
    if(rc) goto done;

    // recv
    // recv the header of the pck
    while(headerLen != NETWORK_HEADER_LEN)
    {
        size_t pos = 0;
        size_t recvLen = NETWORK_HEADER_LEN - header.length();

        rc = _peer->recv(headerptr, recvLen, NETWORK_TIMEOUT);
        if(rc) goto done;
        
        if(header.length() == 0) {
            headerBuf.assign(headerptr, recvLen);
            if(headerBuf.find_first_of('~') != std::string::npos) {
                pos = headerBuf.find_first_of('~');
                header = headerBuf.substr(pos, recvLen - pos);
                headerLen += recvLen - pos;
            }
            headerBuf.clear();
        } else {
            header += std::string(headerptr, recvLen);
            headerLen += recvLen;
        }
    }

    contentLen = btonum<uint64_t>(header, 1);
    if(contentLen == 0) goto done;
    
    contentptr = new char[contentLen];

    rmsg.clear();
    rmsg = "";

    // recv the rest content
    while(rmsg.length() != contentLen)
    {
        int recvLen = contentLen - rmsg.length();

        rc = _peer->recvNF(contentptr, recvLen, NETWORK_TIMEOUT);
        if(rc) goto done;

        rmsg += std::string(contentptr, recvLen);
    }

done:
    delete[] headerptr;
    if(contentptr) delete[] contentptr;
    return rc;
}