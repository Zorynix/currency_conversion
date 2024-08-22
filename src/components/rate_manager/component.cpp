#include "component.hpp"
#include <unordered_map>
#include <vector>
#include "components/rate_manager/secdist.hpp"
#include "schemas/schemas.hpp"
#include "sql/sql_querries.hpp"
#include "userver/clients/http/component.hpp"
#include "userver/components/loggable_component_base.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "userver/storages/secdist/component.hpp"
#include "userver/yaml_config/merge_schemas.hpp"
#include "userver/yaml_config/schema.hpp"

namespace components::rate_manager {
Component::Component(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context)
    : userver::components::LoggableComponentBase(config, context),
      _pg_cluster(
          context.FindComponent<userver::components::Postgres>("postgres-db-1")
              .GetCluster()),
      _http_client(context.FindComponent<userver::components::HttpClient>()
                       .GetHttpClient()),
      _url_all_currencies(config["url_all_currencies"].As<std::string>()),
      _url_latest_exchange_rates(
          config["url_latest_exchange_rates"].As<std::string>()),
      _secret(context.FindComponent<userver::components::Secdist>()
                  .Get()
                  .Get<secdist_config::Secret>()) {}

std::unordered_map<std::string, models::rates::Currency> Component::GetCurrencies()
    const {
  const auto& responce =
      _http_client.CreateRequest()
          .get(_url_all_currencies)
          .headers({{"apikey", _secret.api_key.GetUnderlying()}})
          .perform();

  const auto& result =
      userver::formats::json::FromString(responce->body_view())["data"]
          .As<std::unordered_map<std::string, models::rates::Currency>>();

  return result;
}

std::unordered_map<std::string, models::rates::Currency> Component::AddCurrencies()
    const {
  const auto& data = GetCurrencies();

  auto trx = _pg_cluster->Begin({});

  std::vector<models::rates::Currency> currencies;
  currencies.reserve(data.size());

  for (const auto& [key, value] : data) {
    currencies.push_back(value);
  }

  trx.ExecuteDecomposeBulk(sql::kInsertCurrencies, currencies);
  trx.Commit();

  return data;
}

std::unordered_map<std::string, models::rates::ExchangeRate>
Component::GetExchangeRates() const {
  const auto& responce =
      _http_client.CreateRequest()
          .get(_url_latest_exchange_rates)
          .headers({{"apikey", _secret.api_key.GetUnderlying()}})
          .perform();

  const auto& result =
      userver::formats::json::FromString(responce->body_view())["data"]
          .As<std::unordered_map<std::string, models::rates::ExchangeRate>>();

  return result;
}

std::unordered_map<std::string, models::rates::ExchangeRate>
Component::AddExchangeRates() const {
  auto trx = _pg_cluster->Begin({});

  const auto& data = GetExchangeRates();

  std::vector<models::rates::ExchangeRate> rates;
  rates.reserve(data.size());

  for (const auto& [key, value] : data) {
    rates.push_back(value);
  }

  trx.ExecuteDecomposeBulk(sql::kInsertRates, rates);

  trx.Commit();

  return data;
}

std::unordered_map<std::string, models::rates::ExchangeRate> Component::UpdateRates()
    const {
  auto trx = _pg_cluster->Begin(
      "trx__update_rates",
      userver::storages::postgres::ClusterHostType::kMaster, {});

  std::unordered_map<std::string, models::rates::ExchangeRate> result;

  trx.Execute(sql::kInsertHistory);

  auto data = trx.Execute(sql::kGetHistory)
                  .AsContainer<std::vector<models::rates::ExchangeRate>>(
                      userver::storages::postgres::kRowTag);

  for (const auto& rate : data) {
    result[rate.code] = rate;
  }

  AddExchangeRates();

  trx.Commit();

  return result;
}

userver::yaml_config::Schema Component::GetStaticConfigSchema() {
  return userver::yaml_config::MergeSchemas<
      userver::components::LoggableComponentBase>(R"(
type: object
description: test component
additionalProperties: false
properties: 
  url_all_currencies: 
    type: string
    description: url for all currencies
  url_latest_exchange_rates:
    type: string
    description: url for exchange rates
  )");
}

}  // namespace components::rate_manager