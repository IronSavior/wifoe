#ifndef _GUARD_SOCKET_H_
#define _GUARD_SOCKET_H_
#pragma once

#include <vector>
#include <chrono>
#include <functional>

namespace sys {
  using clock = std::chrono::system_clock;
  
  class nic;
  
  enum struct socket_type {
    DGRAM,
    RAW
  };
  
  enum struct socket_proto {
    ALL,
    EXPERIMENTAL1,
    EXPERIMENTAL2
  };
  
  using socket_handle = int;
  
  class socket {
    socket_handle fd;
  public:
    socket( const socket& src ) = delete;
    socket& operator=( const socket& src ) = delete;
    
    socket( socket_type type, socket_proto protocol );
    
    socket( socket&& src ) = default;
    socket& operator=( socket&& src ) = default;
    
    ~socket();
    
    bool operator<( const socket& rhs ) const;
    operator int() const;
    
    void bind( const nic& iface ) const;
  };

  using socket_list = std::vector<socket_handle>;

  void send_broadcast( const socket_handle& sock, const nic& iface, const std::vector<char>& msg );
  void send_raw( const socket_handle& sock, const std::vector<char>& msg );
  
  socket_list select_for_read(
    const socket_list& sockets,
    const clock::duration& timeout = clock::duration::zero()
  );
  
  std::vector<char> read_sock( const socket_handle& );
  
  void set_send_buffer_size( const socket_handle& sock, const unsigned int bufsize );
  
} // namespace sys

#endif
