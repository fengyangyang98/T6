#ifndef CORE_HPP_
#define CORE_HPP_

// Linux
#include <unistd.h>
#include <linux/limits.h>
#include <sys/time.h>
#include <syscall.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/mman.h>
#include <pthread.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdarg>
#include <errno.h>
#include <malloc.h>
#include <algorithm>
#include <cstdlib>

// CPP
#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <atomic>
#include <list>
#include <queue>
#include <thread>
#include <assert.h>
#include <semaphore.h>

// error code list
#define KV_OK                                           0
#define KV_IO                                          -1
#define KV_INVALIDARG                                  -2
#define KV_TIMEOUT                                     -3
#define KV_QUIESCED                                    -4
#define KV_EDU_INVAL_STATUS                            -5
#define KV_NETWORK                                     -6
#define KV_NETWORK_CLOSE                               -7

#define KV_SET                                         -8
#define KV_GET                                         -9
#define KV_DEL                                         -10

typedef uint64_t txid;

typedef enum NodeState : uint8_t
{
    NODE_INVALID,
    NODE_WORKING,
    NODE_RECOVERY,
    NODE_WAITING
} PState;

typedef enum WorkState : uint8_t
{
    WORK_INVALID,
    WORK_PRE,
    WORK_COMMIT,
    WORK_DONE,
    WORK_ABORT
} WorkState;

#endif