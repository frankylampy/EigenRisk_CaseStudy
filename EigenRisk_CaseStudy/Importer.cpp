#include "Importer.hpp"

int64_t Importer::ProcessMakeCountryYearQuery(const std::string& make, const std::string& country, int year) const {
	//Validate input query parameters
	ValidateMake(make);
	ValidateCountry(country);
	ValidateYear(year);

	// Lookup in the map
	StringStringIntTuple key = std::make_tuple(make, country, year);
	auto it = m_makeCountryYearSalesCountMap->find(key);
	if (it != m_makeCountryYearSalesCountMap->end()) {
		return it->second;
	}
	return -1; // Indicates no data found
}

long Importer::ProcessMakeYearRevenueQuery(const std::string& make, int year) const
{
	//Validate input query parameters
	ValidateMake(make);
	ValidateYear(year);

	// Lookup in the map
	StringIntPair key = std::make_pair(make, year);
	auto it = m_makeYearRevenueMap->find(key);
	if (it != m_makeYearRevenueMap->end()) {
		return static_cast<long>(it->second);
	}
	return -1;
}

int64_t Importer::ProcessMakeRegionYearRevenueQuery(
	const std::string& make, 
	const std::string& region, 
	int year, 
	StringDoublePairVector& vec) const
{
	//Validate input query parameters
	ValidateMake(make);
	ValidateRegion(region);
	ValidateYear(year);

	// Lookup in the map
	StringStringIntTuple key = std::make_tuple(make, region, year);
	auto it = m_makeRegionYearRevenueMap->find(key);
	if (it != m_makeRegionYearRevenueMap->end()) {
		vec = sortMapByValue(*(it->second));
		return 0;
	}
	return -1;
}

int64_t Importer::ProcessMakeRegionYearSalesQuery(
	const std::string& make, 
	const std::string& region, 
	int year, 
	StringIntPairVector& vec) const
{
	//Validate input query parameters
	ValidateMake(make);
	ValidateRegion(region);
	ValidateYear(year);

	// Lookup in the map
	StringStringIntTuple key = std::make_tuple(make, region, year);
	auto it = m_makeRegionYearCountMap->find(key);
	if (it != m_makeRegionYearCountMap->end()) {
		vec = sortMapByValue(*(it->second));
		return 0;
	}
	return -1;
}

void Importer::ValidateModel(const std::string& make) const {
	if (m_uniqueModels && !m_uniqueModels->empty()) {
		auto it = m_uniqueModels->find(make);
		if (it != m_uniqueModels->end()) {
			return;
		}
	}
	throw std::runtime_error("Invalid Manufacturer."); // Country not found
}
void Importer::ValidateCountry(const std::string& country) const {
	if (m_uniqueCountries && !m_uniqueCountries->empty()) {
		auto it = m_uniqueCountries->find(country);
		if (it != m_uniqueCountries->end()) {
			return;
		}
	}
	throw std::runtime_error("Invalid Country."); // Country not found
}
void Importer::ValidateYear(const int& year) const {
	if (m_uniqueYears && !m_uniqueYears->empty()) {
		auto it = m_uniqueYears->find(year);
		if (it != m_uniqueYears->end()) {
			return;
		}
	}
	throw std::runtime_error("Invalid Year."); // Country not found
}

void Importer::ValidateMake(const std::string& Make) const
{
	if (m_uniqueMakes && !m_uniqueMakes->empty()) {
		auto it = m_uniqueMakes->find(Make);
		if (it != m_uniqueMakes->end()) {
			return;
		}
	}
	throw std::runtime_error("Invalid Manufacturer."); // Country not found
}

void Importer::ValidateRegion(const std::string& region) const
{
	if (m_uniqueRegions && !m_uniqueRegions->empty()) {
		auto it = m_uniqueRegions->find(region);
		if (it != m_uniqueRegions->end()) {
			return;
		}
	}
	throw std::runtime_error("Invalid Region."); // Country not found
}

Importer::~Importer()
{
	if (m_makeRegionYearRevenueMap) {
		for (auto& entry : *m_makeRegionYearRevenueMap)
		{
			auto* innerMapPtr = entry.second;
			if (innerMapPtr != nullptr)
			{
				delete innerMapPtr;   // free the heap allocation
			}
		}
		m_makeRegionYearRevenueMap->clear(); // remove all keys from the parent map
	}

	if (m_makeRegionYearCountMap) {
		for (auto& entry : *m_makeRegionYearCountMap)
		{
			auto* innerMapPtr = entry.second;
			if (innerMapPtr != nullptr)
			{
				delete innerMapPtr;   // free the heap allocation
			}
		}
		m_makeRegionYearCountMap->clear(); // remove all keys from the parent map
	}
}
