#include <string> //strings
#include <iostream> //cin, istream, ostream

//Read a line from a stream and then remove whitespace
//from both ends.
std::string readLineAndStrip(std::istream& stream=std::cin);

//Remove whitespace from both ends of a string.
//i.e. "  Room " -> "Room"
void stripWhitespace(std::string&);

//Reads lines from the istream and prints trys to parse them as an unsigned.
//If the parsing fails, then errorMessage is printed to the ostream,
//and another line is read.
//Note that a newline is printed after the error message.
unsigned readAndCheck(const char* errorMessage, std::istream& in=std::cin,
                      std::ostream& out=std::cout);
//This overload of readAndCheck does the above but rejects amounts greater than max.
unsigned readAndCheck(const char* errorMessage, unsigned max,
                      std::istream& in=std::cin, std::ostream& out=std::cout);
//This overload of readAndCheck does the above but rejects amounts less than min.
unsigned readAndCheck(const char* errorMessage, unsigned max, unsigned min,
                      std::istream& in=std::cin, std::ostream& out=std::cout);

//Returns true if c cannot be part of an account name.
bool isInvalidForNames(char c);
