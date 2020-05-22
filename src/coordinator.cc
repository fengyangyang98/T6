#include "core.hpp"
#include "coordinator.hpp"
#include "parser.hpp"
#include "logger.hpp"

int Coordinator::Init(std::vector<NodeInfo> ps, NodeInfo c)
{
    int rc = KV_OK;

    // set the txid, conserve some for the recovery
    _TXID = TXID_START;

    // the init state is recovery
    _cstate = C_RECOVERY;

    // set the coordinator network for communication with clients
    _cnet = Network(c.add, c.port);

    // get p number
    _pnum = ps.size();

    for (p_id id = 1; id <= (p_id)_pnum; id++)
    {
        // init p's state is waiting
        _pstate[id] = P_WAITING;
        // set the p's network
        _pnet[id] = Network(ps[id - 1].add, ps[id - 1].port);
        // set the node info
        _pinfo[id] = ps[id - 1];
    }

    // set the thread
    for (p_id id = 1; id <= (p_id)ps.size(); id++)
    {
        _talive[id] = std::thread(&Coordinator::keepAlive, this, id);
    }

    return rc;
}

void Coordinator::pWorking(p_id id)
{
    assert(_pnet.count(id) == 1);

    while (1)
    {
        std::string rmsg = "";
        std::string task;
        int rc = KV_OK;

        _ptaskSem[id].cGet();
        _pRetSem[id].pGet();
        // the working
        if (_pstate[id] != P_WORKING)
        {
            _pworkingRet[id] = RET_SKIP;
            goto done;
        }

        task = _ptask[id];

        // send and recv from p
        rc = _pnet[id].sendAndRecv(task, rmsg);
        _pworkingRet[id] = rc;

        // any error occurs, close the socket and reconnet for recovering
        if (rc)
        {
            _pnet[id].close();
            _pstate[id] = P_WAITING;
        }

    done:
        _ptaskSem[id].cRelease();
        _pRetSem[id].pRelease();
    }
}

void Coordinator::keepAlive(p_id id)
{
    assert(_pnet.count(id) == 1);

    while (1)
    {

        if (_pstate[id] != P_WAITING)
        {
            continue;
        }

        // try to connect with the participant
        while (_pnet[id].reconnect())
        {
            sleep(1);
        }

        // change the state code
        _recoveryMutex.get();
        _pstate[id] = P_TEMP;
        _tmpNum++;
        _recoveryMutex.release();
    }
}


/*
    The process is like:
        0. get the task from the client
        1. try to give the request to the all p and recv the response
        2. try to give the task to the all p and recv the
        3. then
            1. if the done get from any p, txid will increase by one
            2. or this resquest fails
        4. give back response
*/
int Coordinator::Working()
{
    int rc = KV_OK;

    std::vector<std::string> task;
    while (_cnet.acceptWithoutCloseBind())
    {
    }
    _cnet.recvCommand(task);

    std::string resp = _parser.getRESPArry(task);
    std::string rc;

    if (task.size() < 1)
    {
        rc = _parser.getErrorMessage();
    }
    else if (task[0] == "SET" || task[0] == "DEL")
    {
        rc = UpdateDB(resp);
    }
    else if (task[0] == "GET")
    {
        rc = Request(resp);
    }
    else
    {
        rc = _parser.getErrorMessage();
    }

    _cnet.sendResult(rc)
        _cnet.close();

    return rc;
}

std::string Coordinator::UpdateDB(std::string resp)
{
    int rc = KV_OK;

    _lg.writeLog(_TXID, LOG_PRE, resp);

    // push the work into all the p
    std::string voteReq = Log(_TXID, LOG_PRE, "").logToStr();
    for (p_id id = 1; id <= _pnum; id++)
    {
        _ptaskSem[id].pGet();
        _ptask[id] = voteReq;
        _ptaskSem[id].pRelease();
    }

    bool vote = false;
    // get and check the result
    for (p_id id = 1; id <= _pnum; id++)
    {
        _pRetSem[id].cGet();
        // check the state
        if (_pworkingRet[id] == KV_OK &&
            _pworkingDataRet[id] == "")
        {
            vote = true;
        }
        _pRetSem[id].cRelease();
    }

    // give the commit
    bool commit = false;
    if (vote == true)
    {
        std::string commitReq = Log(_TXID, LOG_COMMIT, resp).logToStr();
        for (p_id id = 1; id <= _pnum; id++)
        {
            _ptaskSem[id].pGet();
            _ptask[id] = commitReq;
            _ptaskSem[id].pRelease();
        }

        std::string voteReq = Log(_TXID, LOG_PRE, "").logToStr();
        for (p_id id = 1; id <= _pnum; id++)
        {
            _pRetSem[id].cGet();
            // check the state
            if (_pworkingRet[id] == KV_OK &&
                _pworkingDataRet[id] == "done")
            {
                commit = true;
            }
            _pRetSem[id].cRelease();
        }
    }
    else
    {
        return _parser.getErrorMessage();
    }

    if (commit == true)
    {
        // push the log into the logger
        _lg.chageLogState(_TXID, LOG_COMMIT);
        return _parser.getSuccessMessage();
    }
    else
    {
        return _parser.getErrorMessage();
    }
}

std::string Coordinator::Request(std::string resp)
{
    // push the work into all the p
    for (p_id id = 1; id <= _pnum; id++)
    {
        _ptaskSem[id].pGet();
        _ptask[id] = resp;
        _ptaskSem[id].pRelease();

        _pRetSem[id].cGet();
        if (_pworkingRet[id] == KV_OK &&
            _pworkingDataRet[id] != "")
        {
            std::string rc = _pworkingDataRet[id];
            _pRetSem[id].cRelease();
            return rc;
        }
        _pRetSem[id].cRelease();
    }

    return _parser.getErrorMessage();
}


/*
    The process is like that:
        1. give a txid request to the all p to build a txid table
        2. cal the recovery task of each p
            -> 1. ask log 
            <- 2. get log
            -> 3. push log into the p
            <- 4. get done flag
*/
int Coordinator::Recovery()
{
    int rc = KV_OK;

    // waiting for more temp p
    _recoveryMutex.get();
    if(_tmpNum != 0) goto done;
    
    std::map<p_id, txid> maxTxidTB;
    std::map<txid, p_id> task;

    std::string txidReq = Log(RECOVERY_TXID, LOG_PRE, "LOCAL_TXID").logToStr();
    for (p_id id = 1; id <= _pnum; id++)
    {
        _ptaskSem[id].pGet();
        _ptask[id] = txidReq;
        _ptaskSem[id].pRelease();
    }

    // get max txid of each p
    for (p_id id = 1; id <= _pnum; id++)
    {
        _pRetSem[id].cGet();
        // check the state
        if (_pworkingRet[id] == KV_OK &&
            _pworkingDataRet[id] != "")
        {
            maxTxidTB[id] = strtol(_pworkingDataRet[id].c_str());
        }
        _pRetSem[id].cRelease();
    }


    
done:
    _recoveryMutex.release();
    return rc;
}