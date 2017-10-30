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

typedef std::map<
  int32_t,
  std::shared_ptr<air::TimeStampData> > TimeStampRecords;

class Server :
      public foal::TraderCallback {
 public:
  explicit Server(
      const rapidjson::Document& doc);

  virtual ~Server();

  virtual void onRspOrderInsert(
      const std::string& rsp,
      const std::string& err_info,
      int req_id,
      bool is_last);

  virtual void onErrRtnOrderInsert(
      const std::string& rtn,
      const std::string& err_info);

 protected:
  void run();

  void updateT(
      int32_t order_ref,
      bool is_t1 = true);

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
