#pragma once
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
struct ExchangeRates {
  std::string code;
  int currency_id;
  int target_currency_id;
  double exchange_rate;
  int rate_source_id;
  std::optional<userver::storages::postgres::TimePointTz> created_at;
  std::optional<userver::storages::postgres::TimePointTz> updated_at;
  std::optional<userver::storages::postgres::TimePointTz> deleted_at;
};

inline ExchangeRates Parse(const userver::formats::json::Value& json,
                           userver::formats::parse::To<ExchangeRates>) {
  return ExchangeRates{
      json["code"].As<std::string>(),
      json["currency_id"].As<int>(0),
      json["target_currency_id"].As<int>(0),
      json["value"].As<double>(),
      json["rate_source_id"].As<int>(0),
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
    const ExchangeRates& data,
    userver::formats::serialize::To<userver::formats::json::Value>) {
  userver::formats::json::ValueBuilder builder;
  builder["code"] = data.code;
  builder["currency_id"] = data.currency_id;
  builder["target_currency_id"] = data.target_currency_id;
  builder["exchange_rate"] = data.exchange_rate;
  builder["rate_source_id"] = data.rate_source_id;
  builder["created_at"] = data.created_at;
  builder["updated_at"] = data.updated_at;
  builder["deleted_at"] = data.deleted_at;

  return builder.ExtractValue();
}

}  // namespace models