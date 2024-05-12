#include "secdist.hpp"
#include "userver/formats/json/value.hpp"

namespace components::rate_manager::secdist_config {

Secret::Secret(const userver::formats::json::Value& data)
    : api_key(data["API_KEY"].As<std::string>()) {}

}  // namespace components::rate_manager::secdist_config