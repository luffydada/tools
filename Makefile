.PHONY: all server client clean

ARM ?= no
CROSS_COMPILE := /opt/arm-linux-gnueabihf/gcc-4.9/bin/arm-linux-gnueabihf-
CC := gcc
CXX := g++
CPPFLAGS := -Wall
LDFLAGS := -L/usr/lib/x86_64-linux-gnu
LIBS := -lprotobuf
ifeq ($(ARM), yes)
	CC = $(CROSS_COMPILE)gcc
	CXX = $(CROSS_COMPILE)gcc
	CPPFLAGS += -I/opt/arm-linux-gnueabihf/usr/include
	LDFLAGS = -L/opt/arm-linux-gnueabihf/usr/lib
else
	CPPFLAGS += -I/usr/include
endif
CPPFLAGS += -Iout

all : client server

server : person.pb.o server.o 
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS)

client : person.pb.o client.o
	$(CXX) $^ -o $@ $(LDFLAGS) $(LIBS)

person.pb.o:
	$(CXX) $(CPPFLAGS) -c out/person.pb.cc

clean:
	-rm *.o
