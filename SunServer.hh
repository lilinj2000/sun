#include <memory>
#include <stdexcept>
#include <map>
#include "SunConfig.hh"
#include "fema/TraderService.hh"
#include "soil/Condition.hh"
#include <boost/chrono/chrono.hpp>

namespace sun
{

template <class Clock=boost::chrono::high_resolution_clock>
class OrderRecord
{
 public:
  typedef Clock                       clock;
  typedef typename Clock::duration    duration;
  typedef typename Clock::time_point  time_point;

  time_point order_request;
  time_point order_response;
  time_point order_return;
};
typedef OrderRecord< boost::chrono::system_clock > system_order_record;
typedef OrderRecord< boost::chrono::high_resolution_clock > high_resolution_order_record;

class SunServer : public fema::TraderServiceCallback
{
 public:
  SunServer(int argc, char *argv[]);

  ~SunServer();

  void loadTest();

  virtual void onRspError(int errord_id, const std::string& error_msg);
  
  virtual void onRspOrderInsert(int order_local_id, bool success);

  virtual void onRspOrderAction(int order_local_id, int canceled_local_id, bool success);

  virtual void onRtnOrder(int order_local_id, const std::string& order_status);

  virtual void onRtnTrade(int order_local_id,
                          double price, int volume);

  
 protected:
  
  void wait(int ms=2000 );

  void statInfo();

  int convertDuration(const high_resolution_order_record::duration& du);

  int minValue(const std::vector<int>& values);

  int maxValue(const std::vector<int>& values);

  int avgValue(const std::vector<int>& values);

 private:
  
  std::auto_ptr<fema::TraderService> service_;

  std::auto_ptr<SunConfig> sun_cfg_;
  
  SunOptions* sun_options_;
  
  std::auto_ptr<soil::Condition> cond_;

  std::map<int, high_resolution_order_record> order_records_;
  std::map<int, high_resolution_order_record> order_action_records_;

  std::map<int, int> order_action_;
  // high_resolution_order_record::time_point start_point_;
};

};  // namespace sun
