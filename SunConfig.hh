#ifndef SUN_CONFIG_HH
#define SUN_CONFIG_HH

#include <string>
#include <memory>
#include "soil/Config.hh"

namespace sun
{

namespace po = boost::program_options;

class SunOptions : public soil::Options
{
 public:

  SunOptions();
  
  virtual ~SunOptions();

  virtual po::options_description* cmdOptions();

  virtual po::options_description* configOptions();

  virtual void cmdlineCallback(po::variables_map& vm);

  bool load_test;
  bool query_account;
  bool query_order;
  bool just_order;
  bool order_cancel;
  std::string investor_id;
  std::string instrument_id;
  char direction;
  double price;
  int volume;

  int order_interval;
  int order_count;
  std::string stat_prefix;
  std::string time_accuracy;
  std::string log_cfg;
  std::string order_sysid;

 private:
  boost::program_options::options_description config_options_;

  boost::program_options::options_description cmd_options_;
};

class SunConfig
{
 public:
  
  SunConfig(int argc=0, char* argv[]=NULL);
  ~SunConfig();

  SunOptions* sunOptions()
  {
    return sun_options_.get();
  }

  soil::Options* femaTraderOptions()
  {
    return fema_trader_options_.get();
  }

 private:
  std::auto_ptr<SunOptions> sun_options_;
  
  std::auto_ptr<soil::Options> fema_trader_options_;
};

}  // namespace sun


#endif 
