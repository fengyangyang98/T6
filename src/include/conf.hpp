#ifndef CONF_HPP_
#define CONF_HPP_

#include "core.hpp"

std::string GetOptLong(int argc, char *argv[]);
bool readConf(std::string conf, 
              std::vector<NodeInfo> & p,
              NodeInfo & c,
              Mode & m);

void showInfo(std::vector<NodeInfo>  p,
              NodeInfo  c,
              Mode  m);
#endif 