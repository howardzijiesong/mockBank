#include <iostream> //ostream
#include <iomanip> //setw, setfill


enum TransactionCode {cr, dl, de, wd, tr, es};

struct Transaction {
	private:
	
	static const char* codeNames[];

	TransactionCode code=es;
	unsigned account1=0;
	unsigned account2=0;
	unsigned amount=0;
	std::string name;
	
	public:
	//Constructor
	//Defailt constructor creates empty ES transaction
	Transaction() {}

	//This overload is for transfer.
	Transaction(TransactionCode code, unsigned account1, unsigned account2,
	            unsigned amount) :
	            code(code), account1(account1), account2(account2), amount(amount)
	            {}
	//This overload is for withdraw and deposit.
	Transaction(TransactionCode code, unsigned account1, unsigned amount) :
	            code(code), account1(account1), amount(amount) {}

	//This overload is for create and delete.
	Transaction(TransactionCode code, unsigned account1, std::string name) :
	            code(code), account1(account1), name(name) {}

	//Stream output operator overload, for writing the transactions to the file.
	friend std::ostream& operator<<(std::ostream& stream, Transaction t) {
		stream << Transaction::codeNames[t.code] << " ";
		stream << std::setw(8) << std::setfill('0'); //Fill left side with 0s to make 8 digits
		stream << t.account1 << " ";
		stream << std::setw(8) << std::setfill('0'); //Need to do this for each number
		stream << t.account2 << " ";
		stream << std::setw(3) << std::setfill('0'); //Fill up to 3 zeroes to get $0 to be 000
		stream << t.amount << " ";
		if (t.name == "") {
			//The empty string is used to represent the lack of a name.
			stream << "***";
		} else {
			stream << t.name;
		}
		return stream;
	}
};
