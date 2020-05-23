#ifndef COORDINATOR_HPP_
#define COORDINATOR_HPP_

#include "core.hpp"
#include "logger.hpp"
#include "lock.hpp"
#include "network.hpp"
#include "parser.hpp"

#define MAX_THREAD_NUMBER   1024
#define TXID_START          3
#define RECOVERY_TXID       1
#define ASK_DATA_TXID       2

#define RET_SKIP            1


typedef enum CState : uint8_t
{
    C_INVALID,
    C_WORKING,
    C_RECOVERY
} CState;

typedef struct NodeInfo
{
    std::string add;
    unsigned int port;
} NodeInfo;

class Coordinator
{
private:

    // global trasation id
    txid                                _TXID;

    // the state of the coordinator
    CState                              _cstate;
    // the network of the coordinator
    Network                             _cnet;


    p_id                                _pnum;
    // the participant state table
    std::map<p_id, std::atomic<P_State>>_pstate;
    // the participant network;
    std::map<p_id, Network>             _pnet;
    // the participant info
    std::map<p_id, NodeInfo>            _pinfo;


    // the logger
    Logger                              _lg;
    // helper
    Parser                              _parser;


    // for the p working thread
    cpLock                              _ptaskSem[MAX_THREAD_NUMBER];
    cpLock                              _pRetSem[MAX_THREAD_NUMBER];
    // task buf
    std::map<p_id, std::string>         _ptask;
    // pworking  return
    std::map<p_id, int>                 _pworkingRet;
    // pworking data return
    std::map<p_id, std::string>         _pworkingDataRet;
    // the participant list
    std::map<p_id,std::thread>          _tworking;


    // for the recovery
    int                                 _tmpNum = 0;
    std::map<p_id, txid>                maxTxidTB;
    // mutex
    exclusiveLock                       _recoveryMutex;
    // task buf
    std::map<p_id, std::string>         _pRtask;
    // precovery  return
    std::map<p_id, int>                 _precoveryRet;
    // precovery data return
    std::map<p_id, std::string>         _preoveryDataRet;
    //lock
    cpLock                              _pRtaskSem[MAX_THREAD_NUMBER];
    cpLock                              _pRRetSem[MAX_THREAD_NUMBER];

    // the participant keepalive list
    std::thread                         _talive[MAX_THREAD_NUMBER];

private:

    void pWorking(p_id id);
    void keepAlive(p_id id);
    void pRecovery(p_id id);
    
    std::string UpdateDB(std::string resp);
    std::string Request(std::string resp);



    void recoveryFromC(txid min);
    void recoveryFromP(txid min);

    p_id getRecoveryLeader(txid & id);

public:

    int Init(std::vector<NodeInfo> ps, NodeInfo c);
    int Recovery();
    int Working();

    
};
                
#endif