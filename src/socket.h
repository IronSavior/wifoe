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
  
  class socket {
    int fd;
  public:
    using  ref = std::reference_wrapper<socket>;
    using cref = std::reference_wrapper<const socket>;
    
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

  using socket_list = std::vector<socket::cref>;

  void send_broadcast( const socket& sock, const nic& iface, const std::vector<char>& msg );
  void send_raw( const socket& sock, const std::vector<char>& msg );
  
  socket_list select_for_read(
    const socket_list& sockets,
    const clock::duration& timeout = clock::duration::zero()
  );
  
} // namespace sys

#endif