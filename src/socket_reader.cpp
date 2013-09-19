#include <vector>
#include <chrono>
#include <functional>
#include <boost/signals2.hpp>

#include "socket_reader.h"
#include "socket.h"

namespace wifoe {

socket_reader::socket_reader( const clock::duration& timeout ) : timeout{timeout} {}

using boost::signals2::connection;
connection socket_reader::watch_socket( const sys::socket& sock, socket_reader::event_signal::slot_type slot ) {
  return signals[std::cref(sock)].connect(slot);
}

/*
sys::socket_list socket_reader::active_sockets() const {
  sys::socket_list list;
  for( auto pair : signals ) {
    if( pair.second.num_slots() > 0 ) list.push_back(pair.first);
  }
  return list;
}
*/

void socket_reader::loop() {
  /*
  for(;;) {
    auto read_socks = sys::select_for_read
  }
  */
}

} // namespace wifoe
