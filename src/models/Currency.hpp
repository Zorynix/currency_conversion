#pragma once
#include <cstdint>
#include <string>
#include "userver/formats/json/value.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/formats/parse/to.hpp"
#include "userver/storages/postgres/io/chrono.hpp"
#include "userver/formats/serialize/to.hpp"

namespace models {
struct Currency {
  std::string code;
  std::string name;
  std::string symbol_native;
  int decimal_digits;
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


inline userver::formats::json::Value Serialize(const Currency& data, userver::formats::serialize::To<userver::formats::json::Value>){
  userver::formats::json::ValueBuilder builder;
  builder["code"] = data.code;
  builder["name"] = data.name;
  builder["symbol_native"] = data.symbol_native;
  builder["decimal_digits"] = data.decimal_digits;
  builder["active"] = data.active;
  builder["main_area_id"] = data.main_area_id;
  builder["created_at"] = data.created_at;
  builder["updated_at"] = data.updated_at;
  builder["deleted_at"] = data.deleted_at;

  return builder.ExtractValue();
}


}  // namespace models
