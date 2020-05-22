VPATH =../ ./
CPP = g++
CXXFLAGS = -std=c++11 -O2 -g 
CLIENTOBJS = ossSocket.o client.o
SERVEROBJS = ossSocket.o server.o
LIBS = -lpthread
INCLUDE = -I../../include/ -I../../third_party/ossSocket/

all: server client

client : $(CLIENTOBJS)
	$(CPP) $(CXXFLAGS) -o $@ $(CLIENTOBJS) $(LIBS) $(INCLUDE)

server : $(SERVEROBJS) 
	$(CPP) $(CXXFLAGS) -o $@ $(SERVEROBJS) $(LIBS) $(INCLUDE)

server.o : server.cpp
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS) $(INCLUDE)

ossSocket.o : ../../ossSocket.cc
	$(CPP) $(CXXFLAGS) -c $^ $(LIBS) $(INCLUDE)

PHONY: clean

clean:
	rm client server *.out *.o .*.gch .*.h.gch