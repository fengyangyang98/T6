#include "read_conf.hpp"
#include <getopt.h>

// var
namespace kv_2pc_sys {
char *conf_url = nullptr;
Mode mode = Mode::ERR;
std::vector<Node_conf> participant;
std::vector<Node_conf> coordinator;
} // namespace kv_2pc_sys

// func
string key_word_list[] = {
    // mode set conf
    "mode", "coordinator", "participant",
    // ip/port set conf
    "coordinator_info", "participant_info"};

bool kv_2pc_sys::analysis_conf(string buf) {
  int loc = 0;
  string delim = " ", tmp = buf + " ";
  std::vector<string> table;
  while ((loc = tmp.find(delim)) != string::npos) {
    if (loc != 0) {
      table.push_back(tmp.substr(0, loc));
    }
    tmp = tmp.substr(loc + delim.size());
  }

  if (table[0] == "mode") {
    if (table[1] == "coordinator") {
      mode = COORDINATOR;
    } else if (table[1] == "participant") {
      mode = PARTICIPANT;
    }
  } else if (table[0] == "participant_info") {
    Node_conf p;
    string tmp = table[1] + ":";
    string delim = ":";
    std::vector<string> tmp_table;
    while ((loc = tmp.find(delim)) != string::npos) {
      if (loc != 0) {
        tmp_table.push_back(tmp.substr(0, loc));
      }
      tmp = tmp.substr(loc + delim.size());
    }
    p.ip = tmp_table[0];
    p.port = atoi(tmp_table[1].c_str());
    participant.push_back(p);
  } else if (table[0] == "coordinator_info") {
    Node_conf c;
    string tmp = table[1] + ":";
    string delim = ":";
    std::vector<string> tmp_table;
    while ((loc = tmp.find(delim)) != string::npos) {
      if (loc != 0) {
        tmp_table.push_back(tmp.substr(0, loc));
      }
      tmp = tmp.substr(loc + delim.size());
    }
    c.ip = tmp_table[0];
    c.port = atoi(tmp_table[1].c_str());
    coordinator.push_back(c);
  }
  return true;
}

bool kv_2pc_sys::read_conf() {
  std::fstream conf_file;
  conf_file.open(conf_url);
  if (!conf_file.is_open()) {
    return false;
  }
  participant.reserve(3);
  coordinator.reserve(1);

  string buf;
  // read
  while (!conf_file.eof()) {
    getline(conf_file, buf);
    // Annotate
    if ('!' == buf[0]) {
      continue;
    }
    analysis_conf(buf);
  }

  return true;
}

void kv_2pc_sys::GetOptLong(int argc, char *argv[]) {
  int opt, digit_optind = 0, option_index = 0;
  const char *optstring = ":c:";

  static struct option long_options[] = {
      {"config_path", required_argument, NULL, 'c'}, {0, 0, 0, 0}};

  while (true) {
    if (-1 == (opt = getopt_long(argc, argv, optstring, long_options,
                                 &option_index))) {
      break;
    }
    if ('c' == opt) {
      conf_url = new char[256];
      strcpy(conf_url, argv[optind - 1]);
    }
  }
  return;
}

void kv_2pc_sys::this_debug::show_conf() {
  using std::cout;
  using std::endl;
  cout << endl << "------show conf------" << endl;
  cout << "coordinator list:\n";
  for (auto i = coordinator.begin() ; i < coordinator.end(); i++) {
    cout << i->ip << " ";
    cout << i->port << std::endl;
  }
  cout << "\t----\n" << "participant list :\n";
  for (auto i = participant.begin() ; i < participant.end(); i++) {
    cout << i->ip << " ";
    cout << i->port << std::endl;
  }
  cout << "--------------------" << endl;
}