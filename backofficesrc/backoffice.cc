#include <fstream> //ifstream, ofstream
#include "transaction.h" //Transaction
#include "backoffice.h"

//#include <iostream>

//Creates the back office object from an accounts file at the given path.
BackOffice::BackOffice(char* accountsPath) {
	std::ifstream accountStream(accountsPath);
	accountStream.seekg(0,std::ios::end); //seek file to end
	if (accountStream.tellg() <= 1) {
		return; //File is empty or just one character (usually newline).
	}
	accountStream.seekg(0);
	unsigned accountNumber;
	BalanceNamePair balName;
	while (accountStream) {
//		std::cout << "Reading line\n";
		accountStream >> accountNumber >> balName;
		accounts[accountNumber] = balName;
	}
}

//Processes all transactions in the file specified by transactionPath.
void BackOffice::processTransactions(char* transactionPath) {
	std::ifstream transactionStream(transactionPath);
	Transaction t;
	while (transactionStream >> t) {
		//Delegate to the transcation's process method.
		t.process(accounts);
	}
}

//Write out the master accounts file and valid account file to the paths specified
void BackOffice::writeOut(char* masterAccountsPath, char* validAccountsPath) {
	std::ofstream masterAccountsStream(masterAccountsPath);
	std::ofstream validAccountsStream(validAccountsPath);

	for (auto& pair : accounts) {
		//Recall: pair.first is the account number, pair.second is the
		//BalanceNamePair.
		masterAccountsStream << pair.first << " " << pair.second << "\n";
		validAccountsStream << pair.first << "\n";
	}

	//The valid accounts file is delimited by a fake account with number 0.
	validAccountsStream << "00000000\n";


}
