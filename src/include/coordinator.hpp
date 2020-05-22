#ifndef COORDINATOR_HPP_
#define COORDINATOR_HPP_

#include "core.hpp"

typedef enum CState : uint8_t
{
    C_INVALID,
    C_WORKING,
    C_RECOVERY
} CState;

class Coordinator
{
private:

    // global trasation id
    txid _TXID;

    // the state of the coordinator
    CState _state;

    // the participant list


    // the client task pool


    // the logger


public:
    int Init();

    int keepAlive();

    int Recovery();

    int Working();
};
                
#endif