DROP SCHEMA IF EXISTS currencies CASCADE;

CREATE SCHEMA IF NOT EXISTS currencies;

CREATE TABLE IF NOT EXISTS currencies.currencies (
    code VARCHAR(255) PRIMARY KEY,
    name VARCHAR(255),
    symbol_native VARCHAR(10),
    decimal_digits INT,
    active BOOLEAN,
    main_area_id INT,
    created_at TimestampTz,
    updated_at TimestampTz,
    deleted_at TimestampTz
);

CREATE TABLE IF NOT EXISTS currencies.exchange_rates (
    code VARCHAR(255) PRIMARY KEY,
    currency_id INT,
    target_currency_id INT,
    exchange_rate FLOAT,
    rate_source_id INT,
    created_at TimestampTz,
    updated_at TimestampTz,
    deleted_at TimestampTz
);

CREATE TABLE IF NOT EXISTS currencies.exchange_rate_histories (
    code VARCHAR(255) PRIMARY KEY,
    currency_id INT,
    target_currency_id INT,
    exchange_rate FLOAT,
    rate_source_id INT,
    created_at TimestampTz,
    updated_at TimestampTz,
    deleted_at TimestampTz
);