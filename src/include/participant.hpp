#ifndef PARTICIPANT_HPP_
#define PARTICIPANT_HPP_

#include "core.hpp"

class Participant
{
private:

    // the local txid
    txid TXID;

    // the log

public:

    int Init();

    int keepAlive();

    int Recovery();

    int Working();
};

#endif