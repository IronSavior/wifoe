#include <chrono>
#include <string>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>

#include "config.h"

namespace opt = boost::program_options;

const opt::options_description Config::desc() {
  return _desc;
}

Config::Config( const int argc, const char* const argv[] ) : _desc("Options") {
  _desc.add_options()
    ("help",
      opt::bool_switch(&help),
      "display help")
    
    ("wifi-dev,w",
      opt::value<std::string>(&wifi_iface)->default_value(std::string()),
      "Wifi monitor interface")

    ("dist-dev,d",
      opt::value<std::string>(&dist_iface)->default_value(std::string()),
      "Wifi monitor interface")

    ("src-addr,s",
      opt::value<std::string>(&_src_addr)->default_value(std::string()),
      "Source address for which to distribute frames.")
  ;
  opt::store(opt::parse_command_line(argc, argv, _desc), _vars);
  opt::notify(_vars);
  
  if( help ) return;
  
  if( wifi_iface.empty() ) {
    throw opt::invalid_option_value("You must specify the wifi interface.");
  }

  if( dist_iface.empty() ) {
    throw opt::invalid_option_value("You must specify the distribution interface.");
  }

  if( _src_addr.empty() ) {
    throw opt::invalid_option_value("You must specify a source MAC address.");
  }
  else {
    boost::regex rx("[0-9a-fA-F]{2}");
    using rx_iter = boost::sregex_iterator;
    for( rx_iter m(_src_addr.begin(), _src_addr.end(), rx); m != rx_iter(); ++m ) {
      src_addr.push_back(static_cast<uint8_t>(std::stoul(m->str(), nullptr, 16)));
    }
    if( src_addr.size() != 6 ) throw opt::invalid_option_value("Invalid source MAC address.");
  }
}
