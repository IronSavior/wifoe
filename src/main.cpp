#include "socket.h"
#include "nic.h"

#include <csignal>
#include <array>

extern "C" {
  #include <sys/select.h>
  #include <sys/types.h>
  #include <sys/socket.h>
}

static int terminate_loop;

static void sock_read( int sock ) {
  int len;
  unsigned char buf[3000];
  
  len = recv(sock, buf, sizeof(buf), 0);
  if( len < 0 ) {
    perror("recv");
    return;
  }
  printf("Packet! %4d\n", len);
}

void sig_handler( int sig ) {
  terminate_loop = 1;
}

int main( int argc, char** argv ) {
  sys::nic mon_iface{"mon.wlan0"};
  sys::socket mon_sock{sys::socket_type::RAW, sys::socket_proto::ALL};
  mon_sock.bind(mon_iface);
  
  terminate_loop = 0;
  std::signal(SIGINT, sig_handler);
  /*
  for(;;) {
    if( terminate_loop ) break;
    if( read_ok(mon_sock) ) sock_read(mon_sock);
  }
  */
  return 0;
}
