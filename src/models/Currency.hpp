#pragma once
#include <cstdint>
#include <string>
#include "userver/formats/json/value.hpp"
#include "userver/formats/json/value_builder.hpp"
#include "userver/formats/parse/common_containers.hpp"
#include "userver/formats/parse/to.hpp"
#include "userver/formats/serialize/common_containers.hpp"
#include "userver/formats/serialize/to.hpp"
#include "userver/storages/postgres/io/chrono.hpp"
#include "userver/utils/datetime.hpp"

namespace models {
struct Currency {
  std::string code;
  std::string name;
  std::string symbol_native;
  int decimal_digits;
  std::optional<userver::storages::postgres::TimePointTz> created_at;
  std::optional<userver::storages::postgres::TimePointTz> updated_at;
  std::optional<userver::storages::postgres::TimePointTz> deleted_at;
};

inline Currency Parse(const userver::formats::json::Value& json,
                      userver::formats::parse::To<Currency>) {
  return Currency{
      json["code"].As<std::string>(),
      json["name"].As<std::string>(),
      json["symbol_native"].As<std::string>(),
      json["decimal_digits"].As<int8_t>(),
      json["created_at"]
          .As<std::optional<userver::storages::postgres::TimePointTz>>(
              userver::utils::datetime::Now()),
      json["updated_at"]
          .As<std::optional<userver::storages::postgres::TimePointTz>>(
              userver::utils::datetime::Now()),
      json["deleted_at"]
          .As<std::optional<userver::storages::postgres::TimePointTz>>(
              userver::utils::datetime::Now()),
  };
}

inline userver::formats::json::Value Serialize(
    const Currency& data,
    userver::formats::serialize::To<userver::formats::json::Value>) {
  userver::formats::json::ValueBuilder builder;
  builder["code"] = data.code;
  builder["name"] = data.name;
  builder["symbol_native"] = data.symbol_native;
  builder["decimal_digits"] = data.decimal_digits;
  builder["created_at"] = data.created_at;
  builder["updated_at"] = data.updated_at;
  builder["deleted_at"] = data.deleted_at;

  return builder.ExtractValue();
}

}  // namespace models
