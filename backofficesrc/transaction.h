#include <iostream> //ostream
#include <iomanip> //setw, setfill
#include <string> //string
#include "backoffice.h" //AccountMap
#include "amount.h" //Amount
#include "helperfunctions.h" //readLineAndStrip

enum TransactionCode {cr, dl, de, wd, tr, es};

struct Transaction {
	private:
	//Member variables
	TransactionCode code;
	unsigned account1;
	unsigned account2;
	Amount amount;
	std::string name;

	//Error codes (for exiting on an error)
	enum errorCode {accountDoesNotExist=1, accountAlreadyExists, amountOutOfRange,
	                 incorrectName};

	//An array of the actual strings for each transaction code.
	static const char* const codeNames[];

	//Convert a transaction code string ("DE") into a transaction code (de)
	static TransactionCode stringToCode(std::string);

	//Exits if account1 is not in the map.
	void checkAccount(const BackOffice::AccountMap& map) const;
	//Exits if num is not an account number in the map.
	void checkAccount(const BackOffice::AccountMap& map, unsigned num) const;
	//Exits if either account1 or account2 are not in the map.
	void checkAccounts(const BackOffice::AccountMap& map) const;

	//Exits if the balance + amount exceeds the maximum.
	void checkOverflow(unsigned balance) const noexcept;
	//Exits if the balance < amount.
	void checkUnderflow(unsigned balance) const noexcept;
	//create
	void createAcc(BackOffice::AccountMap& map) const;
	//delete
	void deleteAcc(BackOffice::AccountMap& map) const;
	//withdraw
	void withdraw(BackOffice::AccountMap& map, unsigned account) const;

	//deposit
	void deposit(BackOffice::AccountMap& map, unsigned account) const;

	public:
	//Just use default constructors.
	Transaction() = default;

	//Process a given transaction, writing back the changes to the AccountMap.
	void process(BackOffice::AccountMap& map) const;

	//Read the transaction from a stream (i.e. the transaction summary file stream)
	friend std::istream& operator>>(std::istream& stream, Transaction& t) {
		std::string codeString;
		if (stream >> codeString) { //False if we hit end of file
			t.code = stringToCode(codeString);

			stream >> t.account1 >> t.account2 >> t.amount;
			t.name = readLineAndStrip(stream);
		}
		return stream;
	}
};
