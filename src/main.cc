#include "core.hpp"
#include "test.hpp"
#include "util.hpp"


int main(int argc, char ** argv)
{    
    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            switch (argv[i][1])
            {
                case 'c':
                    network_client_test();
                    break;
                case 's':  
                    network_server_test();
                    break;
                default:
                    break;
            }
        }
    }
}