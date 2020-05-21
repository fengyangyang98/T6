#include "read_conf.hpp"
#include "Node.hpp"

using std::cout;

int main(int argc, char **argvs) {
  kv_2pc_sys::GetOptLong(argc, argvs);
  if (!kv_2pc_sys::read_conf()) {
    std::cout << "ERR! conf file read.\n";
    return 0;
  }
  // kv_2pc_sys::this_debug::show_conf();
  // 2PC
  
  return 0;
}