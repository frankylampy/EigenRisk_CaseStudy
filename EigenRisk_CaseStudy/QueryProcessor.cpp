#include "QueryProcessor.hpp"
#include "Importer.hpp"

static void fetchMakeCountryYear(std::string& make,int& year, std::string* country = nullptr) {
	std::string input;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::getline(std::cin, input);
	std::stringstream ss(input);
	std::string entry;
	int index = 0;
	while (std::getline(ss, entry, ',')) {
		switch (index) {
		case 0: make = entry;
			break;
		case 1: 
			if (country)
			{
				*country = entry;
				break;
			}
			
		case 2: 
			try {
				year = std::stoi(entry);
				break;
			}
			catch(...) {
				// Ignore errors				
				}
		default:
			//Ignore additional inputs if any
			break;
		}
		++index;
	}
}

static QueryType chooseQuery() {

	std::cout << "\nChoose a query to execute:\n";
	std::cout << "\t0: Exit\n";
	std::cout << "\t1: Manufacturer-Country-Year Sales\n";
	std::cout << "\t2: Manufacturer-Year Revenue\n";
	std::cout << "\t3: Manufacturer-Region-Year Revenue\n";

	int choice = -1;
	int InvalidInputCount = 1;
	while (true)
	{
		std::cout << "\nEnter your choice (0-3): ";

		if (std::cin >> choice && choice >= 0 && choice <= 3)
		{
			return static_cast<QueryType>(choice);
		}
		else
		{
			if (InvalidInputCount < 3) {
				std::cout << "Invalid input " << 3 - InvalidInputCount << " attempts remaining\n";
				std::cin.clear();                // clear error state
				std::cin.ignore(10000, '\n');    // flush invalid input
				InvalidInputCount++;
			}
			else {
				std::cout << "\nToo many invalid attempts. Exiting.\n";
				return QueryType::Exit;
			}
		}
	}
}

void QueryProcessor::Run(const Importer* importer)
{
    QueryType query = chooseQuery();

    while (query != QueryType::Exit)
    {
        switch (query)
        {
        case QueryType::MakeCountryYearSales:
            ProcessMakeCountryYearQuery(importer);
            break;

		case QueryType::MakeYearRevenue:
            ProcessMakeYearRevenueQuery(importer);
            break;

		case QueryType::MakeRegionYearRevenue:
            ProcessMakeRegionRevenueYearQuery(importer);
            break;

        default:
            std::cout << "\nInvalid query type selected.\n";
            break;
        }
		std::cout <<"\n-------- QUERY COMPLETE --------\n";
        query = chooseQuery();    // continue until Exit
    }

    std::cout << "Exiting...\n";
}

void QueryProcessor::ProcessMakeCountryYearQuery(const Importer* importer)
{
	std::string make;
	std::string country;
	int year(0);
	std::cout << "\nEnter Manufacturer,Country,Year to fetch sales data (e.g. Audi,China,2025): ";
	fetchMakeCountryYear(make,year,&country);

	int64_t count = importer->ProcessMakeCountryYearQuery(make,country,year);
	if (count < 0) {
		//Query did not return valid data
		std::cout << "\n NO DATA FOUND \n";
		return;
	}

	printf("\nNumber of cars sold by %s in %s in the year %d: %lld\n", make.c_str(), country.c_str(), year, count);
}

void QueryProcessor::ProcessMakeYearRevenueQuery(const Importer* importer)
{
	std::string make;
	int year(0);
	std::cout << "\nEnter Manufacturer,Year to fetch Revenue data (e.g. Audi,2025): ";
	fetchMakeCountryYear(make, year);

	long count = importer->ProcessMakeYearRevenueQuery(make,year);
	if (count < 0) {
		//Query did not return valid data
		std::cout << "\n NO DATA FOUND \n";
		return;
	}
	printf("\nRevenue (in USD) by %s in the year %d: $%ld\n",make.c_str(), year, count);
	//std::cout << "Number of cars sold by " <<
}

void QueryProcessor::ProcessMakeRegionRevenueYearQuery(const Importer* importer)
{
	std::string make;
	std::string region;
	int year(0);
	std::cout << "\nEnter Manufacturer,Region,Year to Revenue sales data sorted by countries (e.g. Audi,Asia,2025): ";
	fetchMakeCountryYear(make, year, &region);
	StringDoublePairVector regionRevenueData;
	int64_t count =  importer->ProcessMakeRegionRevenueYearQuery(make, region, year, regionRevenueData);
	if (count < 0) {
		//Query did not return valid data
		std::cout << "\n NO DATA FOUND \n";
		return;
	}
	printf("\nRevenue of cars sold by %s in region %s in the year %d is listed below:\n", make.c_str(), region.c_str(), year);
	printStringDoublePairVector(regionRevenueData);
}
