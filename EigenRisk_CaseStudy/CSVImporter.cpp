#include "Importer.hpp"
#include "CarSale.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <filesystem>

CSVImporter::CSVImporter(const std::string& fileName) : 
    Importer(fileName, FileType::CSV_FILE)
{
    //countLines();
    m_carSaleData = std::make_unique<CarSaleDataVector>();
    m_makeCountryYearSalesCount = std::make_unique<MakeCountryYearTupleMap>();
    m_makeCountryCount = std::make_unique<MakeCountryPairMap>();
    m_makeRegionCount = std::make_unique<MakeRegionPairMap>();
    m_makeYearRevenue = std::make_unique<MakeYearRevenuePairMap>();

    m_uniqueYears = std::make_unique<std::set<int>>();
    m_uniqueCountries = std::make_unique<std::unordered_set<std::string>>();
    m_uniqueRegions = std::make_unique<std::unordered_set<std::string>>();
    m_uniqueMakes = std::make_unique<std::unordered_set<std::string>>();
    m_uniqueMakes = std::make_unique<std::unordered_set<std::string>>();


    // Reserve space in vectors, sets and maps
    if (m_lineCount > 0) {
        m_carSaleData->reserve(m_lineCount);
        m_makeCountryCount->reserve(m_lineCount);
        m_makeCountryYearSalesCount->reserve(m_lineCount);
        m_makeRegionCount->reserve(m_lineCount);
        m_makeYearRevenue->reserve(m_lineCount);

        m_uniqueCountries->reserve(m_lineCount);
        m_uniqueRegions->reserve(m_lineCount);
        m_uniqueMakes->reserve(m_lineCount);
        m_uniqueMakes->reserve(m_lineCount);
    }
}

void CSVImporter::countLines() {
    std::ifstream file(m_fileName);
    size_t m_lineCount = std::count(std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>(), '\n');
    std::cout << "Number of lines: " << m_lineCount << std::endl;

    /*std::filesystem::path p{ m_fileName };
    auto fSize = std::filesystem::file_size(p);
    std::cout << "File size: " << fSize << " bytes" << std::endl;
    std::cout << "Approx Number of Lines: " << size_t(fSize / sizeof(CarSale)) << std::endl;*/
}


void CSVImporter::readFile()
{
    if (m_carSaleData == nullptr)
    {
        throw std::runtime_error("Car sale data vector is not initialized.");
    }

    if(m_fileName.empty())
    {
        throw std::runtime_error("Unable to find a valid file.");
    }
	std::cout << "Opening file: " << m_fileName << std::endl;
    std::ifstream file(m_fileName);
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open the file: " + m_fileName);
    }

    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line))
    {
        if (lineNumber > m_lineCount)
            break;

        ++lineNumber;
        // Ignore first line (header)
        if (lineNumber == 1)
            continue;

        // Parse each line into a CarSale and store it
		//std::cout << "Parsing line " << lineNumber-1 << std::endl;
        auto sale = parseCarSale(line);
        GenerateDataMaps(sale);
        m_carSaleData->push_back(std::move(sale));
        
    }
    
    std::sort(m_carSaleData->begin(), m_carSaleData->end(),
        [](const std::unique_ptr<CarSale>& a, const std::unique_ptr<CarSale>& b) {
            return std::chrono::sys_days(a->sale_date) >
                std::chrono::sys_days(b->sale_date);
		});
}

void CSVImporter::fetchData()
{
	// Implementation can be added as needed
    std::cout << "Total car sales records imported: " << m_carSaleData->size() << std::endl;
    std::cout << "Vehicle Years From " 
        << *m_uniqueYears->begin() << " To " 
		<< *m_uniqueYears->rbegin() << std::endl;
	std::cout << "Make Types:" << m_uniqueMakes->size() << std::endl;
    std::cout << "Make:" << m_uniqueMakes->size() << std::endl;
	std::cout << "Regions:" << m_uniqueRegions->size() << std::endl;
}

void CSVImporter::GenerateDataMaps(const std::unique_ptr<CarSale>& sale) {
    MakeCountryYearTuple MakeCountryYearKey = std::make_tuple(
        sale->manufacturer, sale->country, static_cast<int>(sale->sale_date.year()));
    incrementCounter(m_makeCountryYearSalesCount.get(), MakeCountryYearKey);

    MakeYearRevenuePair makeYearRevenueKey = std::make_pair(
        sale->manufacturer, sale->vehicle_year);
    incrementCounter(m_makeYearRevenue.get(), makeYearRevenueKey, sale->sale_price_usd);

    MakeRegionPair MakeRegionKey = std::make_pair(sale->manufacturer, sale->region);
    incrementCounter(m_makeRegionCount.get(), MakeRegionKey);

    MakeCountryPair MakeCountryKey = std::make_pair(sale->manufacturer, sale->country);
    incrementCounter(m_makeCountryCount.get(), MakeCountryKey);

    m_uniqueYears->insert(static_cast<int>(sale->sale_date.year()));
    m_uniqueCountries->insert(sale->country);
    m_uniqueRegions->insert(sale->region);
    m_uniqueMakes->insert(sale->manufacturer);
    //m_uniqueMakes->insert(sale->Make);
}



