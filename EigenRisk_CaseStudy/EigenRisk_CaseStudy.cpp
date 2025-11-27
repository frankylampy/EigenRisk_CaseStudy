// EigenRisk_CaseStudy.cpp : Defines the entry point for the application.
//

#include "EigenRisk_CaseStudy.hpp"
//#include <stdexcept>
#include <vector>
using namespace std;




int main(int argc, char* argv[])
{
	// Validate input arguments
	/*std::string fExtension = "";
	try {
		Import::Validator::ValidateInputArguments(argc, argv,&fExtension);
	}
	catch (const std::invalid_argument& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}*/
	std::string fExtension("csv");
	std::string fName("../../../data/world_car_sales_1m.csv");
	std::unique_ptr<Importer> importer;
	// Create appropriate importer based on file extension (can be extended for other file types)
	if (FileFormatMap.at(fExtension) == FileType::CSV_FILE)
	{
		importer = std::make_unique<CSVImporter>(fName);
	}
	else if (FileFormatMap.at(fExtension) == FileType::TXT_FILE)
	{
		// importer = std::make_unique<TXTImporter>(fName); - Placeholder for future TXTImporter
	}
	else if (FileFormatMap.at(fExtension) == FileType::LOG_FILE)
	{
		// importer = std::make_unique<LOGImporter>(fName); - Placeholder for future LOGImporter
	}

	
	try {
		// Read and process the file
		importer->readFile();
		// Show basic data stats
		importer->fetchData();

		// Run Query Processor
		QueryProcessor::Run(importer.get());
	}
	catch (const std::runtime_error& e) {
		cerr << "\tERROR: " << e.what() << endl;
		return 1;
	}


	return 0;
}
