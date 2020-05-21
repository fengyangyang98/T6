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

// error

#define KV_OK   0
#define KV_SET -1
#define KV_GET -2
#define KV_DEL -3

#endif