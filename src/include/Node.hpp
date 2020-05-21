#ifndef NODE_HPP
#define NODE_HPP

#include "include/kv.hpp"
#include "include/parser.hpp"
#include "queue"
#include "read_conf.hpp"
#include "thread"

#include <arpa/inet.h>
#include <cstring>
#include <ctype.h>
#include <mutex>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <condition_variable>

using std::queue;

namespace kv_2pc_sys {
struct Cp_lock {
  std::mutex m;
  std::condition_variable empty;
  int size;
};

enum FSM_state { init = 0, wait_ready, abort, commit };
struct Event {
  std::string command;
  std::string reply;
  unsigned long long e_id;
  struct sockaddr_in addr;
};

class Node {
protected:
  queue<Event> command_list;
  KV kvdb;
  FSM_state curr_state;
  Event curr_event;

public:
  Node() {}
  ~Node() {}

  virtual void get_command(const int client_sockfd,
                           const struct sockaddr_in &client_addr) {}
  virtual void exe() {}

  virtual void server(){}
  virtual void work_main(){};
  virtual void FSM() = 0;
  virtual void pc() = 0;
  virtual void init() = 0;
  virtual void wait() = 0;
  virtual void abort() = 0;
  virtual void commit() = 0;
};

class Coor : public Node {
private:
public:
  Coor() {}
  ~Coor() {}

  virtual void get_command(const int client_sockfd,
                           const struct sockaddr_in &client_addr);
  virtual void server();
  virtual void work_main();
  virtual void FSM();
  virtual void init();
  virtual void wait();
  virtual void abort();
  virtual void commit();
  virtual void pc();
};

class Part : public Node {
private:
public:
  Part() {}
  ~Part() {}

  virtual void exe();

  virtual void FSM();
  virtual void init();
  virtual void wait();
  virtual void abort();
  virtual void commit();
  virtual void pc();
};

}; // namespace kv_2pc_sys

#endif // NODE_HPP