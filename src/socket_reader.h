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
    using event_signature = void (const std::vector<unsigned char> packet );
    using event_signal = boost::signals2::signal<event_signature>;
    using socket_signal_map = std::map<sys::socket_handle, std::shared_ptr<event_signal>>;
    
    clock::duration timeout;
    socket_signal_map signals;
    bool terminate_loop;
    
    const sys::socket_list socket_list() const;
  public:
    boost::signals2::connection watch_socket( const sys::socket_handle sock, const event_signal::slot_type slot );
    
    socket_reader() = delete;
    socket_reader( const clock::duration& timeout );
    
    void loop();
    void end_loop();
  };
  
} // namespace wifoe

#endif
