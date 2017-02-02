CXXFLAGS = -Wall -std=c++11 -O2
CXX = g++

default: backoffice

backoffice: main.o backoffice.o transaction.o helperfunctions.o balancenamepair.o
	$(CXX) $(CXXFLAGS) -o backoffice main.o backoffice.o transaction.o helperfunctions.o balancenamepair.o

main.o: main.cc backoffice.h

backoffice.o: backoffice.cc backoffice.h transaction.h helperfunctions.h balancenamepair.h

transaction.o: transaction.cc transaction.h backoffice.h amount.h

helperfunctions.o: helperfunctions.cc helperfunctions.h

balancenamepair.o: balancenamepair.cc balancenamepair.h helperfunctions.h

clean:
	rm main.o backoffice.o transaction.o helperfunctions.o balancenamepair.o backoffice
