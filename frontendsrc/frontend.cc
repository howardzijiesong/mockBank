#include "frontend.h"
#include "helperfunctions.h" //readLineAndStrip
#include <iostream> //cout, cin
#include <string> //strings
#include <fstream> //ifstream
#include <iterator> //istream_iterator
#include <stdlib.h> //strtoul
#include <algorithm> //any_of
//In C++11 operators like "and", "not", "or", etc. are keywords.
//However MSVC doesn't support this. Thus we need to include the
//following C header to support these.
#include <iso646.h>

std::unordered_set<std::string> FrontEnd::commands ({"login", "logout", "create",
                                                      "delete", "deposit", "withdraw",
                                                      "withdraw", "transfer"});

FrontEnd::FrontEnd(char* validAccountsPath,char* transactionSummaryPath) :
	               transactionSummaryPath(transactionSummaryPath) {
	//Create an stream for reading the file.
	std::ifstream listFile(validAccountsPath);

	//Read the file of numbers into the unordered_set
	validAccounts.insert(std::istream_iterator<unsigned>(listFile),
	                     std::istream_iterator<unsigned>());

	//The last element of validAccounts is 00000000. This just delimits the end, so we
	//don't need it in our list of accounts.
	validAccounts.erase(0);
/* debug code
	for (auto i : validAccounts) {
		std::cerr << i;
	}
*/
}


void FrontEnd::run() {
	while (true) {
//		std::getline(std::cin, input);
		std::string input = readLineAndStrip();
		if (input == "login") {
			if (loginState != out) {
				std::cout << "You are already logged in.\n";
			} else {
				login();
			}
		} else if (loginState == out) {

			if (commands.count(input)) {
				std::cout << "Please login first.\n";
			} else {
				std::cout << "Input not recognised.\n";
			}
		//Only run other commands if logged in.
		} else if (input == "logout") {
			logout();
		} else if (input == "create") {
			createAcc();
		} else if (input == "delete") {
			deleteAcc();
		} else if (input == "deposit") {
			deposit();
		} else if (input == "withdraw") {
			withdraw();
		} else if (input == "transfer") {
			transfer();
		} else {
			std::cout << "Input not recognised.\n";
		}
	}
}

void FrontEnd::login() {
	std::cout << "atm or agent?\n";
	while (loginState == out) {
		std::string input = readLineAndStrip(std::cin);
		if (input == "atm") {
			loginState = atm;
			std::cout << "Logged in as atm.\n";
		} else if (input == "agent") {
			loginState = agent;
			std::cout << "Logged in as agent.\n";
		} else {
			if (commands.count(input)) {
				//count returns 1 if input is in the set of commands,
				//0 otherwise
				std::cout << "Please choose atm or agent.\n";
			} else { //Not a command
				std::cout << "Input not recognised.\n";
			}
		}
	}
}

void FrontEnd::logout() {
	//We know that logout is only call if you're logged in.
	loginState = out;
	std::cout << "Logged out.\n";

	std::ofstream outFile(transactionSummaryPath);
	for (auto t : transactions) {
		outFile << t << "\n";
	}

	//Default constructor creates ES (end of session) transaction.
	outFile << Transaction() << "\n";

	//Reset the total withdrawl for the session.
	totalWithdrawl = 0;
}

void FrontEnd::createAcc() {
	if (loginState == atm) {
		std::cout << "create requires agent privledges.\n";
		return;
	}
	//in agent mode

	std::cout << "Enter the account number to create:\n";

	unsigned account;
	do {
		std::string input = readLineAndStrip();
		if (input[0] == '0') {
			std::cout << "Account number may not begin with 0.\n";
			continue;
		}
		char* endptr;
		account = strtoul(input.c_str(),&endptr,10);
		if (*endptr) { //We didn't parse the whole string
			std::cout << "Account number must only contain numbers 0-9.\n";
			continue;
		}
		//Now account is a number, but any valid number.
		if (validAccounts.count(account)) {
			std::cout << "Account " << account << " already exists.\n";
		} else if (account < 10000000 or account > 99999999) {
			std::cout << "Account number must be exactly 8 digits.\n";
		} else {
			break; //We found a valid account number, stop looping.
		}
	} while (true);

	//Now account number is valid

	std::cout << "Enter the name of the account to create:\n";

	std::string name;
	do {
		name = readLineAndStrip();
		auto s = name.size();
		if (s < 3 or s > 30) {
			std::cout << "Account name must be between 3-30 characters.\n";

		//if any of the characters are invalid...
		} else if (any_of(name.begin(), name.end(), isInvalidForNames)) {
			std::cout << "Account name must only contain alphanumeric characters "
			             "and spaces.\n";
		} else {
			break; //Name is valid
		}
	} while (true);

	std::cout << "Account " << account << " created with name " << name << ".\n";

	transactions.emplace_back(cr,account,name);

	//Transactions are prevented by not adding the new account to
	//validAccounts()

}

