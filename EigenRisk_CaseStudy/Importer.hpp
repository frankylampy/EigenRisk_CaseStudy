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

// Set Typdefs
typedef std::set<int> IntSet;
typedef std::unordered_set<std::string> StringSet;

// Pair/Tuple Typdefs
typedef std::tuple<std::string, std::string, int> StringStringIntTuple;  
typedef std::pair<std::string, int> StringIntPair; 
typedef std::pair<std::string, double> StringDoublePair;
typedef std::pair<std::string, std::string> StringStringPair; 

// Map Typdefs
typedef std::unordered_map<std::string, double> StringDoubleMap;
typedef std::unordered_map<StringStringIntTuple, int, TupleHash<std::string, std::string, int>> StringStringIntTupleIntMap;
typedef std::unordered_map<StringIntPair, double, PairHash<std::string, double>> StringIntPairDoubleMap;
typedef std::unordered_map<StringStringPair, int, PairHash<std::string, std::string>> StringStringPairIntMap;
typedef std::unordered_map <StringStringIntTuple, std::unordered_map<std::string, double>*, TupleHash<std::string, std::string, int>> StringStringIntTupleStringDoubleMapPtrMap;

// Vector Typdefs
typedef std::vector<std::unique_ptr<CarSale>> CarSaleVector;
typedef std::vector<StringDoublePair> StringDoublePairVector;

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

	int64_t ProcessMakeCountryYearQuery(const std::string& make, const std::string& country, int year) const; // Returns Make-Country-Year sales count
	long ProcessMakeYearRevenueQuery(const std::string& make, int year) const; // Returns revenue
	int64_t ProcessMakeRegionRevenueYearQuery(const std::string& make, const std::string& region, int year, 
		StringDoublePairVector& vec) const; // Returns Make-Region-Year sales count by Country

private:
	void ValidateMake(const std::string& make) const; // Validates if the make exists in the dataset
	void ValidateCountry(const std::string& country) const; // Validates if the country exists in the dataset
	void ValidateYear(const int& year) const; // Validates if the year exists in the dataset
	void ValidateRegion(const std::string& region) const;	// Validates if the region exists in the dataset
	void ValidateModel(const std::string& Make) const; // Validates if the model exists in the dataset

protected:
	std::string m_fileName;
	FileType m_fileType;
	size_t m_lineCount = 0;
	std::unique_ptr<CarSaleVector> m_carSaleDataVec; // Vector all CarSale data

	std::unique_ptr<StringStringIntTupleIntMap> m_makeCountryYearSalesCountMap; // Make, Country, Year -> Sales Count
	std::unique_ptr<StringIntPairDoubleMap>	m_makeYearRevenueMap; 		 // Make, Year -> Revenue
	std::unique_ptr<StringStringPairIntMap> m_makeRegionCountMap;		 // Make, Region -> Sales Count
	std::unique_ptr<StringStringPairIntMap> m_makeCountryCountMap;		 // Make, Country -> Sales Count
	std::unique_ptr <StringDoubleMap> m_countryRevenueMap; // Country -> Revenue
	std::unique_ptr<StringStringIntTupleStringDoubleMapPtrMap> m_makeRegionYearRevenueMap; // Make, Region, Year -> (Conutries -> Revenue Map*)
	
	std::unique_ptr<IntSet> m_uniqueYears;	// Unique years in data
	std::unique_ptr<StringSet> m_uniqueCountries; // Unique years in data
	std::unique_ptr<StringSet> m_uniqueRegions; // Unique regions in data
	std::unique_ptr<StringSet> m_uniqueMakes;	// Unique makes in data
	std::unique_ptr<StringSet> m_uniqueModels; // Unique models in data

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

