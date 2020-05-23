#include "core.hpp"
#include "test.hpp"
#include "util.hpp"
#include "coordinator.hpp"

class hello
{
private:
    std::thread t[10];
    int number = 0;
public:

    int sayHello(int i) {
        sleep(1);
        std::cout << number << "--" << i << ": Hello" << std::endl;
        sleep(1);
        return i;
    }

    int launch(int i, int n) 
    {
        number = n;
        for(int k = 0; k < i; k++) {
            t[k] = std::thread(&hello::sayHello,this, k );
        }
        return 0;
    }

    ~hello() {
        for(int k = 0; k < 10; k++) {
            t[k].join();
        }
    }
};

int main(int argc, char ** argv)
{    
    // for (int i = 1; i < argc; i++)
    // {
    //     if (argv[i][0] == '-')
    //     {
    //         switch (argv[i][1])
    //         {
    //             case 'c':
    //                 network_client_test();
    //                 break;
    //             case 's':  
    //                 network_server_test();
    //                 break;
    //             default:
    //                 break;
    //         }
    //     }
    // }

    loggerTest();
    
}