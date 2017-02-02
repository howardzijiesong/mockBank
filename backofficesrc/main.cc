#include "backoffice.h"

//This program takes in 4 command line arguments.
int main(int argc, char** argv) {
	//argv[1] is path to the master_accounts_file.
	BackOffice backOffice(argv[1]); //Initialize the back office.
	//argv[2] is path to the merged_transaction_file.
	backOffice.processTransactions(argv[2]); //Process the transactions.
	//argv[3] is the path to the new_master_file.
	//argv[4] is the path to the valid_accounts file.
	backOffice.writeOut(argv[3],argv[4]); //Write out the new files.
	return 0;
}
