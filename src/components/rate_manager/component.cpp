#include "component.hpp"
#include <unordered_map>
#include <vector>
#include "components/rate_manager/secdist.hpp"
#include "models/Currency.hpp"
#include "models/ExchangeRates.hpp"
#include "sql/sql_querries.hpp"
#include "userver/clients/http/component.hpp"
#include "userver/clients/http/response.hpp"
#include "userver/components/loggable_component_base.hpp"
#include "userver/http/predefined_header.hpp"
#include "userver/storages/postgres/cluster.hpp"
#include "userver/storages/postgres/cluster_types.hpp"
#include "userver/storages/postgres/component.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "userver/storages/secdist/component.hpp"
#include "userver/utils/from_string.hpp"

static constexpr userver::http::headers::PredefinedHeader kApiKey{"apikey"};

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

std::unordered_map<std::string, models::Currency> Component::GetCurrencies()
    const {
  const auto& responce = _http_client.CreateRequest()
                             .get(_url_all_currencies)
                             .headers(userver::clients::http::Headers(
                                 {{kApiKey, _secret.api_key}}))
                             .perform();

  const auto& result =
      userver::formats::json::FromString(responce->body_view())["data"]
          .As<std::unordered_map<std::string, models::Currency>>();

  return result;
}

std::unordered_map<std::string, models::Currency> Component::AddCurrencies()
    const {
  const auto& data = GetCurrencies();

  auto trx = _pg_cluster->Begin({});

  std::vector<models::Currency> currencies;
  currencies.reserve(data.size());

  for (const auto& [key, value] : data) {
    currencies.push_back(value);
  }

  trx.ExecuteDecomposeBulk(sql::kInsertCurrencies, currencies);
  trx.Commit();

  return data;
}

std::unordered_map<std::string, models::ExchangeRates>
Component::GetExchangeRates() const {
    const auto& responce = _http_client.CreateRequest().get(_url_latest_exchange_rates).headers(userver::clients::http::Headers({{kApiKey, _secret.api_key}})).perform();

    const auto& result = userver::formats::json::FromString(responce.body_view())["data"].As<std::unordered_map<std::string, models::ExchangeRates>>();


    return result;

}

std::unordered_map<std::string, models::ExchangeRates>
Component::AddExchangeRates() const {
    
    auto trx = _pg_cluster->Begin({});

    const auto& data = GetExchangeRates();

    std::vector<models::ExchangeRates> rates;
    rates.reserve(data.size());

    for (const auto& [key, value] : data) {
        rates.push_back(value);
    }

    trx.ExecuteDecomposeBulk(sql::kInsertRates, rates);
    trx.Commit();

    return data;

}

std::unordered_map<std::string, models::ExchangeRates>
Component::UpdateRates()
    const { 
        
        auto trx = _pg_cluster->Begin("trx__update_rates", userver::storages::postgres::ClusterHostType::kSlave, {});
        
        auto data = trx.Execute(sql::kGetHistory).AsContainer<std::vector<models::ExchangeRates>>(userver::storages::postgres::kRowTag);

        std::unordered_map<std::string, models::ExchangeRates> result;

        for (const auto& rate : data) {
        result[rate.code] = rate;
    }

        auto count = trx.Execute(sql::kSelectHistoryCount).AsSingleRow<int>();

        if (count == 0) {
            trx.Execute(sql::kInsertHistory);
        }
        else {
            trx.Execute(sql::kUpdateHistory);
        }

        AddExchangeRates();

        return result;

    }

}  // namespace components::rate_manager