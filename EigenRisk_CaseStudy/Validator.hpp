// Validator.hpp : Utility functions for validating input files.
# pragma once
#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP
#endif // !VALIDATOR_HPP
#include <string>

namespace Import {
	namespace Validator {
		// Validates the file type based on its extension.
		bool ValidateInputArguments(const int argc, char* argv[], std::string* ext = nullptr);
		bool ValidateFileType(const std::string & fName);
		bool ValidateFileExists(const std::string& fName);
	}
}
