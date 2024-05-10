#pragma once
#include <string>
#include "userver/formats/json/value.hpp"
#include "userver/formats/parse/to.hpp"
#include "userver/storages/postgres/io/chrono.hpp"

namespace models {
struct ExchangeRates {
  std::string code;
  int currency_id;
  int target_currency_id;
  double exchange_rate;
  int rate_source_id;
  userver::storages::postgres::TimePointTz created_at;
  userver::storages::postgres::TimePointTz updated_at;
  userver::storages::postgres::TimePointTz deleted_at;
};

inline ExchangeRates Parse(const userver::formats::json::Value& json,
                           userver::formats::parse::To<ExchangeRates>) {
  return ExchangeRates{
      json["code"].As<std::string>(),
      json["currency_id"].As<int>(),
      json["target_currency_id"].As<int>(),
      json["exchange_rate"].As<double>(),
      json["rate_source_id"].As<int>(),
      json["created_at"].As<userver::storages::postgres::TimePointTz>(),
      json["updated_at"].As<userver::storages::postgres::TimePointTz>(),
      json["deleted_at"].As<userver::storages::postgres::TimePointTz>(),
  };
}

}  // namespace models