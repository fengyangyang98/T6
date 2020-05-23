#include "core.hpp"
#include "coordinator.hpp"
#include "participant.hpp"
#include "conf.hpp"

int main(int argc, char ** argv)
{   

    if(argc == 1) {
        std::string msg;
        while(std::cin >> msg) {
            std::vector<std::string> list;
            split(msg, list, ' ');
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