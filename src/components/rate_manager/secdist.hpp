#pragma once

#include "userver/formats/json/parser/parser_json.hpp"
#include "userver/formats/json/value.hpp"
#include "userver/utils/strong_typedef.hpp"
namespace components::rate_manager::secdist_config {

using ApiKey = userver::utils::NonLoggable<class ApiKeyTag, std::string>;

struct Secret {
  explicit Secret(const userver::formats::json::Value&);
  ApiKey api_key;
};

Secret Parse(const userver::formats::json::Value& json,
             userver::formats::parse::To<Secret>);

}  // namespace components::rate_manager::secdist_config