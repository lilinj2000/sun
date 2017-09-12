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

void Server::onRspError(
    const std::string& rsp) {
  LOG_TRACE("Server::onRspError()");

  LOG_DEBUG("{}", rsp);
}

void Server::onRspQryInvestorAccount(
    const std::string& rsp,
    bool is_last) {
  LOG_TRACE("onRspQryInvestorAccount()");

  LOG_DEBUG("{}", rsp);
}

void Server::onRspQryOrder(
    const std::string& rsp,
    bool is_last) {
  LOG_TRACE("onRspQryOrder()");

  LOG_DEBUG("{}", rsp);
}

void Server::onRspQryTrade(
    const std::string& rsp,
    bool is_last) {
  LOG_TRACE("onRspQryTrade()");

  LOG_DEBUG("{}", rsp);
}

void Server::onRspQryInvestorPosition(
    const std::string& rsp,
    bool is_last) {
  LOG_TRACE("onRspQryInvestorPosition()");

  LOG_DEBUG("{}", rsp);
}

void Server::onRspOrderInsert(
    const std::string& rsp,
    bool is_last) {
  LOG_TRACE("onRspOrderInsert()");

  LOG_DEBUG("{}", rsp);

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
  auto it = records_.find(order_ref);
  if (it != records_.end()) {
    it->second->updateT1();
  }
}

void Server::onRspOrderAction(
    const std::string& rsp,
    bool is_last) {
  LOG_TRACE("onRspOrderAction()");

  LOG_DEBUG("{}", rsp);
}

void Server::onRtnOrder(
    const std::string& rtn) {
  LOG_TRACE("onRtnOrder()");

  LOG_DEBUG("{}", rtn);

  rapidjson::Document doc;
  doc.Parse(rtn);

  std::string key
      = "/CUstpFtdcOrderField/UserOrderLocalID";

  std::string order_local_id;
  soil::json::get_item_value(
      &order_local_id,
      doc,
      key);

  int32_t order_ref = std::stoi(order_local_id);
  auto it = records_.find(order_ref);
  if (it != records_.end()) {
    it->second->updateT2();

    data_file_->putData(it->second);
    records_.erase(it);
  }
}

void Server::onRtnTrade(
    const std::string& rtn) {
  LOG_TRACE("onRtnTrade()");

  LOG_DEBUG("{}", rtn);
}

void Server::onErrRtnOrderInsert(
    const std::string& rtn) {
  LOG_TRACE("onErrRtnOrderInsert()");

  LOG_DEBUG("{}", rtn);
}

void Server::onErrRtnOrderAction(
    const std::string& rtn) {
  LOG_TRACE("onErrRtnOrderAction()");

  LOG_DEBUG("{}", rtn);
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

}  // namespace sun
