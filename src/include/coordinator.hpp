#ifndef COORDINATOR_HPP_
#define COORDINATOR_HPP_

#include "core.hpp"

class Coordinator
{
private:

    // global trasation id
    txid TXID;

    // the participant list

    // the client task pool


public:
    int Init();

    int keepAlive();

    int Recovery();

    int Working();
};

#endif