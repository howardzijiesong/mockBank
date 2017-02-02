#ifndef BACKOFFICE_H //Include guard
#define BACKOFFICE_H 1

#include <map> //unordered_map
#include <string> //string
#include "balancenamepair.h" //BalanceNamePair

class BackOffice {
	public:
	//Typedefs have to go first so we can declare variables using them.
	typedef std::map<unsigned,BalanceNamePair> AccountMap;

	private:
	//Professor Cordy said this was a good application of a hash table.
	AccountMap accounts;

	public:
	//Constructor
	BackOffice(char* accountsPath);


	void processTransactions(char* transactionPath);

	void writeOut(char* masterAccountsPath, char* validAccountsPath);
};

#endif
