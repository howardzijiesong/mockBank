#include <algorithm> //find, distance
#include <iterator> //begin, end
#include <stdlib.h> //exit
#include "balancenamepair.h" //BalanceNamePair
#include "transaction.h"

const char* const Transaction::codeNames[] = {"CR","DL","DE","WD","TR","ES"};

//Convert transaction string into the corrosponding code.
TransactionCode Transaction::stringToCode(std::string str) {
	//iter = iterator to the location of str in the array.
	auto iter = std::find(std::begin(codeNames), std::end(codeNames), str);

	//Return the index of the element in the array.
	//This is the same as the value of the code in it's enumeration.
	return (TransactionCode)std::distance(std::begin(codeNames), iter);
}

//Calls an overloaded method of the same name.
void Transaction::checkAccount(const BackOffice::AccountMap& map) const {
	checkAccount(map,account1);
}

//Checks that an account exists in the master_accounts_file.
void Transaction::checkAccount(const BackOffice::AccountMap& map, unsigned value) const {
	if (not map.count(value)) {
		std::cerr << "Error: Account " << value << " does not exist.\n";//2
		exit(accountDoesNotExist);
	}
}

//Check that the balance does not exceed maximum.
void Transaction::checkOverflow(unsigned balance) const noexcept {
	if (balance + amount > 99999999) {
		std::cerr << "Error: Adding " << amount << " causes account "
		          << account1 << " to exceed the maximum balance of $999999.99.\n";
		exit(amountOutOfRange);
	}
}

//Check that the balance does not exceed maximum.
void Transaction::checkUnderflow(unsigned balance) const noexcept {
	if (balance < amount) {
		std::cerr << "Error: Removing " << amount << " is not possible.\n"//4
		          << "Account " << account1 << " only has balance " << balance << ".\n";//4
		exit(amountOutOfRange);
	}
}

void Transaction::createAcc(BackOffice::AccountMap& map) const {
	if (map.count(account1)) {//1
		std::cerr << "Error: Account " << account1 << " cannot be created, since "
		"it already exists.\n";//2
		exit(accountAlreadyExists);//3
	}
	//map[account1] = BalanceNamePair(amount,name);
	map[account1] = {amount,name};//4
}

void Transaction::deleteAcc(BackOffice::AccountMap& map) const {
	checkAccount(map);
	auto& nameInDatabase = map[account1].name;
	if (name!=nameInDatabase) {
		std::cerr << "Error: Account " << account1 << " with name "
				  << name << " deleted with name " << nameInDatabase
				  << ".\n";
		exit(incorrectName);
	}
	map.erase(account1);
}
void Transaction::withdraw(BackOffice::AccountMap& map, unsigned account) const {
	checkAccount(map);  //1
	auto& balance = map[account].balance;//3
	checkUnderflow(balance);//3
	balance -= amount;//5
}

void Transaction::deposit(BackOffice::AccountMap& map, unsigned account) const {
	checkAccount(map);
	auto& balance = map[account].balance;
	checkOverflow(balance);
	balance += amount;
}

void Transaction::process(BackOffice::AccountMap& map) const {
	switch (code) {
		case cr: { //create using statement coverage testing
			createAcc(map);
			break;//5
		}
		case dl: { //delete
			deleteAcc(map);
			break;
		}
		case de: { //deposit
			deposit(map, account1);
			break;
		}
		case wd: { //withdraw
			withdraw(map, account1); //use basic block testing
			break;
		}
		case tr: { //transfer
			withdraw(map, account1);
			deposit(map, account2);
			break;
		}
		default:
		; //Do nothing for es transactions.
	}
}
