#pragma once
#include <cstdint>
#include <string>
#include "userver/formats/json/value.hpp"
#include "userver/formats/parse/to.hpp"
#include "userver/storages/postgres/io/chrono.hpp"

namespace models {
struct Currency {
  std::string code;
  std::string name;
  std::string symbol_native;
  int8_t decimal_digits;
  bool active;
  int main_area_id;
  userver::storages::postgres::TimePointTz created_at;
  userver::storages::postgres::TimePointTz updated_at;
  userver::storages::postgres::TimePointTz deleted_at;
};

inline Currency Parse(const userver::formats::json::Value& json,
                      userver::formats::parse::To<Currency>) {
  return Currency{
      json["code"].As<std::string>(),
      json["name"].As<std::string>(),
      json["symbol_native"].As<std::string>(),
      json["decimal_digits"].As<int8_t>(),
      json["active"].As<bool>(),
      json["main_area_id"].As<int>(),
      json["created_at"].As<userver::storages::postgres::TimePointTz>(),
      json["updated_at"].As<userver::storages::postgres::TimePointTz>(),
      json["deleted_at"].As<userver::storages::postgres::TimePointTz>(),
  };
}

}  // namespace models
