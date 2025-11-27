// EigenRisk_CaseStudy.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "EigenRisk_CaseStudy.hpp"
#include "Importer.hpp"
#include "Validator.hpp"
#include "QueryProcessor.hpp"

using namespace std;

static std::unique_ptr<Importer> createImporter(const std::string& fName, const std::string& fExtension) {
	// Create appropriate importer based on file extension (can be extended for other file types)
	std::unique_ptr<Importer> importerObj;
	if (FileFormatMap.at(fExtension) == FileType::CSV_FILE)
	{
		importerObj = std::make_unique<CSVImporter>(fName);
	}
	else if (FileFormatMap.at(fExtension) == FileType::TXT_FILE)
	{
		// importerObj = std::make_unique<TXTImporter>(fName); - Placeholder for future TXTImporter
	}
	else if (FileFormatMap.at(fExtension) == FileType::LOG_FILE)
	{
		// importerObj = std::make_unique<LOGImporter>(fName); - Placeholder for future LOGImporter
	}
	return std::move(importerObj);
}

int main(int argc, char* argv[])
{
	// Validate input arguments
	std::string fExtension = "";
	try {
		Import::Validator::ValidateInputArguments(argc, argv,&fExtension);
	}
	catch (const std::invalid_argument& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}

	std::string fName(argv[1]);
	std::unique_ptr<Importer> importer = createImporter(fName,fExtension);
	
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
