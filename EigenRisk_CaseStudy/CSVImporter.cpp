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
    m_carSaleDataVec = std::make_unique<CarSaleVector>();
    m_makeCountryYearSalesCountMap = std::make_unique<StringStringIntTupleIntMap>();
    m_makeCountryCountMap = std::make_unique<StringStringPairIntMap>();
    m_makeRegionCountMap = std::make_unique<StringStringPairIntMap>();
    m_makeYearRevenueMap = std::make_unique<StringIntPairDoubleMap>();
	m_makeRegionYearRevenueMap = std::make_unique<StringStringIntTupleStringDoubleMapPtrMap>();

    m_uniqueYears = std::make_unique<IntSet>();
    m_uniqueCountries = std::make_unique<StringSet>();
    m_uniqueRegions = std::make_unique<StringSet>();
    m_uniqueMakes = std::make_unique<StringSet>();
    m_uniqueMakes = std::make_unique<StringSet>();


    // Reserve space in vectors, sets and maps
    if (m_lineCount > 0) {
        m_carSaleDataVec->reserve(m_lineCount);
        m_makeCountryCountMap->reserve(m_lineCount);
        m_makeCountryYearSalesCountMap->reserve(m_lineCount);
        m_makeRegionCountMap->reserve(m_lineCount);
        m_makeYearRevenueMap->reserve(m_lineCount);
		m_makeRegionYearRevenueMap->reserve(m_lineCount);
        m_uniqueCountries->reserve(m_lineCount);
        m_uniqueRegions->reserve(m_lineCount);
        m_uniqueMakes->reserve(m_lineCount);
        m_uniqueMakes->reserve(m_lineCount);
    }
}

void CSVImporter::countLines() {
    std::ifstream file(m_fileName);
    size_t m_lineCount = std::count(std::istreambuf_iterator<char>(file),
		std::istreambuf_iterator<char>(), '\n') - 1; // Subtract 1 for header line
    std::cout << "Number of lines: " << m_lineCount << std::endl;

    /*std::filesystem::path p{ m_fileName };
    auto fSize = std::filesystem::file_size(p);
    std::cout << "File size: " << fSize << " bytes" << std::endl;
    std::cout << "Approx Number of Lines: " << size_t(fSize / sizeof(CarSale)) << std::endl;*/
}


void CSVImporter::readFile()
{
    if (m_carSaleDataVec == nullptr)
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
        m_carSaleDataVec->push_back(std::move(sale));
        
    }
    
    std::sort(m_carSaleDataVec->begin(), m_carSaleDataVec->end(),
        [](const std::unique_ptr<CarSale>& a, const std::unique_ptr<CarSale>& b) {
            return std::chrono::sys_days(a->sale_date) >
                std::chrono::sys_days(b->sale_date);
		});
}

void CSVImporter::fetchData()
{
	// Implementation can be added as needed
    std::cout << "Total car sales records imported: " << m_carSaleDataVec->size() << std::endl;
    std::cout << "Vehicle Years From " 
        << *m_uniqueYears->begin() << " To " 
		<< *m_uniqueYears->rbegin() << std::endl;
	std::cout << "Make Types:" << m_uniqueMakes->size() << std::endl;
    std::cout << "Make:" << m_uniqueMakes->size() << std::endl;
	std::cout << "Regions:" << m_uniqueRegions->size() << std::endl;
}

void CSVImporter::GenerateDataMaps(const std::unique_ptr<CarSale>& sale) {
    auto saleYear = static_cast<int>(sale->sale_date.year());
    StringStringIntTuple makeCountryYearKey = std::make_tuple(
        sale->manufacturer, sale->country, saleYear);

    incrementCounter(m_makeCountryYearSalesCountMap.get(), makeCountryYearKey);

    StringIntPair makeYearRevenueKey = std::make_pair(
        sale->manufacturer, saleYear);
    incrementCounter(m_makeYearRevenueMap.get(), makeYearRevenueKey, sale->sale_price_usd);

    StringStringPair makeRegionKey = std::make_pair(sale->manufacturer, sale->region);
    incrementCounter(m_makeRegionCountMap.get(), makeRegionKey);

    StringStringPair makeCountryKey = std::make_pair(sale->manufacturer, sale->country);
    incrementCounter(m_makeCountryCountMap.get(), makeCountryKey);

    StringStringIntTuple makeRegionYearKey = std::make_tuple(
		sale->manufacturer, sale->region, saleYear);
    auto it = m_makeRegionYearRevenueMap->find(makeRegionYearKey);
    if (it == m_makeRegionYearRevenueMap->end()) {
		auto countryRevenueMap = std::unique_ptr<StringDoubleMap>(new StringDoubleMap());
		std::string countryKey = sale->country;
        //countryRevenueMap->emplace(sale->country, sale->sale_price_usd);
        incrementCounter(countryRevenueMap.get(), countryKey,sale->sale_price_usd);
		m_makeRegionYearRevenueMap->emplace(makeRegionYearKey, countryRevenueMap.release());
    }
    else
    {
        incrementCounter(it->second, sale->country, sale->sale_price_usd);
    }

	
    

    m_uniqueYears->insert(static_cast<int>(sale->sale_date.year()));
    m_uniqueCountries->insert(sale->country);
    m_uniqueRegions->insert(sale->region);
    m_uniqueMakes->insert(sale->manufacturer);
    //m_uniqueMakes->insert(sale->Make);
}



