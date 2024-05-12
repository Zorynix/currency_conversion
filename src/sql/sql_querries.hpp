#pragma once

#include "userver/storages/postgres/query.hpp"
namespace sql {

const userver::storages::postgres::Query kInsertCurrencies{
    "INSERT INTO currencies.currencies(code, name, symbol_native, "
    "decimal_digits, created_at, updated_at, deleted_at) "
    "VALUES (UNNEST($1), UNNEST($2), "
    "UNNEST($3), UNNEST($4), UNNEST($5), UNNEST($6), UNNEST($7)) "
    "ON CONFLICT (code) DO UPDATE SET "
    "name = EXCLUDED.name, "
    "symbol_native = EXCLUDED.symbol_native, "
    "decimal_digits = EXCLUDED.decimal_digits, "
    "updated_at = EXCLUDED.updated_at, "
    "deleted_at = EXCLUDED.deleted_at;",
    userver::storages::postgres::Query::Name{"InsertCurrencies"}};

const userver::storages::postgres::Query kInsertRates{
    "INSERT INTO currencies.exchange_rates(code, currency_id, "
    "target_currency_id, exchange_rate, rate_source_id, created_at, "
    "updated_at, deleted_at) VALUES (UNNEST($1), UNNEST($2), "
    "UNNEST($3), UNNEST($4), UNNEST($5), UNNEST($6), UNNEST($7), UNNEST($8)"
    ") ON CONFLICT (code) DO UPDATE SET currency_id = EXCLUDED.currency_id, "
    "target_currency_id = EXCLUDED.target_currency_id, exchange_rate = "
    "EXCLUDED.exchange_rate, rate_source_id = EXCLUDED.rate_source_id, "
    "updated_at = EXCLUDED.updated_at, deleted_at = EXCLUDED.deleted_at;",
    userver::storages::postgres::Query::Name{"InsertRates"}};

const userver::storages::postgres::Query kInsertHistory{
    "INSERT INTO currencies.exchange_rate_histories(code, currency_id, target_currency_id, exchange_rate, rate_source_id, created_at, updated_at, deleted_at) SELECT code, currency_id, target_currency_id, exchange_rate, rate_source_id, created_at, updated_at, deleted_at FROM "
    "currencies.exchange_rates ON CONFLICT (code) DO UPDATE SET currency_id = "
    "EXCLUDED.currency_id, target_currency_id = EXCLUDED.target_currency_id, "
    "exchange_rate = EXCLUDED.exchange_rate, rate_source_id = "
    "EXCLUDED.rate_source_id, updated_at = EXCLUDED.updated_at, deleted_at = "
    "EXCLUDED.deleted_at",
    userver::storages::postgres::Query::Name{"InsertHistory"}};

const userver::storages::postgres::Query kGetHistory{
    "SELECT code, currency_id, target_currency_id, exchange_rate, rate_source_id, created_at, updated_at, deleted_at FROM currencies.exchange_rate_histories;",
    userver::storages::postgres::Query::Name{"GetHistory"}};

}  // namespace sql
