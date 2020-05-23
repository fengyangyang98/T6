#include "core.hpp"
#include "parser.hpp"

void parserTest()
{
    Parser p;
    std::string tmp  = "";
    std::vector<std::string> tmpA;

    std::cout << "[TEST] getStringMessage\n";
    std::cout << "[RAW ] CS06142\n";
    tmp = "CS06142";
    std::cout << p.getStringMessage(tmp) << "\n\n";

    std::cout << "[TEST] getSuccessMessage\n";
    std::cout << "[RAW ]\n";
    std::cout << p.getSuccessMessage() << "\n\n";

    std::cout << "[TEST] getErrorMessage\n";
    std::cout << "[RAW ]\n";
    std::cout << p.getErrorMessage() << "\n\n";

    std::cout << "[TEST] getIntergerMessage\n";
    std::cout << "[RAW ] 201708010314\n";
    std::cout << p.getIntergerMessage(8010314) << "\n\n";

    std::cout << "[TEST] getRESPArry\n";
    std::cout << "[RAW ] Cloud Computing\n";
    tmpA.push_back("Cloud");
    tmpA.push_back("Computing");
    std::cout << p.getRESPArry(tmpA) << "\n\n";

    std::cout << "[TEST] parseStringMessage\n";
    std::cout << "[RAW ] $7\\r\\nCS06142\\r\\n \n";
    tmp = "$7\r\nCS06142\r\n";
    std::cout << p.parseStringMessage((char *)tmp.c_str()) << "\n\n";

    std::cout << "[TEST] parserIntergerMessage\n";
    std::cout << "[RAW ] :10\\r\\n\n";
    tmp = ":10\r\n";
    std::cout << p.parserIntergerMessage((char *)tmp.c_str()) << "\n\n";

    std::cout << "[TEST] parserSuccessMessage\n";
    std::cout << "[RAW ]\n";
    tmp = "+OK\r\n";
    std::cout << p.parserSuccessMessage((char *)tmp.c_str()) << "\n\n";
    tmp = "-ERROR\r\n";
    std::cout << p.parserSuccessMessage((char *)tmp.c_str()) << "\n\n";

    std::cout << "[TEST] parserRESPArry\n";
    std::cout << "[RAW ]\n";
    tmp = "*2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n";
    tmpA.clear();
    // tmpA = p.parserRESPArry((char *)tmp.c_str());

    // for(auto s : tmpA) 
    // {
    //     std::cout << s << std::endl;
    // }
}
