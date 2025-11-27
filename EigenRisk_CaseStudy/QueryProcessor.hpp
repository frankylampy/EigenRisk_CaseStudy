#pragma once
#include <iostream>
#include <string>
#include "Importer.hpp"      // Whatever class loads sales data


enum class QueryType {
    Exit = 0,
    MakeCountryYearSales = 1,
    MakeYearRevenue = 2,
    MakeRegionYearRevenue = 3,
};

class QueryProcessor
{
public:
    static void Run(const Importer* importer);

    static void ProcessMakeCountryYearQuery(const Importer* importer);
    static void ProcessMakeYearRevenueQuery(const Importer* importer);
    static void ProcessMakeRegionRevenueYearQuery(const Importer* importer);
};