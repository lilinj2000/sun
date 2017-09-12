// Copyright (c) 2010
// All rights reserved.

#include <memory>
#include "Server.hh"
#include "soil/json.hh"
#include "soil/Log.hh"

int main(int argc, char* argv[]) {
  std::unique_ptr<sun::Server> server;

  rapidjson::Document doc;
  soil::json::load_from_file(&doc, "sun.json");
  soil::log::init(doc);

  server.reset(new sun::Server(doc));

  return 0;
}
