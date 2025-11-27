#pragma once
#ifndef IMPORTER_HPP
#define IMPORTER_HPP
#endif // !IMPORTER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <unordered_set>
#include <set>
#include "HashUtils.hpp"
#include "CarSale.hpp"

typedef std::vector<std::unique_ptr<CarSale>> CarSaleDataVector;
typedef std::tuple<std::string, std::string, int> MakeCountryYearTuple;  // Make, Country, Year
typedef std::pair<std::string, int> MakeYearRevenuePair; // Make, Yearly Revenue
typedef std::pair<std::string, std::string> MakeRegionPair; // Make, Region
typedef std::pair<std::string, std::string> MakeCountryPair; // Make, Country
typedef std::unordered_map<MakeCountryYearTuple, int, TupleHash<std::string, std::string, int>> MakeCountryYearTupleMap;
typedef std::unordered_map<MakeYearRevenuePair, double, PairHash<std::string, double>> MakeYearRevenuePairMap;
typedef std::unordered_map<MakeRegionPair, int, PairHash<std::string, std::string>> MakeRegionPairMap;
typedef std::unordered_map<MakeCountryPair, int, PairHash<std::string, std::string>> MakeCountryPairMap;

typedef enum {
	LOG_FILE,
	CSV_FILE,
	TXT_FILE
} FileType;

static const std::unordered_map<std::string, FileType> FileFormatMap = {
	{"log", LOG_FILE},
	{"csv", CSV_FILE},
	{"txt", TXT_FILE}
};

class Importer
{
public:
	Importer(const std::string& fileName, const FileType& fType) : 
		m_fileName(fileName), m_fileType(fType) {
		m_lineCount = 50000;
	}
	Importer(const Importer&) = delete;
	Importer& operator=(const Importer&) = delete;
	Importer() = delete;
	virtual ~Importer() = default;
	virtual void readFile() = 0;
	virtual void fetchData() = 0;

	size_t ProcessMakeCountryYearQuery(const std::string& make, const std::string& country, int year) const;
	long ProcessMakeYearRevenueQuery(const std::string& make, int year) const;

private:
	void ValidateMake(const std::string& make) const;
	void ValidateCountry(const std::string& country) const;
	void ValidateYear(const int& year) const;
	void ValidateRegion(const std::string& region) const;
	void ValidateModel(const std::string& Make) const;

protected:
	std::string m_fileName;
	FileType m_fileType;
	size_t m_lineCount = 0;
	std::unique_ptr <CarSaleDataVector> m_carSaleData;
	std::unique_ptr <MakeCountryYearTupleMap> m_makeCountryYearSalesCount;
	std::unique_ptr<MakeYearRevenuePairMap>	m_makeYearRevenue;
	std::unique_ptr<MakeRegionPairMap> m_makeRegionCount;
	std::unique_ptr<MakeCountryPairMap> m_makeCountryCount;

	std::unique_ptr<std::set<int>> m_uniqueYears;
	std::unique_ptr<std::unordered_set<std::string>> m_uniqueCountries;
	std::unique_ptr<std::unordered_set<std::string>> m_uniqueRegions;
	std::unique_ptr<std::unordered_set<std::string>> m_uniqueMakes;	
	std::unique_ptr<std::unordered_set<std::string>> m_uniqueModels;

	virtual void countLines() = 0;
	virtual void GenerateDataMaps(const std::unique_ptr<CarSale>& sale) = 0;
};

class CSVImporter : public Importer
{
public: 
	CSVImporter(const std::string& fileName);
	~CSVImporter() override = default;
	virtual void readFile() override;
	virtual void fetchData() override;
protected:
	virtual void countLines() override;
	virtual void GenerateDataMaps(const std::unique_ptr<CarSale>& sale) override;

};

