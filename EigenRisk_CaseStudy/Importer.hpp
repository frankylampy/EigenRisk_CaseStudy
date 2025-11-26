#pragma once
#ifndef IMPORTER_HPP
#define IMPORTER_HPP
#endif // !IMPORTER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>
#include "HashUtils.hpp"

struct CarSale; // Forward declaration
//#include "CarSale.hpp"
typedef std::vector<std::unique_ptr<CarSale>> CarSaleDataVector;
typedef std::tuple<std::string, std::string, int> ModelCountryYearTuple;  // Model, Country, Year
typedef std::pair<std::string, int> ModelYearRevenuePair; // Model, Yearly Revenue
typedef std::pair<std::string, std::string> ModelRegionPair; // Model, Region
typedef std::pair<std::string, std::string> ModelCountryPair; // Model, Country

class Importer
{
public:
	Importer(const std::string& fileName, const std::string& fExt);
	~Importer();

	
	
	void openFile();

private:
	std::string m_fileName;
	std::string m_fileExtension;
	int64_t m_lineCount = 0;
	CarSaleDataVector m_carSaleData;
	
	// Private method to count lines in the file
	void countLines();
	void GenerateDataMaps(const std::unique_ptr<CarSale>& sale);
	
	std::unordered_map<ModelCountryYearTuple, int, TupleHash<std::string, std::string, int>>
		m_modelCountryYearSalesCount;

	std::unordered_map<ModelYearRevenuePair, double, PairHash<std::string, double>>
		m_modelYearRevenue;

	std::unordered_map<ModelRegionPair, int, PairHash<std::string, std::string>>
		m_modelRegionCount;

	std::unordered_map<ModelCountryPair, int, PairHash<std::string, std::string>>
		m_modelCountryCount;
};

