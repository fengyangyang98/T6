#ifndef PARTICIPANT_HPP_
#define PARTICIPANT_HPP_

#include "core.hpp"
#include "logger.hpp"
#include "network.hpp"
#include "parser.hpp"
#include "kv.hpp"

class Participant
{
private:

    // the local txid
    txid                _TXID;

    // the state of the node
    P_State             _state;

    // network
    Network             _net;

    // the log
    Logger              _lg;

    // kv
    KV                  _kvdb;

    // helper
    Parser              _parser;

private:
    int keepAlive();
    int Working();

    std::string pWorker(std::string task);
    std::string eventParser(std::string event);

public:
    Participant();
    int Init();
    int Launch();
};

#endif