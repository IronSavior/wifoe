#ifndef _GUARD_NIC_H_
#define _GUARD_NIC_H_
#pragma once

#include <string>

namespace sys {
  
  class nic {
    unsigned int index;
  public:
    operator unsigned int() const;
    
    nic( const std::string& name );
    nic( const unsigned int index );
    
    nic() = delete;
    nic( const nic& other ) = default;
    nic& operator=( const nic& rhs ) = default;
    nic( nic&& other ) = default;
    nic& operator=( nic&& rhs ) = default;
  };
  
  unsigned int nic_index( const std::string& name );
  std::string nic_name( const unsigned int index );

} // namespace sys

#endif
