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
    Participant() {};
    void Init(NodeInfo info);
    int Launch();
};

#endif