DROP SCHEMA IF EXISTS currencies CASCADE;

CREATE SCHEMA IF NOT EXISTS currencies;

CREATE TABLE IF NOT EXISTS currencies.currencies (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    code VARCHAR(255) NOT NULL UNIQUE,
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
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    code VARCHAR(255) NOT NULL UNIQUE,
    currency_id INT,
    target_currency_id INT,
    exchange_rate FLOAT,
    rate_source_id INT,
    created_at TimestampTz,
    updated_at TimestampTz,
    deleted_at TimestampTz
);

CREATE TABLE IF NOT EXISTS currencies.exchange_rate_histories (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    code VARCHAR(255) NOT NULL UNIQUE,
    currency_id INT,
    target_currency_id INT,
    exchange_rate FLOAT,
    rate_source_id INT,
    created_at TimestampTz,
    updated_at TimestampTz,
    deleted_at TimestampTz
);