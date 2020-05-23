#include "core.hpp"
#include "participant.hpp"

int Participant::keepAlive()
{
    _net.close();
    while (1)
    {
        // try to connect with the coordinator
        while (_net.reaccept())
        {
            sleep(1);
        }
    }

    return KV_OK;
}

int Participant::Working()
{
    int rc = KV_OK;
    std::string content;
    std::string rmsg;
    std::vector<std::string> comds;

    // if network close will return error
    rc = _net.recv(content);
    if(rc) goto done;

    // parse and send
    rmsg = pWorker(content);
    _net.send(rmsg);

done:
    return rc;
}

std::string Participant::pWorker(std::string task)
{
    std::string rc = "";
    txid tid;
    // parser the task
    Log l(task);

    switch(l.ID)
    {
        case SUPER_TXID:
            // reserve
            break;
        case RECOVERY_TXID:
            // recovery request, the p should send the max txid
            return std::to_string(_TXID - 1);
            break;
        case ASK_DATA_TXID:
            tid = strtol(l.event.c_str(), nullptr, 10);
            if(tid >= _TXID) {
                return "";
            } else {
                return _lg.getLogByTXID(tid).logToStr();
            }
            break;
        default:
            if(l.ID == _TXID) {
                return Log(l.ID, LOG_ABORT, "").logToStr();
            } else {
                if(l.state == LOG_COMMIT) {
                    _lg.writeLog(l);
                    _TXID++;
                }
                return Log(l.ID, LOG_COMMIT, eventParser(l.event)).logToStr();
            }
            break;
    }


    return rc;
}

std::string Participant::eventParser(std::string event)
{
    std::string rstr = "";
    int rc = KV_OK;
    int delnumber = 0;
    
    std::vector<std::string> comds;

    // if network close will return error
    rc = _net.recv(event);
    if(rc) {
        rstr = _parser.getErrorMessage();
        goto done;
    }

    // parse and send
    _parser.parserRESPArry((char *)event.c_str(), comds);

    if(comds.size() < 1) {
        rstr = _parser.getErrorMessage();
        goto done;
    }

    switch(comds[0][0]) {
        case 'D': // DEL CS06142 CS162
            comds.erase(comds.begin());
            delnumber = _kvdb.kvDELArry(comds);
            rstr = _parser.getIntergerMessage(delnumber);
            break;
            
        case 'S': // SET key value
            if(comds.size() != 3) {
                rstr = _parser.getErrorMessage();
            } else {
                _kvdb.kvSET(comds[1], comds[2]);
                rstr = _parser.getSuccessMessage();
            }
            break;
        case 'G': // GET key
            if(comds.size() != 2) {
                rstr = _parser.getErrorMessage();
            } else {
                rstr = _parser.getRESPArry( _kvdb.kvGET(comds[1]) );
            }
            break;
        case 'P': // P
            rstr = _parser.getRESPArry("PRE");
            break;
        default:
            rstr = _parser.getErrorMessage();
            break;

    }

done:
    return rstr;
}

void Participant::Init(NodeInfo info)
{
    _TXID   = TXID_START;
    _net    = Network(info.add, info.port);
}

int Participant::Launch()
{
    while(1) {
        if(Working()) {
            keepAlive();
        }
    }
    return KV_OK;
}