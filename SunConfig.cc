#include "SunConfig.hh"
#include "SunLog.hh"
#include "fema/TraderService.hh"

#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace sun
{

SunOptions::SunOptions():
    cmd_options_("SunCmdOptions"),
    config_options_("SunConfigOptions")
{

  namespace po = boost::program_options;

  // the options will be allowed only on command line
  cmd_options_.add_options()
      ("load_test", "load test")
      ("query_account", "query account")
      ("query_order", "query order")
      ("just_order", "just order")
      ("order_cancel", po::value<std::string>(&order_sysid)->value_name("order_sysid"),
       "order cancel")
      ;

  config_options_.add_options()
      ("sun.investor_id", po::value<std::string>(&investor_id), 
       "investor id")
      ("sun.instrument_id", po::value<std::string>(&instrument_id), 
       "instrument id")
      ("sun.direction", po::value<char>(&direction), 
       "direction")
      ("sun.price", po::value<double>(&price), 
       "price")
      ("sun.volume", po::value<int>(&volume), 
       "volume")
      ("sun.order_interval", po::value<int>(&order_interval), 
       "order interval")
      ("sun.order_count", po::value<int>(&order_count), 
       "order count")
      ("sun.stat_prefix", po::value<std::string>(&stat_prefix), 
       "stat prefix")
      ("sun.time_accuracy", po::value<std::string>(&time_accuracy), 
       "time accuracy")
      ("sun.log_cfg", po::value<std::string>(&log_cfg), 
       "log config file")
      ;

  return;
  
}

SunOptions::~SunOptions()
{
}

po::options_description* SunOptions::cmdOptions()
{
  return &cmd_options_;
}

po::options_description* SunOptions::configOptions()
{
  return &config_options_;
}

void SunOptions::cmdlineCallback(po::variables_map& vm)
{
  load_test = true;
  query_account = false;
  query_order = false;
  just_order = false;
  order_cancel = false;

  if( !vm.count("load_test") )
  {
    if( vm.count("query_account") )
    {
      query_account = true;
    }

    if( vm.count("query_order") )
    {
      query_order = true;
    }

    if( vm.count("just_order") )
    {
      just_order = true;
    }

    if( vm.count("order_cancel") )
    {
      order_cancel = true;
    }
  }

  if( query_account
      || query_order
      || just_order
      || order_cancel)
  {
    load_test = false;
  }

}

SunConfig::SunConfig(int argc, char* argv[])
{
  sun_options_.reset(new SunOptions());
  fema_trader_options_.reset( fema::TraderService::createOptions() );
      
  std::auto_ptr<soil::Config> config( soil::Config::create() );
  config->registerOptions( sun_options_.get() );
  config->registerOptions( fema_trader_options_.get() );

  config->configFile() = "sun.cfg";
  config->loadConfig(argc, argv);
  
  // init the log
  SUN_LOG_INIT( sun_options_->log_cfg );
  
  return;
}

SunConfig::~SunConfig()
{
}

};  
