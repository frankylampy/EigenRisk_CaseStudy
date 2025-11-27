#include "Validator.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>

static std::string getExtension(const std::string& fName) {
	size_t pos = fName.find_last_of('.');
	if (pos == std::string::npos) {
		return "";
	}
	return fName.substr(pos + 1);
}

bool Import::Validator::ValidateInputArguments(const int argc, char* argv[],std::string* ext)
{
	if (argc != 2) {
		throw std::invalid_argument("Insufficient arguments provided. Please provide an input file.");
	}
	else if (!ValidateFileType(argv[1]))
	{
		throw std::invalid_argument("Invalid file type provided. Allowed types are: .txt, .csv, .log");
	}
	else if (!ValidateFileExists(argv[1])) {
		throw std::invalid_argument("The specified file does not exist: " + std::string(argv[1]));
	}
	else if (ext) {		
		*ext = getExtension(argv[1]);
		std::cout << "\n\tFile extension detected: " << *ext << std::endl;
	}
	return true;
}
		

bool Import::Validator::ValidateFileType(const std::string& fName)
{
	// get file extension abd validate it
	std::string extension = getExtension(fName);
	if (extension.empty())
		return false;

	std::vector<std::string> allowedExtensions = { "txt", "csv","log" };

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
