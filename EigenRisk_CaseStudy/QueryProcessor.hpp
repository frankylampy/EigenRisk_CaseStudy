#pragma once
#include <iostream>
#include <string>
#include "Importer.hpp"      // Whatever class loads sales data


enum class QueryType {
    Exit = 0,
    MakeCountryYear = 1,
    MakeRevenueYear = 2,
    MakeRegionYear = 3
};

class QueryProcessor
{
public:
    static void Run(Importer* importer);

    static void ProcessMakeCountryYearQuery(Importer* importer);
    static void ProcessMakeRevenueYearQuery(Importer* importer);
    static void ProcessMakeRegionYearQuery(Importer* importer);
};