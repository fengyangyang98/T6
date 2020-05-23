#include "core.hpp"
#include "coordinator.hpp"
#include "participant.hpp"
#include "conf.hpp"

int main(int argc, char ** argv)
{   

    if(argc == 1) {

        std::string add = "127.0.0.1";
        ossSocket client;
        std::string msg;
        Parser p;

        while(getline(std::cin, msg)) {
            client = ossSocket(add.c_str(), 8001);

            client.initSocket();
            while( client.connect() ) { }

            std::vector<std::string> list;
            split(msg, list, ' ');       

            std::string pac = p.getRESPArry(list);
            client.send(pac.c_str(), pac.length(), 100000000);

            char * p = new char[1024];
            memset(p, 0, 1024);
            int len = 1024;
            while(client.recvNF(p, len, 100000)) {}

            std::string msg2(p, len);
            std::cout << msg2;

            delete[] p;
            msg.clear();
        }

        
        
        return 0;
    }

    std::vector<NodeInfo>   pinfo;
    NodeInfo                cinfo;
    Mode                    m = MODE_INVALID;

    std::string conf = GetOptLong(argc, argv);
    if( !readConf(conf, pinfo, cinfo, m) ) {
        std::cout << "> ERROR: CHECK THE CONFIGURE FILE. " << std::endl;
        return 0;
    }

    if( m == MODE_INVALID ) {
        std::cout << "> ERROR: UNDEFINE THE TYPE OF THE NODE. " << std::endl;
        return 0;
    }

    if(m == MODE_C) {
        Coordinator c;
        c.Init(pinfo, cinfo);
        c.Launch();
    } else if( m == MODE_P) {
        Participant p;
        if(pinfo.size() != 1) {
            std::cout << "> ERROR: UNDEFINE THE PARTICIPANT. " << std::endl;
            return 0;
        }
        p.Init(pinfo[0]);
        p.Launch();
    }

    return 0;
}