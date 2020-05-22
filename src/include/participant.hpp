#ifndef PARTICIPANT_HPP_
#define PARTICIPANT_HPP_

#include "core.hpp"

class Participant
{
private:

    // the local txid
    txid _TXID;

    // the state of the node
    NodeState _state;

    // the log

public:
    Participant();

    int Init();

    int keepAlive();

    int Recovery();

    int Working();
};

#endif