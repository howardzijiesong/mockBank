CXXFLAGS = -Wall -std=c++11 -O2
CXX = g++

default: frontend

frontend: main.o frontend.o transaction.o helperfunctions.o
	$(CXX) $(CXXFLAGS) -o frontend main.o frontend.o transaction.o helperfunctions.o

main.o: main.cc frontend.h

frontend.o: frontend.cc frontend.h transaction.h helperfunctions.h

transaction.o: transaction.cc transaction.h

helperfunctions.o: helperfunctions.cc helperfunctions.h

clean:
	rm main.o frontend.o transaction.o helperfunctions.o frontend
