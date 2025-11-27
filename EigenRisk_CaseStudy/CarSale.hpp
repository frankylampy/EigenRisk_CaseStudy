#pragma once
#ifndef CARSALE_HPP
#define CARSALE_HPP
#endif // !CARSALE_HPP

#include <string>
#include <chrono>
#include <unordered_map>

enum class CarSaleField {
    SALE_ID = 0,
    SALE_DATE,
    COUNTRY,
    REGION,
    LATITUDE,
    LONGITUDE,
    DEALERSHIP_ID,
    DEALERSHIP_NAME,
    MANUFACTURER,
    Make,
    VEHICLE_YEAR,
    BODY_TYPE,
    FUEL_TYPE,
    TRANSMISSION,
    DRIVETRAIN,
    COLOR,
    VIN,
    CONDITION,
    PREVIOUS_OWNERS,
    ODOMETER_KM,
    SALE_PRICE_USD,
    CURRENCY,
    FINANCING,
    PAYMENT_TYPE,
    SALES_CHANNEL,
    BUYER_ID,
    BUYER_AGE,
    BUYER_GENDER,
    BUYER_INCOME_USD,
    SALESPERSON_ID,
    SALESPERSON_NAME,
    WARRANTY_MONTHS,
    WARRANTY_PROVIDER,
    FEATURES,
    CO2_G_KM,
    MPG_CITY,
    MPG_HIGHWAY,
    ENGINE_DISPLACEMENT_L,
    HORSEPOWER,
    TORQUE_NM,
    DEALER_RATING,
    CONDITION_NOTES,
    SERVICE_HISTORY,
    COUNT   // useful for array size
};

struct CarSale {
    std::string sale_id;
    std::chrono::year_month_day sale_date;      // or std::chrono::year_month_day
    std::string country;
    std::string region;
    double latitude;
    double longitude;
    std::string dealership_id;
    std::string dealership_name;
    std::string manufacturer;
    std::string Make;
    int vehicle_year;
    std::string body_type;
    std::string fuel_type;
    std::string transmission;
    std::string drivetrain;
    std::string color;
    std::string vin;
    std::string condition;
    int previous_owners;
    double odometer_km;
    double sale_price_usd;
    std::string currency;
    std::string financing;
    std::string payment_type;
    std::string sales_channel;
    int buyer_id;
    int buyer_age;
    std::string buyer_gender;
    double buyer_income_usd;
    int salesperson_id;
    std::string salesperson_name;
    int warranty_months;
    std::string warranty_provider;
    std::string features;
    double co2_g_km;
    double mpg_city;
    double mpg_highway;
    double engine_displacement_l;
    int horsepower;
    int torque_nm;
    double dealer_rating;
    std::string condition_notes;
    std::string service_history;
};


std::unique_ptr<CarSale> parseCarSale(const std::string& line);

