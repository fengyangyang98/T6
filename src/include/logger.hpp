#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include "core.hpp"

typedef enum logState : uint8_t
{
    INVALID,
    LOG_PRE,
    LOG_COMMIT,
    LOG_ABORT
} logState;

/*
    The logger is designed for consistance ensurance. The log will be
    stored in the memory, which means the log will not be persisted. 
    Therefore, no ensurance for the data persistance.

    The logger will be used when the system in the recovery mode. The 
    coordinator and participants will communicate through the coordinator,
    and they will choose the latest trasaction ID, and last, the system 
    will be recoveried until the newest trasaction.
*/

/*
        The log's format is like this:
        [ ID    ]
        [ STATE ]
        [ EVENT ]
*/

typedef struct Log
{
    txid ID;
    logState state;
    std::string event;

    Log(txid id, logState state, std::string event) :
        ID(id), state(state), event(event) { }
    
    void changeState(logState newState)
    {
        state = newState;
    }

    void strToLog();
    std::string logToStr();
    
} Log;


class Logger
{
private:
    std::map<txid, Log> _l;
public:
    int writeLog(txid ID, logState state, std::string event);
    int chageLogState(txid ID, logState newState);
};


#endif