#include "handler.hpp"
#include "components/rate_manager/component.hpp"
#include "userver/server/handlers/http_handler_json_base.hpp"

namespace handlers::rates {
Handler::Handler(const userver::components::ComponentConfig& config,
                 const userver::components::ComponentContext& context)
    : userver::server::handlers::HttpHandlerJsonBase(config, context),
      _rate_manager(
          context.FindComponent<components::rate_manager::Component>()){};

userver::formats::json::Value Handler::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest&,
    const userver::formats::json::Value&,
    userver::server::request::RequestContext&) const {
  auto data = _rate_manager.AddExchangeRates();

  return userver::formats::json::ValueBuilder{data}.ExtractValue();
}
}  // namespace handlers::rates