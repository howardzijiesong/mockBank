#include <ostream> //ostream
#include <istream> //istream
#include <iomanip> //setw, setfill

//Class for an amount of money.
//A simple wrapper around unsigned integer.
struct Amount {
	public:
	//Amount in cents
	unsigned amount;
	
	//Convert unsigned to amount
	Amount(unsigned amount) : amount(amount) {}
	Amount() = default;
	
	operator unsigned() const { //Convert to unsigned
		return amount;
	}

	//Write out the amount to a stream in a formatted way.
	friend std::ostream& operator<<(std::ostream& stream, Amount a) {
		stream << "$" << a.amount/100 << "." << std::setw(2) << std::setfill('0')
		       << a.amount % 100;
		return stream;
	}

	friend std::istream& operator>>(std::istream& stream, Amount& a) {
		stream >> a.amount;
		return stream;
	}

};

