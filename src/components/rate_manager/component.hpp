#pragma once

#include <unordered_map>
#include <userver/storages/postgres/postgres_fwd.hpp>
#include "components/rate_manager/secdist.hpp"
#include "schemas/schemas.hpp"
#include "userver/clients/http/client.hpp"
#include "userver/components/component_config.hpp"
#include "userver/components/component_context.hpp"
#include "userver/components/loggable_component_base.hpp"
#include "userver/yaml_config/schema.hpp"

namespace components::rate_manager {
class Component final : public userver::components::LoggableComponentBase {
 public:
  static constexpr std::string_view kName = "rate-manager";
  Component(const userver::components::ComponentConfig&,
            const userver::components::ComponentContext&);
  std::unordered_map<std::string, models::rates::Currency> GetCurrencies() const;
  std::unordered_map<std::string, models::rates::Currency> AddCurrencies() const;
  std::unordered_map<std::string, models::rates::ExchangeRate> GetExchangeRates()
      const;
  std::unordered_map<std::string, models::rates::ExchangeRate> AddExchangeRates()
      const;
  std::unordered_map<std::string, models::rates::ExchangeRate> UpdateRates() const;
  static userver::yaml_config::Schema GetStaticConfigSchema();

 private:
  userver::storages::postgres::ClusterPtr _pg_cluster;
  userver::clients::http::Client& _http_client;
  std::string _url_all_currencies;
  std::string _url_latest_exchange_rates;
  secdist_config::Secret _secret;
};
}  // namespace components::rate_manager