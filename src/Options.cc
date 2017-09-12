// Copyright (c) 2010
// All rights reserved.

#include "Options.hh"

namespace sun {

using soil::json::get_item_value;

Options::Options(
      const rapidjson::Document& doc) {
  get_item_value(&instru, doc, "/sun/instru");
  get_item_value(&price, doc, "/sun/price");
  get_item_value(&volume, doc, "/sun/volume");
  get_item_value(&interval, doc, "/sun/interval");
  get_item_value(&count, doc, "/sun/count");

  get_item_value(&data_file, doc, "/sun/data_file");
}

};  // namespace sun
