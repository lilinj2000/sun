// Copyright (c) 2010
// All rights reserved.

#ifndef SUN_OPTIONS_HH
#define SUN_OPTIONS_HH

#include <string>
#include "soil/json.hh"

namespace sun {

class Options {
 public:
  explicit Options(
      const rapidjson::Document& doc);

  std::string instru;
  double price;
  int volume;
  int interval;
  int count;

  bool only_t1;

  std::string data_file;
};

};  // namespace sun

#endif
