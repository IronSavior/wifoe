#include "socket.h"
#include "nic.h"
#include "socket_reader.h"
#include "config.h"

#include <csignal>
#include <iostream>
#include <iomanip>
#include <vector>

extern "C" {
  #include "radiotap/radiotap_iter.h"
}

std::string mac_to_str( const std::vector<unsigned char> mac ) {
  std::stringstream ss;
  for( auto oct = mac.begin(); oct != mac.end(); ) {
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(*oct);
    ++oct;
    if( oct != mac.end() ) ss << ":";
  }
  return ss.str();
}

bool check_source_address( const std::vector<unsigned char> packet, const std::vector<unsigned char> target_addr ) {
  ieee80211_radiotap_iterator iter;

  if( ieee80211_radiotap_iterator_init(&iter, (ieee80211_radiotap_header*)packet.data(), packet.size(), NULL) ) {
    std::cerr << "radiotap init error -- maybe throw?" << std::endl;
    return false;
  }
  
  auto tx_addr_start = packet.begin() + iter._max_length + 10; // 2 bytes FC, 2 bytes duration, 6 bytes addr1
  std::vector<unsigned char> tx_addr(tx_addr_start, tx_addr_start + 6);
  return target_addr == tx_addr;
}

static wifoe::socket_reader* g_sock_reader = nullptr;
void sig_handler( int sig ) {
  if( g_sock_reader != nullptr ) {
    g_sock_reader->end_loop();
  }
}

int main( int argc, char** argv ) {
  Config conf(argc, argv);
  
  if( conf.help ) {
    std::cout << conf.desc() << std::endl;
    return 0;
  }
  
  sys::nic mon_iface{conf.wifi_iface};
  sys::socket mon_sock{sys::socket_type::RAW, sys::socket_proto::ALL};
  mon_sock.bind(mon_iface);
  
  wifoe::socket_reader sock_reader{std::chrono::milliseconds(10)};
  std::cout << "Socket reader created" << std::endl;

  sys::nic dist_iface{conf.dist_iface};
  sys::socket dist_sock{sys::socket_type::DGRAM, sys::socket_proto::EXPERIMENTAL1};
  dist_sock.bind(dist_iface);
  
  g_sock_reader = &sock_reader;
  std::signal(SIGINT, sig_handler);
  
  sock_reader.watch_socket(mon_sock,
    [&dist_sock, &dist_iface, &conf]( const std::vector<unsigned char> packet ) {
      if( check_source_address(packet, conf.src_addr) ) {
        std::cout << "Monitor read event: " << packet.size() << std::endl;
        sys::send_broadcast(dist_sock, dist_iface, packet);
      }
    }
  );
  std::cout << "Monitor Socket watch added" << std::endl;

  sock_reader.watch_socket(dist_sock,
    [&mon_sock]( const std::vector<unsigned char> packet ) {
      std::cout << "Distribution read event: " << packet.size() << std::endl;
      sys::send_raw(mon_sock, packet);
    }
  );
  std::cout << "Distribution Socket watch added" << std::endl;

  sock_reader.loop();
  
  std::cout << std::endl << "Loop Interrupted." << std::endl;
  return 0;
}
