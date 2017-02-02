#include <string> //strings
#include <unordered_set> //sets
#include <vector> //vectors
#include "transaction.h" //transaction logging class


class FrontEnd {
	private:
	enum LoginState {out, atm, agent};

	LoginState loginState = out;
	//validAccounts is an unordered set, since all we're using it for is lookups,
	//and an unordered_set is the fastest data structure for that.
	std::unordered_set<unsigned> validAccounts;
	std::vector<Transaction> transactions;
	
	//Set of commands, i.e. login, logout, etc.
	static std::unordered_set<std::string> commands;
	
	//Where to write the transaction summary file to.
	const char* transactionSummaryPath;
	
	//The total amount that's been withdrawn.
	unsigned totalWithdrawl = 0;
	
	//Helper functions:
	//Reads an account number
	unsigned readAccountNumber(std::istream& in=std::cin, std::ostream& out=std::cout);
	//Reads the amount (for deposit, withdraw, transfer)
	unsigned readAmount(std::istream& in=std::cin, std::ostream& out=std::cout);
	
	public:
	FrontEnd(char* validAccountsPath, char* transactionSummaryPath);
	void run();
	void login();
	void logout();
	void createAcc(); //This is called createAcc to be uniform with deleteAcc
	void deleteAcc(); //Can't call this delete since delete is a keyword
	void deposit();
	void withdraw();
	void transfer();
};



