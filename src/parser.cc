#include "parser.hpp"
/*
        @param raw the raw string you want to warp into the string message
        @return a string message
            e.g. CS06142 -> $7\r\nCS06142\r\n
    */
std::string Parser::getStringMessage(std::string & raw)
{
    std::string rc = "$";
    int length = (int) raw.length();
    rc += std::to_string(length);
    rc += "\r\n";
    rc += raw;
    rc += "\r\n";
    return rc;
}

/*
        @return a success message
    */
std::string Parser::getSuccessMessage()
{
    std::string rc = "+OK\r\n";
    return rc;
}

/*
        @ return an error message
    */
std::string Parser::getErrorMessage()
{
    std::string rc = "-ERROR\r\n";
    return rc;
}

/*
        @param raw the raw interger you wanna warp into the interger message
        @return a interger message
            e.g. 10 -> :10\r\n
    */
std::string Parser::getIntergerMessage(int raw)
{
    std::string rc = ":";
    rc += std::to_string(raw);
    rc += "\r\n";
    return rc;
}

/*
        @param raw the raw string vactor you wanna warp into the RESP message
        @return a string vactor message
            e.g. Cloud Computing-> *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    */
std::string Parser::getRESPArry(std::vector<std::string> & raw)
{
    std::string rc = "*";
    int number = raw.size();

    rc += std::to_string(number);
    rc += "\r\n";
    
    for(auto & s : raw) 
    {
        rc += getStringMessage(s);
    }

    return rc;
}

/*
        @para message the raw message recieved
        @return the string parsered from message
            e.g. CS06142 <- $7\r\nCS06142\r\n
    */
std::string Parser::parseStringMessage(char * message, int * length)
{
    std::string rc = "";
    int strl = 0;
    int pos = 0;
    assert(message[0] == '$');

    strl = atoi(message + 1);
    pos = 3 + std::to_string(strl).size();
    rc = std::string(message + pos, strl);

    if(length) {
        *length = pos + strl + 2;
    }

    return rc;
}

/*
        @para message the raw message recieved
        @return the interger parsered from message
            e.g. 10 <- :10\r\n
    */
int Parser::parserIntergerMessage(char * message, int * length)
{
    int rc = 0;
    assert(message[0] == ':');
    rc = atoi(message + 1);

    if(length) {
        *length = 3 + std::to_string(rc).length();
    }
    return rc;
}

/*
        @para message the raw message recieved
        @return the status of the excution
            e.g. +OK\r\n -> true
                 -ERROR\r\n -> false
    */
bool Parser::parserSuccessMessage(char * message, int * length)
{
    assert(message[0] == '+' || message[0] == '-');
    if(message[0] == '+') {
        if(length) {
            *length = 5;
        }
        return true;
    }
    else {
        if(length) {
            *length = 8;
        }
        return false;
    }
}

/*
        @para message the raw message recieved
        @return the string vector parsered from message
            e.g. Cloud Computing <- *2\r\n$5\r\nCloud\r\n$9\r\nComputing\r\n
    */
std::vector<std::string> Parser::parserRESPArry(char * message, int * length)
{
    std::vector<std::string> rc;
    int number = 0;
    int pos = 0;

    assert(message[0] == '*');
    number = atoi(message + 1);
    pos += std::to_string(number).length() + 3;

    for(int i = 0; i < number; i++) {
        int len = 0;
        std::string tmp = parseStringMessage(message + pos, &len);
        rc.push_back(tmp);
        pos += len;
    }

    if(length) {
        *length = pos;
    }
    return rc;
}