#pragma once

#include "components/rate_manager/component.hpp"
#include "userver/server/handlers/http_handler_json_base.hpp"
namespace handlers::currencies {
class Handler final : public userver::server::handlers::HttpHandlerJsonBase {
 public:
  static constexpr std::string_view kName = "handler-currencies";
  Handler(const userver::components::ComponentConfig&,
          const userver::components::ComponentContext&);
  userver::formats::json::Value HandleRequestJsonThrow(
      const userver::server::http::HttpRequest&,
      const userver::formats::json::Value& json,
      userver::server::request::RequestContext&) const override;

 private:
  const components::rate_manager::Component& _rate_manager;
};

}  // namespace handlers::rates