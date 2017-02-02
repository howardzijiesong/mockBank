import subprocess
import difflib
import os
import sys

#Usage:
#First argument - path to the Front End executable.
#Second argument - path to the test directory.
#This contains the following:
#	input/	- Directory for input files
#	output/ - Directory for output files
#	valid_accounts_list.txt - accounts list to use for testing.
#	A test consists of a file in the input directory, and a corresponding file in the output directory.
#	These files have the same name, and are of the form r<requirement number>t<test number>.txt
#Third argument - path to where the report file will go. This is a .csv file which states which
#tests failed and succeeded.
#Additionaly, .html diff files are placed in the current directory.

import re
#Regex for the file names, extracts the requirement and test number.
fileRegex = re.compile(r'r([0-9]+)t([0-9]+).txt') 

#Key function for sorting the filenames (so r10t1.txt is after r9t1.txt for example)
def fileToKey(fileName):
	match = fileRegex.match(fileName)
	return 2**32*int(match.group(1)) + int(match.group(2)) #r2t3 maps to 2*(2**32) + 3

#If we don't have the right number of arguments print a usage message.
if(len(sys.argv) is not 4):
	
	print("Wrong number of arguments\nusage: test.py <programName> <testsDirectory> <reportFileName>")
	
else:

	numPassed = 0

	#Get the command line arguments.
	programName = sys.argv[1]
	
	testsDirectory = sys.argv[2]
	
	inputDirectory = testsDirectory + "/input/"
	
	outputDirectory = testsDirectory + "/output/"
	
	#Get a list of the files in the test input directory, only keeping those of the rxty.txt form,
	#where x and y are integers (i.e. r1t2.txt).
	testFiles = [fileName for fileName in os.listdir(inputDirectory) if fileRegex.match(fileName)]
	
	reportFileName = sys.argv[3]
	
	#Open our report file.
	reportFile = open(reportFileName, 'w')
	
	#Iterate through all the tests.
	for inputFileName in sorted(testFiles,key=fileToKey): 
		#Iterate through the test files in sorted order, according to fileToKey.
		
		#Clear previous test's transaction summary file.
		os.remove("transaction_summary.txt") #Delete the file.
		open("transaction_summary.txt","w").close() #Create new empty file.
		
		inputFile = open(inputDirectory + inputFileName, 'r')

		#Run the program with the test input and store the standard output, accounting for "\r\n" line endings (for portability). 
		programOutput = subprocess.check_output([programName, testsDirectory + "/valid_accounts_list.txt", "transaction_summary.txt"], stdin = inputFile).decode().replace("\r\n", "\n")

		#Open our expected output file.
		outputFile = open(outputDirectory + inputFileName, 'r')
		
		#Read the expected output file into a string.
		expectedOutput = outputFile.read()
		
		#Add a trailing newline to expected output if it doesn't have one (i.e. due to it being created on Windows).
		if expectedOutput and expectedOutput[-1]!="\n":
			expectedOutput += "\n" 
		
		
		#We need to check if we're testing the standard output, or the transaction summary file.
		#A given test tests the transaction summary file if the first line of the expected output file is
		#"Transaction Summary File". If so, the rest of the file is the expected transaction summary file.

		#Split file a list of two strings: [first_line, rest_of_the_file].
		tsFileContents = expectedOutput.split('\n', 1) 
		if(tsFileContents[0] == "Transaction Summary File"):
			expectedOutput = tsFileContents[1]
			tsFile = open("transaction_summary.txt", 'r')
			programOutput = tsFile.read()
			tsFile.close()
		
		#Compare the the actual program output to what we expected.
		if(programOutput == expectedOutput):
		
			#Report that the test was passed.
			reportFile.write(inputFileName + ", Passed\n")
			
			numPassed += 1
		
		else:
			#Report that the test was failed.
			reportFile.write(inputFileName + ", Failed\n")
			
			#Open the HTML diff file so we can record the differences.
			differenceFile = open("diff_" + inputFileName + ".html", 'w')
			
			#Use difflib to generate HTML files for us.
			if(outputFile is not None):
				differenceFile.write(difflib.HtmlDiff().make_file(programOutput.splitlines(True), expectedOutput.splitlines(True), "actual", "expected"))
			
			differenceFile.close()
		
		#Close the input and expected output files.
		outputFile.close()
		inputFile.close()
	
	#Close the report file.
	reportFile.close()
	
	#Print a message saying what percentage of tests passed.
	print("Tests Passed: " + "{0:.2f}".format(numPassed * 100.0 / len(testFiles)) + "%")


