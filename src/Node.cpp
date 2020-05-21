#include "Node.hpp"

std::mutex m;
std::condition_variable empty;

const int BUF_SIZE = 512000;
bool flag = 0; // 阻塞
volatile unsigned long long global_e_id = 0;

namespace kv_2pc_sys {
// Start here
void Coor::work_main() {
  std::thread client_listener(server,nullptr);
  std::thread PC_communicate(FSM,nullptr);
  client_listener.detach();
  PC_communicate.detach();
  return ;
}

// Server
void Coor::get_command(const int client_sockfd,
                       const struct sockaddr_in &client_addr) {
  char ipbuf[128];
  char buf[BUF_SIZE];
  memset(buf, 0, BUF_SIZE);

  while (1) {
    // read data, 阻塞读取
    int len = recv(client_sockfd, buf, sizeof(buf), flag);
    if (len == -1) {
      close(client_sockfd);
      // close(server_sockfd);
      perror("read error");
    } else if (len == 0) { // 这里以len为0表示当前处理请求的客户端断开连接
      break;
    }
    Event tmp_e;
    {
      std::lock_guard<std::mutex> lockGuard(m);
      global_e_id++;
      tmp_e.e_id = global_e_id;
      tmp_e.command = std::string(buf);
      tmp_e.addr = client_addr;
      command_list.push(tmp_e);
    }
    memset(buf, '\0', len); // 清空buf
  }
  close(client_sockfd);
  printf("Disconnect client iP: %s, port: %d\n",
         inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, ipbuf, sizeof(ipbuf)),
         ntohs(client_addr.sin_port));
  return;
}
void Coor::server() {
  // Prepare socket
  int server_sockfd = socket(PF_INET, SOCK_STREAM, 0);
  if (server_sockfd == -1) {
    close(server_sockfd);
    perror("socket error!");
  }
  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons((uint16_t)coordinator[0].port);
  inet_pton(AF_INET, coordinator[0].ip.c_str(), &server_addr.sin_addr.s_addr);

  if (bind(server_sockfd, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    close(server_sockfd);
    perror("bind error");
  }

  if (listen(server_sockfd, 1024) == -1) {
    close(server_sockfd);
    perror("listen error");
  }

  printf("Listen on port %d\n", coordinator[0].port);

  while (1) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_sockfd =
        accept(server_sockfd, (struct sockaddr *)&client_addr, &client_len);

    std::thread tcpThread(get_command, client_sockfd, client_addr);
    tcpThread.detach(); // 不阻塞主线程，执行tcp通讯的线程从线程对象分离
  }
  close(server_sockfd);
  return;
}
// FSM
void Coor::FSM() {
  std::thread pc_communit[participant.size()];
  while (true) {
    switch (curr_state) {
    case FSM_state::init:
      init();
      break;
    case FSM_state::wait_ready:
      wait();
      break;
    case FSM_state::abort:
      abort();
      break;
    case FSM_state::commit:
      commit();
      break;
    default:
      break;
    }
  }
}
void Coor::init() {
  {
    std::lock_guard<std::mutex> lockGuard(m);
    curr_event = command_list.front();
    command_list.pop();
  }

  return;
}
void Coor::wait() { return; }
void Coor::abort() { return; }
void Coor::commit() { return; }
void Coor::pc() { return; }
} // namespace kv_2pc_sys

namespace kv_2pc_sys {
void Part::exe() { return; }

void Part::init() { return; }
void Part::wait() { return; }
void Part::abort() { return; }
void Part::commit() { return; }
void Part::pc() { return; }
} // namespace kv_2pc_sys