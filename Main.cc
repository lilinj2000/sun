#include <memory>
#include "SunServer.hh"

int main(int argc, char* argv[])
{
  std::auto_ptr<sun::SunServer> server;

  server.reset( new sun::SunServer(argc, argv) );
  
  return 0;
}
