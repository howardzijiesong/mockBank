#include <iostream>
#include <string>
#include "frontend.h"

//Should be called program_name valid_accounts_file transaction_summary_files
int main(int c, char** argv) {
	FrontEnd frontEnd(argv[1],argv[2]);
	frontEnd.run();
	return 0; //Should never get here
}


