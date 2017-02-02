#include "helperfunctions.h"
#include <ctype.h> //isspace
#include <stdlib.h> //strtoul, exit
#include <algorithm> //find_if_not
#include <limits.h> //UINT_MAX
//In C++11 operators like "and", "not", "or", etc. are keywords.
//However MSVC doesn't support this. Thus we need to include the
//following C header to support these.
#include <iso646.h>

//Reads lines from the istream and strips whitespace from the left and right
//of the string.
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
