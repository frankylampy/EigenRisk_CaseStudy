#include "Validator.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>

static void printHelp() {
	std::cout <<
		"\tUsage: EigenRisk_CaseStudy.exe [OPTIONS]\n"
		"\t\n"
		"\tOptions:\n"
		"\t  $PathToCSVFile	Input CSV File\n"
		"\t  --help, -h		Show this help message\n"
		"\t  -d, --debug, DEBUG=1   (optional) Debug mode\n"
		"\t\n"
		"\tExamples:\n"
		"\t  EigenRisk_CaseStudy.exe file.csv\n"
		"\t  EigenRisk_CaseStudy.exe file.csv --debug \n"
		"\t  EigenRisk_CaseStudy.exe file.csv DEBUG=1 \n"
		"\t  EigenRisk_CaseStudy.exe -h\n"
		<< std::endl;
}

static std::string getExtension(const std::string& fName) {
	size_t pos = fName.find_last_of('.');
	if (pos == std::string::npos) {
		return "";
	}
	return fName.substr(pos + 1);
}

bool Import::Validator::ValidateInputArguments(const int argc, char* argv[],std::string* ext)
{
	if (argc < 2) {
		throw std::invalid_argument("Insufficient arguments provided. Please provide an input file.");
	}

	if (argc > 1) {
		std::string helpArg1(argv[1]);
		if (helpArg1 == "--help" || helpArg1 == "-h") {
			printHelp();
			exit(0);
		}
	}

	bool debug = false;
	if (argc > 2)
	{
		// DEBUG mode
		std::string arg(argv[2]);
		debug = (arg == "DEBUG=1" || arg == "--debug" || arg == "-d");
	}

	if (!ValidateFileType(argv[1]))
	{
		throw std::invalid_argument("Invalid file type provided. Allowed types are: .csv");
	}
	else if (!ValidateFileExists(argv[1])) {
		throw std::invalid_argument("The specified file does not exist: " + std::string(argv[1]));
	}
	else if (ext) {		
		*ext = getExtension(argv[1]);
		if (debug)
			std::cout << "\n\tFile extension detected: " << *ext << std::endl;
	}
	
	return debug;
}
		

bool Import::Validator::ValidateFileType(const std::string& fName)
{
	// get file extension abd validate it
	std::string extension = getExtension(fName);
	if (extension.empty())
		return false;

	std::vector<std::string> allowedExtensions = {"csv"};

	return std::find(allowedExtensions.begin(),
		allowedExtensions.end(),
		extension) != allowedExtensions.end();
}

bool Import::Validator::ValidateFileExists(const std::string& fName)
{
	//std::ifstream f(fName.c_str());
	struct stat buffer;
	return (stat(fName.c_str(), &buffer) == 0);
}
