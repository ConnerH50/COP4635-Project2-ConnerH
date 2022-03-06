CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -pthread #makefile for threads 

all: TCPServer TCPClient

OBJECTS1 = TCPServer.o

TCPServer:  $(OBJECTS1)
	$(CXX) $(CXXFLAGS) -o $@ $^
	
OBJECTS2 = TCPClient.o

TCPClient:  $(OBJECTS2)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	$(RM) *.o main TCPServer TCPClient
	
.PHONY: all


