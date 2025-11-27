
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Importer.hpp"
#include "CarSale.hpp"
#include <algorithm>

void Importer::countLines() {
    std::ifstream file(m_fileName);
    size_t m_lineCount =  std::count(std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>(), '\n');
    std::cout << "Number of lines: " << m_lineCount << std::endl;
}

Importer::Importer(const std::string& fileName, const std::string& fExt) : 
    m_fileName(fileName), 
    m_fileExtension(fExt.empty() ? "Invalid" : fExt),
	m_lineCount(5000) // Temporary hardcoded value for testing
{
	//countLines();
	m_carSaleData = std::make_unique<CarSaleDataVector>();
	m_modelCountryYearSalesCount = std::make_unique<ModelCountryYearTupleMap>();    
    m_modelCountryCount = std::make_unique<ModelCountryPairMap>();    
    m_modelRegionCount = std::make_unique<ModelRegionPairMap>();    
    m_modelYearRevenue = std::make_unique<ModelYearRevenuePairMap>();
    
	// Reserve space in vectors and maps
    if (m_lineCount > 0) {
        m_carSaleData->reserve(m_lineCount);
        m_modelCountryCount->reserve(m_lineCount);
        m_modelCountryYearSalesCount->reserve(m_lineCount);
        m_modelRegionCount->reserve(m_lineCount);
        m_modelYearRevenue->reserve(m_lineCount);
    }
}

void Importer::openFile()
{
	static_assert(m_carSaleData != nullptr, "CarSaleDataVector is not initialized.");
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
    std::cout << "Total car sales records imported: " << m_carSaleData->size() << std::endl;
    std::sort(m_carSaleData->begin(), m_carSaleData->end(),
        [](const std::unique_ptr<CarSale>& a, const std::unique_ptr<CarSale>& b) {
            return std::chrono::sys_days(a->sale_date) >
                std::chrono::sys_days(b->sale_date);
		});
    //std::cout << "Done Sorting: " << m_carSaleData.size() << std::endl;    
}

Importer::~Importer()
{
    m_carSaleData->clear();
}

void Importer::GenerateDataMaps(const std::unique_ptr<CarSale>& sale) {
    ModelCountryYearTuple modelCountryYearKey = std::make_tuple(
        sale->model, sale->country, sale->vehicle_year);
    incrementCounter(m_modelCountryYearSalesCount.get(), modelCountryYearKey);
    
    ModelYearRevenuePair modelYearRevenueKey = std::make_pair(
        sale->model, sale->vehicle_year);
    incrementCounter(m_modelYearRevenue.get(), modelYearRevenueKey, sale->sale_price_usd);
    
    ModelRegionPair modelRegionKey = std::make_pair(sale->model, sale->region);
    incrementCounter(m_modelRegionCount.get(), modelRegionKey);
    
    ModelCountryPair modelCountryKey = std::make_pair(sale->model, sale->country);
    incrementCounter(m_modelCountryCount.get(), modelCountryKey);

}

