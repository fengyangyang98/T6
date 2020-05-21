#ifndef PARSER_HPP
#define PARSER_HPP

#include "core.hpp"

class Parser
{
private:
public:
    /*
        @param raw the raw string you want to warp into the string message
        @return a string message
            e.g. CS06142 -> $7\r\nCS06142\r\n
    */
    std::string getStringMessage(std::string & raw);

    /*
        @return a success message
    */
    std::string getSuccessMessage();

    /*
        @ return an error message
    */
    std::string getErrorMessage();

    /*
        @param raw the raw interger you wanna warp into the interger message
        @return a interger message
            e.g. 10 -> :10\r\n
    */
    std::string getIntergerMessage(int raw);

    /*
        @param raw the raw string vactor you wanna warp into the RESP message
        @return a string vactor message
            e.g. Cloud Computing-> *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    */
    std::string getRESPArry(std::vector<std::string> & raw);

    /*
        @para message the raw message recieved
        @return the string parsered from message
            e.g. CS06142 <- $7\r\nCS06142\r\n
    */
    std::string parseStringMessage(char * message, int * length=nullptr);

    /*
        @para message the raw message recieved
        @return the interger parsered from message
            e.g. 10 <- :10\r\n
    */
    int parserIntergerMessage(char * message, int * length=nullptr);

    /*
        @para message the raw message recieved
        @return the status of the excution
            e.g. +OK\r\n -> true
                 -ERROR\r\n -> false
    */
    bool parserSuccessMessage(char * message, int * length=nullptr);

    /*
        @para message the raw message recieved
        @return the string vector parsered from message
            e.g. Cloud Computing <- *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    */
    std::vector<std::string> parserRESPArry(char * message, int * length=nullptr);


};


#endif