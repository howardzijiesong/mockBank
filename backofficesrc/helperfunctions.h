#ifndef HELPERFUNCTIONS_H //Include guard
#define HELPERFUNCTIONS_H 1

#include <string> //strings
#include <iostream> //cin, istream, ostream

//Read a line from a stream and then remove whitespace
//from both ends.
std::string readLineAndStrip(std::istream& stream=std::cin);

//Remove whitespace from both ends of a string.
//i.e. "  Room " -> "Room"
void stripWhitespace(std::string&);

#endif
