#include "SunServer.hh"
#include "SunLog.hh"

#include <fstream>
#include <algorithm>
#include <numeric>

namespace sun
{

SunServer::SunServer(int argc, char* argv[])
{
  try
  {
    cond_.reset( soil::Condition::create() );
    
    sun_cfg_.reset( new SunConfig(argc, argv) );

    service_.reset( fema::TraderService::createService(sun_cfg_->femaTraderOptions(), this) );

    sun_options_ = sun_cfg_->sunOptions();
    
    if( sun_options_->query_account )
    {
      service_->queryAccount(sun_options_->investor_id);

      wait();
    }

    if( sun_options_->query_order )
    {
      service_->queryOrder(sun_options_->investor_id);

      wait();
    }

    if( sun_options_->just_order )
    {
      service_->orderInsert(sun_options_->investor_id,
                  sun_options_->instrument_id,
                  sun_options_->direction,
                  sun_options_->price,
                  sun_options_->volume);

      wait();
    }

    if( sun_options_->order_cancel )
    {
      service_->orderAction(sun_options_->investor_id,
                            sun_options_->order_sysid);

      wait();
    }

    if( sun_options_->load_test )
    {
      loadTest();
    }
  }
  catch(std::exception& e)
  {
    SUN_ERROR <<e.what();
  }
}

SunServer::~SunServer()
{
}

void SunServer::loadTest()
{
  int count = 0;

  while( count<sun_options_->order_count )
  {
    int i_send = ++count;
    
    SUN_CUSTOM <<"send " <<i_send;

    int order_local_id = service_->orderInsert(sun_options_->investor_id,
                                               sun_options_->instrument_id,
                                               sun_options_->direction,
                                               sun_options_->price,
                                               sun_options_->volume);

    order_records_[order_local_id].order_request = high_resolution_order_record::clock::now();

    wait(sun_options_->order_interval);
  }

  // ensure the last the order be processed.
  wait();

  statInfo();
}

void SunServer::onRspError(int error_id, const std::string& error_msg)
{
  SUN_TRACE <<"SunServer::onRspError";

  SUN_ERROR <<"error_id: " <<error_id
            <<" error_msg: " <<error_msg;
}
  
void SunServer::onRspOrderInsert(int order_local_id, bool success)
{
  SUN_TRACE <<"SunServer::onRspOrderInsert";

  SUN_INFO <<"order_local_id: " <<order_local_id
           <<" scuccess: " <<std::boolalpha <<success;

  order_records_[order_local_id].order_response = high_resolution_order_record::clock::now();
}

void SunServer::onRspOrderAction(int order_local_id, int canceled_local_id, bool success)
{
  SUN_TRACE <<"SunServer::onRspOrderAction";

  SUN_INFO <<"order_local_id: " <<order_local_id
           <<" canceled_local_id: " <<canceled_local_id
           <<" success: " <<std::boolalpha <<success;

  order_action_records_[order_local_id].order_response = high_resolution_order_record::clock::now();
}

void SunServer::onRtnOrder(int order_local_id, const std::string& order_status)
{
  SUN_TRACE <<"SunServer::onRtnOrder";

  SUN_INFO <<"order_local_id: " <<order_local_id
           <<" order_status: " <<order_status;

  if( !order_action_.count( order_local_id ) )
  {
      order_records_[order_local_id].order_return = high_resolution_order_record::clock::now();

      int action_local_id = service_->orderAction(sun_options_->investor_id, order_local_id);
      
      order_action_records_[action_local_id].order_request = high_resolution_order_record::clock::now();
      
      order_action_[order_local_id] = action_local_id;
  }
  else if( order_action_.count( order_local_id ) )
  {
    int action_local_id = order_action_[order_local_id];
    order_action_records_[action_local_id].order_return = high_resolution_order_record::clock::now();
  }
  else
  {
    SUN_ERROR <<"can't find the local id - " <<order_local_id;
  }
}

void SunServer::onRtnTrade(int order_local_id, double price, int volume)
{
  SUN_TRACE <<"SunServer::onRtnTrade";

  SUN_INFO <<"order_local_id: " <<order_local_id
           <<" price: " <<price
           <<" volume: " <<volume;
}


void SunServer::wait(int ms)
{
  cond_->wait( ms );
}

void SunServer::statInfo()
{
  std::map<int, high_resolution_order_record>::iterator i_order = order_records_.begin();

  std::string stat_file = sun_options_->stat_prefix + "_order.dat";
  std::ofstream order_file(stat_file.data());
  order_file <<"order_seq \t order_request \t order_response \t order_return" <<std::endl;

  std::vector<int> rspList;
  std::vector<int> rtnList;
  for(; i_order!=order_records_.end(); i_order++)
  {
    order_file <<i_order->first <<" \t"
               <<convertDuration(i_order->second.order_request - i_order->second.order_request) <<" \t";
    int rsp_time = convertDuration(i_order->second.order_response - i_order->second.order_request);
    rspList.push_back(rsp_time);
    order_file <<rsp_time <<" \t";
    
    int rtn_time = convertDuration(i_order->second.order_return - i_order->second.order_request);
    rtnList.push_back(rtn_time);
    order_file <<rtn_time <<std::endl;
  }

  order_file <<"order_response min: " <<minValue(rspList)
             <<" max: " <<maxValue(rspList)
             <<" average: " <<avgValue(rspList) <<std::endl;

  order_file <<"order_return min: " <<minValue(rtnList)
             <<" max: " <<maxValue(rtnList)
             <<" average: " <<avgValue(rtnList) <<std::endl;

  std::string stat_action_file = sun_options_->stat_prefix + "_order_action.dat";
  std::ofstream order_action_file(stat_action_file.data());
  order_action_file <<"order_action_seq \t order_action_request \t order_action_response \t order_action_return" <<std::endl;

  rspList.clear();
  rtnList.clear();
  i_order = order_action_records_.begin();
  for(; i_order!=order_action_records_.end(); i_order++)
  {
    order_action_file <<i_order->first <<" \t"
                      <<convertDuration(i_order->second.order_request - i_order->second.order_request) <<" \t";
    
    int rsp_time = convertDuration(i_order->second.order_response - i_order->second.order_request);
    rspList.push_back(rsp_time);
    order_action_file <<rsp_time <<" \t";

    int rtn_time = convertDuration(i_order->second.order_return - i_order->second.order_request);
    rtnList.push_back(rtn_time);
    order_action_file <<rtn_time <<std::endl;
  }

  order_action_file <<"order_action_response min: " <<minValue(rspList)
                    <<" max: " <<maxValue(rspList)
                    <<" average: " <<avgValue(rspList) <<std::endl;

  order_action_file <<"order_action_return min: " <<minValue(rtnList)
                    <<" max: " <<maxValue(rtnList)
                    <<" average: " <<avgValue(rtnList) <<std::endl;

}

int SunServer::convertDuration(const high_resolution_order_record::duration& du)
{
  typedef boost::chrono::milliseconds millis;
  typedef boost::chrono::microseconds micros;
  typedef boost::chrono::nanoseconds nanos;
    
  if( sun_options_->time_accuracy == "milliseconds" )
    return boost::chrono::duration_cast<millis>(du).count();
  else if( sun_options_->time_accuracy == "microseconds" )
    return boost::chrono::duration_cast<micros>(du).count();
  else if( sun_options_->time_accuracy == "nanoseconds" )
    return boost::chrono::duration_cast<nanos>(du).count();
  else
    throw std::runtime_error("time accuracy config error - " + sun_options_->time_accuracy);

}

int SunServer::minValue(const std::vector<int>& values)
{
  return *std::min_element(values.begin(), values.end());
}

int SunServer::maxValue(const std::vector<int>& values)
{
  return *std::max_element(values.begin(), values.end());
}

int SunServer::avgValue(const std::vector<int>& values)
{
  return std::accumulate(values.begin(), values.end(), 0.0) / values.size() ;
}


};  // namespace sun
