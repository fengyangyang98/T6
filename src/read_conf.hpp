#ifndef READ_CONF_HPP
#define READ_CONF_HPP

#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// namespace

using std::string;

namespace kv_2pc_sys {
// base type
enum Mode { ERR = -1, PARTICIPANT = 0, COORDINATOR = 1 };
struct Node_conf {
  string ip;
  unsigned int port;
};

// var
extern char *conf_url;
extern std::vector<Node_conf> participant;
extern std::vector<Node_conf> coordinator;
extern Mode mode;

// func
bool read_conf();
void GetOptLong(int argc, char *argv[]);
bool analysis_conf(std::string buf);

// debug
namespace this_debug {
void show_conf();
}
}; // namespace kv_2pc_sys

#endif // READ_CONF_HPP