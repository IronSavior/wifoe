#ifndef _GUARD_CONFIG_H_
#define _GUARD_CONFIG_H_
#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <boost/program_options.hpp>

class Config {
  boost::program_options::options_description _desc;
  boost::program_options::variables_map       _vars;

  std::string     _src_addr;

public:
  bool            help;
  std::string     wifi_iface;
  std::string     dist_iface;
  
  std::vector<unsigned char>  src_addr;
  
  const boost::program_options::options_description desc();
  
  Config() = delete;
  Config( const int argc, const char* const argv[] );
};

#endif
