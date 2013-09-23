#include "socket.h"
#include "nic.h"
#include "socket_reader.h"

#include <csignal>
#include <iostream>

static wifoe::socket_reader* g_sock_reader = nullptr;

void sig_handler( int sig ) {
  if( g_sock_reader != nullptr ) {
    g_sock_reader->end_loop();
  }
}

int main( int argc, char** argv ) {
  sys::nic mon_iface{"mon.wlan0"};
  sys::socket mon_sock{sys::socket_type::RAW, sys::socket_proto::ALL};
  mon_sock.bind(mon_iface);
  
  wifoe::socket_reader sock_reader{std::chrono::milliseconds(10)};
  std::cout << "Socket reader created" << std::endl;

  sys::nic dist_iface{"tap0"};
  sys::socket dist_sock{sys::socket_type::DGRAM, sys::socket_proto::EXPERIMENTAL1};
  dist_sock.bind(dist_iface);
  
  g_sock_reader = &sock_reader;
  std::signal(SIGINT, sig_handler);
  
  sock_reader.watch_socket(mon_sock,
    [&dist_sock, &dist_iface]( const std::vector<char> packet ) {
      if( packet.size() > 1500 ) {
        std::cout << "Monitor read event: " << packet.size() << std::endl;
      }
      sys::send_broadcast(dist_sock, dist_iface, packet);
    }
  );
  std::cout << "Monitor Socket watch added" << std::endl;

  sock_reader.watch_socket(dist_sock,
    [&mon_sock]( const std::vector<char> packet ) {
      if( packet.size() > 1500 ) {
        std::cout << "Distribution read event: " << packet.size() << std::endl;
      }
      sys::send_raw(mon_sock, packet);
    }
  );
  std::cout << "Distribution Socket watch added" << std::endl;

  sock_reader.loop();
  
  std::cout << std::endl << "Loop Interrupted." << std::endl;
  return 0;
}
