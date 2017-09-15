// Copyright (c) 2010
// All rights reserved.

#include "Server.hh"
#include "soil/Log.hh"
#include "soil/STimer.hh"

namespace sun {

Server::Server(
    const rapidjson::Document& doc) {
  LOG_TRACE("Server::Server()");

  options_.reset(new Options(doc));
  service_.reset(
      foal::TraderService::create(
          doc,
          this));

  if (!options_->data_file.empty()) {
    data_file_.reset(
        new air::TimeStampDataFile(
            options_->data_file));
  }

  cond_.reset(soil::STimer::create());

  run();
}

Server::~Server() {
  LOG_TRACE("Server::~Server()");
}

void Server::onRspOrderInsert(
    const std::string& rsp,
    const std::string& err_info,
    int req_id,
    bool is_last) {
  FOAL_ON_RSP_PRINT(
      rsp,
      err_info,
      req_id,
      is_last);

  rapidjson::Document doc;
  doc.Parse(rsp);

  std::string key
      = "/CUstpFtdcInputOrderField/UserOrderLocalID";

  std::string order_local_id;
  soil::json::get_item_value(
      &order_local_id,
      doc,
      key);

  int32_t order_ref = std::stoi(order_local_id);
  updateT(order_ref);
}

void Server::onErrRtnOrderInsert(
    const std::string& rtn,
    const std::string& err_info) {
  FOAL_ON_ERRRTN_PRINT(
      rtn,
      err_info);

  rapidjson::Document doc;
  doc.Parse(rtn);

  std::string key
      = "/CUstpFtdcInputOrderField/UserOrderLocalID";

  std::string order_local_id;
  soil::json::get_item_value(
      &order_local_id,
      doc,
      key);

  int32_t order_ref = std::stoi(order_local_id);
  updateT(order_ref, false);
}

void Server::run() {
  LOG_TRACE("Server::run()");

  int count = 0;

  do {
    int32_t order_ref = -1;

    order_ref = service_->openBuyOrderFOK(
        options_->instru,
        options_->price,
        options_->volume);

    records_[order_ref]
        = new air::TimeStampData(order_ref);

    ++count;

    if (options_->count > 0
        && count >= options_->count) {
      break;
    }

    wait(options_->interval);
  }while(true);

  wait(options_->interval);
}

void Server::updateT(
    int32_t order_ref,
    bool is_t1) {
  auto it = records_.find(order_ref);
  if (it != records_.end()) {
    if (is_t1) {
      it->second->updateT1();
    } else {
      it->second->updateT2();
    }

    if (it->second->allUpdated()) {
      data_file_->putData(it->second);
      records_.erase(it);
    }
  }
}

}  // namespace sun
