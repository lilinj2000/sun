// Copyright (c) 2010
// All rights reserved.

#ifndef SUN_SERVER_HH
#define SUN_SERVER_HH

#include <map>
#include <string>
#include "Options.hh"
#include "soil/STimer.hh"
#include "foal/TraderService.hh"
#include "air/TimeStampDataFile.hh"

namespace sun {

typedef std::map<int32_t, air::TimeStampData*> TimeStampRecords;

class Server :
      public foal::TraderCallback {
 public:
  explicit Server(
      const rapidjson::Document& doc);

  virtual ~Server();

  virtual void onRspError(
      const std::string& rsp);

  virtual void onRspQryInvestorAccount(
      const std::string& rsp,
      bool is_last);

  virtual void onRspQryOrder(
      const std::string& rsp,
      bool is_last);

  virtual void onRspQryTrade(
      const std::string& rsp,
      bool is_last);

  virtual void onRspQryInvestorPosition(
      const std::string& rsp,
      bool is_last);

  virtual void onRspOrderInsert(
      const std::string&,
      bool is_last);

  virtual void onRspOrderAction(
      const std::string&,
      bool is_last);

  virtual void onRtnOrder(
      const std::string&);

  virtual void onRtnTrade(
      const std::string&);

  virtual void onErrRtnOrderInsert(
      const std::string&);

  virtual void onErrRtnOrderAction(
      const std::string&);

 protected:
  void run();

  void wait(int ms = -1) {
    cond_->wait(ms);
  }

  void notify(bool is_last) {
    if (is_last) {
      cond_->notifyAll();
    }
  }

 private:
  std::unique_ptr<Options> options_;
  std::unique_ptr<foal::TraderService> service_;

  TimeStampRecords records_;
  std::unique_ptr<air::TimeStampDataFile> data_file_;

  std::unique_ptr<soil::STimer> cond_;
};

}  // namespace sun

#endif
