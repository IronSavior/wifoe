#ifndef _GUARD_SOCKET_READER_H_
#define _GUARD_SOCKET_READER_H_
#pragma once

#include <vector>
#include <chrono>
#include <functional>
#include <boost/signals2.hpp>
#include "socket.h"

namespace wifoe {

  class socket_reader {
    using clock = std::chrono::high_resolution_clock;
    using event_signature = void (const std::vector<char>& packet );
    using event_signal  = boost::signals2::signal<event_signature>;
    using socket_signal_map = std::map<const sys::socket::cref, event_signal>;
    
    clock::duration timeout;
    socket_signal_map signals;
  public:
    boost::signals2::connection watch_socket( const sys::socket& sock, const event_signal::slot_type slot );
    
    socket_reader() = delete;
    socket_reader( const clock::duration& timeout );
    
    void loop();
  };
  
} // namespace wifoe

#endif
