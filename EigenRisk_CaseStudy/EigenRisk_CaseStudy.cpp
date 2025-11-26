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
	std::string fExtension(".csv");
	std::string fName("../../../data/world_car_sales_1m.csv");
	std::unique_ptr<Importer> importer = std::make_unique<Importer>(fName, fExtension);
	//std::unique_ptr<Importer> importer = std::make_unique<Importer>(argv[1], fExtension);
	try {
		importer->openFile();
	}
	catch (const std::runtime_error& e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}
	//cout << "Hello CMake." << endl;
	return 0;
}
