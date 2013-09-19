#include "nic.h"
#include <string>
#include <stdexcept>

extern "C" {
  #include <net/if.h>
  #include <errno.h>
  #include <string.h>
}

namespace sys {
  
unsigned int nic_index( const std::string& name ) {
  int idx = 0;
  if( idx = if_nametoindex(name.c_str()) == 0 ) {
    throw std::invalid_argument{std::string{strerror(errno)}};
  }
  return idx;
}

std::string nic_name( const unsigned int index ) {
  char name_buf[IF_NAMESIZE] = {0};
  if( if_indextoname(index, name_buf) == NULL ) {
    throw std::invalid_argument{std::string{strerror(errno)}};
  }
  return std::string{name_buf};
}

nic::operator unsigned int() const {
  return index;
}

nic::nic( const std::string& name ) : index{nic_index(name)} {}

nic::nic( const unsigned int index ) : index{index} {}

} // namespace sys