void FrontEnd::deleteAcc() {
	if (loginState == atm) {
		std::cout << "delete requires agent privledges.\n";
		return;
	}
	//in agent mode

	std::cout << "Enter the account number to delete:\n";

	unsigned account;
	do {
		std::string input = readLineAndStrip();
		if (input[0] == '0') {
			std::cout << "Account number may not begin with 0.\n";
			continue;
		}
		char* endptr;
		account = strtoul(input.c_str(),&endptr,10);
		if (*endptr) { //We didn't parse the whole string
			std::cout << "Account number must only contain numbers 0-9.\n";
			continue;
		}
		//Now account is a number, but any valid number.
		if (account < 10000000 or account > 99999999) {
			std::cout << "Account number must be exactly 8 digits.\n";
		} else if (not validAccounts.count(account)) {
			std::cout << "Account " << account << " does not exist. "
			             "Please enter a valid account number.\n";
		} else {
			break; //We found a valid account number, stop looping.
		}
	} while (true);

	//Now account number is valid

	std::cout << "Enter the name of the account to delete:\n";

	std::string name;
	do {
		name = readLineAndStrip();
		auto s = name.size();
		if (s < 3 or s > 30) {
			std::cout << "Account name must be between 3-30 characters.\n";

		//if any of the characters are invalid...
		} else if (any_of(name.begin(), name.end(), isInvalidForNames)) {
			std::cout << "Account name must only contain alphanumeric characters "
			             "and spaces.\n";
		} else {
			break; //Name is valid
		}
	} while (true);

	std::cout << "Account " << account << " deleted with name " << name << ".\n";

	transactions.emplace_back(dl,account,name);

	//Prevent further transactions involving this account.
	validAccounts.erase(account);
}

void FrontEnd::deposit() {
	//We already know that we're logged in.
	std::cout << "Enter the account number to deposit to:\n";

	unsigned account = readAccountNumber();

	std::cout << "Enter the amount to deposit:\n";

	unsigned amount = readAmount();

	std::cout << "$" << amount/100 << "." << std::setw(2) << std::setfill('0')
	          << amount % 100 << " deposited to account " << std::setw(8)
	          << std::setfill('0') << account << ".\n";

	transactions.emplace_back(de,account,amount);
}

void FrontEnd::withdraw() {
	//We already know that we're logged in.
	std::cout << "Enter the account number to withdraw from:\n";

	unsigned account = readAccountNumber();

	std::cout << "Enter the amount to withdraw:\n";

	unsigned amount = readAmount();

	if (loginState==atm and amount + totalWithdrawl > 100000) {
		std::cout << "Cannot withdraw more than $1000 in one session.\n";
		return; //Do not complete withdrawl or prompt again.
	}

	std::cout << "$" << amount/100 << "." << std::setw(2) << std::setfill('0')
	          << amount % 100 << " withdrawn from account " << std::setw(8)
	          << std::setfill('0') << account << ".\n";

	transactions.emplace_back(wd,account,amount);
	totalWithdrawl += amount;
}

void FrontEnd::transfer() {
	std::cout << "Enter the account number to transfer from:\n";

	unsigned account1 = readAccountNumber();
	std::cout << "Enter the account number to transfer to:\n";

	unsigned account2 = readAccountNumber();

	std::cout << "Enter the amount to transfer:\n";

	unsigned amount = readAmount();

	std::cout << "$" << amount/100 << "." << std::setw(2) << std::setfill('0')
	          << amount % 100 << " transferred from account " << std::setw(8)
	          << std::setfill('0') << account1 << " to account " << std::setw(8)
	          << std::setfill('0') << account2 << ".\n";

	transactions.emplace_back(tr,account1,account2,amount);
	totalWithdrawl += amount;
}

unsigned FrontEnd::readAccountNumber(std::istream& in, std::ostream& out) {
	unsigned account;
	do {
		account = readAndCheck(
		                   "Account number must be 8 digits, not beginning with a zero.",
		                   99999999,10000000);
		//10000000-99999999 is the range of 8 digit numbers

        //if the account is not in the set of valid accounts
		if (not validAccounts.count(account)) {
			std::cout << "Account " << std::setw(2) << std::setfill('0') << account
			          << " does not exist. Please enter a valid account number:\n";
		}

	} while (not validAccounts.count(account));
	return account;
}

unsigned FrontEnd::readAmount(std::istream& in, std::ostream& out) {
    //if we are in ATM mode then our maximum withdrawal is $99999999, otherwise $100000
	unsigned max = loginState == atm ? 100000 : 99999999;
	const char* errorMessage = loginState == atm ?
	                           "Please enter an amount of at most $1000.00." :
	                           "Please enter an amount of at most $999999.99.";

    //check that the input does not exceed the maximum we established
	return readAndCheck(errorMessage, max);

}
