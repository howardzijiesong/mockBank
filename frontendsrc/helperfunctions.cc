#include "helperfunctions.h"
#include <ctype.h> //isspace
#include <stdlib.h> //strtoul, exit
#include <algorithm> //find_if_not
#include <limits.h> //UINT_MAX
//In C++11 operators like "and", "not", "or", etc. are keywords.
//However MSVC doesn't support this. Thus we need to include the
//following C header to support these.
#include <iso646.h>


std::string readLineAndStrip(std::istream& stream) {
	std::string input;
	do {
		std::getline(stream, input);
		if (not stream) {
			exit(0);
		}
		stripWhitespace(input);
	} while (not input.size());
	//Read lines until you get one that's not blank or just whitespace.
	return input;
}

void stripWhitespace(std::string& str) {

	//firstNonWhitespace = iterator to first non-whitespace character in str.
	auto firstNonWhitespace = find_if_not(str.begin(),str.end(),isspace);

	str.erase(str.begin(),firstNonWhitespace); //Erase leading whitespace

	//Same as above, but to the last non whitespace
	auto lastNonWhitespace = find_if_not(str.rbegin(),str.rend(),isspace);
	//Need to call .base() to convert fron a reverse iterator to a regular one.
	str.erase(lastNonWhitespace.base(),str.end());

}

unsigned readAndCheck(const char* errorMessage, std::istream& in,
                      std::ostream& out) {
	return readAndCheck(errorMessage, UINT_MAX, 0,
	                    in, out);
}
unsigned readAndCheck(const char* errorMessage, unsigned max, std::istream& in,
                      std::ostream& out) {
	return readAndCheck(errorMessage, max, 0,
	                    in, out);
}

unsigned readAndCheck(const char* errorMessage, unsigned max, unsigned min,
                      std::istream& in, std::ostream& out) {
	unsigned result;
	std::string input;
	char* endptr;
	//So strtoul takes in a char* to parse into an unsigned int.
	//The second argument is a char** where it writes the pointer
	//to the end of the parsing.
	//If *endptr == '\0' then we've parsed the whole string.
	do {
		input = readLineAndStrip(in);
		result = strtoul(input.c_str(),&endptr,10);
		if (*endptr or result > max or result < min) {
			out << errorMessage << "\n";
		} else {
			return result;
		}
	} while (true);
}

//Returns true if the character is not a valid character to put in a name.
//That is, isValidForNames(c) is false when c is alphanumeric or a space.
bool isInvalidForNames(char c) {
	//isalnum(c) is true if c is alphanumeric
	return not (isalnum(c) or c == ' ');
}

