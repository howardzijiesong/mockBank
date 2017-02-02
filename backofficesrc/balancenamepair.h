#ifndef BALANCENAMEPAIR_H //Include guards
#define BALANCENAMEPAIR_H

#include <string> //string
#include <istream> //istream
#include <ostream> //ostream
#include <iomanip> //setw, setfill
#include "helperfunctions.h" //stripWhitespace

struct BalanceNamePair {
	public:
	unsigned balance;
	std::string name;
	
	friend std::istream& operator>>(std::istream& stream, BalanceNamePair& p) {
		stream >> p.balance;
		std::getline(stream,p.name);
		stripWhitespace(p.name); //getline puts leading whitespace in the name
		return stream;
	}
	
	friend std::ostream& operator<<(std::ostream& stream, const BalanceNamePair& p) {
		stream << std::setw(3) << std::setfill('0') << p.balance << " " << p.name;
		return stream;
	}
	
	BalanceNamePair() = default; //Use default 0-arg constructor
	BalanceNamePair(unsigned balance, const std::string& name) noexcept
	: balance(balance), name(name) {}

};
#endif
