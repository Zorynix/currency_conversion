#pragma once

#include "userver/storages/postgres/query.hpp"
namespace sql {

const userver::storages::postgres::Query kInsertCurrencies{
    "INSERT INTO currencies.currencies(code, name, symbol_native, "
    "decimal_digits, active, main_area_id, created_at, updated_at, deleted_at) VALUES (UNNEST($1), UNNEST($2), "
    "UNNEST($3), UNNEST($4), UNNEST($5), UNNEST($6), UNNEST($7), UNNEST($8), "
    "UNNEST($9));",
    userver::storages::postgres::Query::Name{"InsertCurrencies"}};

const userver::storages::postgres::Query kInsertRates{
    "INSERT INTO currencies.exchange_rates(code, currency_id, target_currency_id, exchange_rate, rate_source_id, created_at, updated_at, deleted_at) VALUES(UNNEST($1), UNNEST($2), "
    "UNNEST($3), UNNEST($4), UNNEST($5), UNNEST($6), UNNEST($7), UNNEST($8)"
    ");",
    userver::storages::postgres::Query::Name{"InsertRates"}};

const userver::storages::postgres::Query kSelectHistoryCount{
    "SELECT COUNT(1) FROM currencies.exchange_rate_histories",
    userver::storages::postgres::Query::Name{"SelectHistoryCount"}};

const userver::storages::postgres::Query kInsertHistory{
    "INSERT INTO currencies.exchange_rate_histories SELECT * FROM currencies.exchange_rates",
    userver::storages::postgres::Query::Name{"InsertHistory"}};


const userver::storages::postgres::Query kUpdateHistory{
    "INSERT INTO currencies.exchange_rate_histories(code, currency_id, target_currency_id, exchange_rate, rate_source_id, created_at, updated_at, deleted_at) SELECT * FROM currencies.exchange_rates ON CONFLICT(code) DO UPDATE SET code = EXCLUDED.code, currency_id = EXCLUDED.currency_id, target_currency_id = EXCLUDED.target_currency_id, exchange_rate = EXCLUDED.exchange_rate,  rate_source_id = EXCLUDED.rate_source_id, created_at = EXCLUDED.created_at , updated_at = EXCLUDED.updated_at , deleted_at = EXCLUDED.deleted_at",
    userver::storages::postgres::Query::Name{"UpdateHistory"}};


    const userver::storages::postgres::Query kGetHistory{
    "SELECT * FROM currencies.exchange_rate_histories;",
    userver::storages::postgres::Query::Name{"GetHistory"}};

}
