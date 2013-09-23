#include "nic.h"
#include "socket.h"

#include <vector>
#include <array>
#include <algorithm>
#include <stdexcept>
#include <cstdint>

extern "C" {
  #include <arpa/inet.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netpacket/packet.h>
  #include <net/ethernet.h>
  #include <errno.h>
  #include <string.h>
  #include <unistd.h>
  #include <sys/select.h>
}

namespace sys {

int lookup_socket_type( const socket_type& t ) {
  switch(t) {
  case socket_type::DGRAM:
    return SOCK_DGRAM;
  case socket_type::RAW:
    break;
  }
  return SOCK_RAW;
}

int lookup_socket_proto( const socket_proto& p ) {
  switch(p) {
  case socket_proto::EXPERIMENTAL1:
    return 0x88b5;
  case socket_proto::EXPERIMENTAL2:
    return 0x88b6;
  case socket_proto::ALL:
    break;
  }
  return ETH_P_ALL;
}

socket::socket( socket_type type, socket_proto protocol )
  : fd{::socket(
      PF_PACKET,
      lookup_socket_type(type),
      htons(lookup_socket_proto(socket_proto::EXPERIMENTAL1))
    )} {
  if( fd < 0 ) {
    throw std::invalid_argument{std::string{strerror(errno)}};
  }
}

socket::~socket() {
  close(fd);
}

socket::operator int() const {
  return fd;
}

void socket::bind( const nic& iface ) const {
  sockaddr_ll ll = {0};
  ll.sll_family = AF_PACKET;
  ll.sll_ifindex = iface;
  ll.sll_protocol = htons(ETH_P_ALL);
  if( ::bind(fd, (sockaddr*)&ll, sizeof(ll)) < 0 ) {
    throw std::invalid_argument{std::string{strerror(errno)}};
  }
}

void send_broadcast( const socket_handle& sock, const nic& iface, const std::vector<char>& msg ) {
  std::array<uint8_t, ETH_ALEN> bca{{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};
  
  sockaddr_ll da = {0};
  da.sll_family = AF_PACKET;
  da.sll_ifindex = iface;
  da.sll_halen = sizeof(bca);
  da.sll_protocol = htons(lookup_socket_proto(socket_proto::EXPERIMENTAL1));
  std::copy(bca.begin(), bca.end(), da.sll_addr);
  
  if( sendto(sock, msg.data(), msg.size(), 0, (sockaddr*)&da, sizeof(da)) < 0 ) {
    throw std::invalid_argument{std::string{strerror(errno)}};
  }
}

void send_raw( const socket_handle& sock, const std::vector<char>& msg ){
  if( send(sock, msg.data(), msg.size(), 0) < 0 ) {
    throw std::invalid_argument{std::string{strerror(errno)}};
  }
}

socket_list select_for_read( const socket_list& sockets, const clock::duration& timeout ) {
  using std::chrono::seconds;
  using std::chrono::microseconds;
  
  socket_list socks;
  int max = 0;
  int res;
  timeval tv;
  fd_set fds;
  
  tv.tv_sec = timeout / seconds(1);
  tv.tv_usec = (timeout % seconds(1)) / microseconds(1);
  
  FD_ZERO(&fds);
  for( auto sock : sockets ) {
    max = (sock > max)? sock : max;
    FD_SET(sock, &fds);
  }
  
  res = select(max+1, &fds, NULL, NULL, &tv );
  if( res < 0 ) {
    throw std::invalid_argument{std::string{strerror(errno)}};
  }
  
  for( auto sock : sockets ) {
    if( FD_ISSET(sock, &fds) ) socks.push_back(sock);
  }
  
  return socks;
}

std::vector<char> read_sock( const socket_handle& sock ) {
  char buf[3000] = {0};
  int len = ::recv(sock, buf, sizeof(buf), 0);
  if( len < 0 ) {
    throw std::invalid_argument{std::string{strerror(errno)}};
  }
  return {buf, buf + len};
}

} // namespace sys
