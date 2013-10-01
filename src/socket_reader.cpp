#include <vector>
#include <chrono>
#include <functional>
#include <utility>
#include <memory>
#include <string>
#include <boost/signals2.hpp>
#include <stdexcept>

#include "socket_reader.h"
#include "socket.h"

namespace wifoe {
  
socket_reader::socket_reader( const clock::duration& timeout ) : timeout{timeout} {}

using boost::signals2::connection;
connection socket_reader::watch_socket( const sys::socket_handle sock, event_signal::slot_type slot ) {
  auto sig_pair = signals.find(sock);
  if( sig_pair == signals.end() ) {
    sig_pair = signals.insert( sig_pair, std::make_pair(sock, std::make_shared<event_signal>()) );
  }
  return sig_pair->second->connect(slot);
}

const sys::socket_list socket_reader::socket_list() const {
  sys::socket_list list;
  for( auto pair : signals ) list.push_back(pair.first);
  return list;
}

void socket_reader::loop() {
  terminate_loop = false;
  for(;;) {
    if( terminate_loop ) break;
    for( auto sock : sys::select_for_read(socket_list()) ) {
      auto& sig = *signals[sock];
      sig(sys::read_sock(sock));
    }
  }
}

void socket_reader::end_loop() {
  terminate_loop = true;
}

} // namespace wifoe
