# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -std=c++11 -Iinclude
LFLAGS  = -lboost_system -pthread -lboost_filesystem -lboost_thread

# All Targets
all: BGSclient

# Tool invocations
# Executable "BGSclient" depends on the files echoClient.o readFromServer.o readFromKeyboard.o connectionHandler.o 
BGSclient: bin/echoClient.o bin/readFromKeyboard.o bin/readFromServer.o bin/connectionHandler.o
	@echo 'Building target: BGSclient'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/BGSclient bin/echoClient.o bin/connectionHandler.o bin/readFromKeyboard.o bin/readFromServer.o $(LFLAGS)
	@echo 'Finished building target: BGSclient'
	@echo ' '

# Depends on the source and header files
bin/echoClient.o: src/echoClient.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/echoClient.o src/echoClient.cpp
	
	
# Depends on the source and header files
bin/connectionHandler.o: src/connectionHandler.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/connectionHandler.o src/connectionHandler.cpp

# Depends on the source and header files 
bin/readFromKeyboard.o: src/readFromKeyboard.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/readFromKeyboard.o src/readFromKeyboard.cpp
	
# Depends on the source and header files 
bin/readFromServer.o: src/readFromServer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/readFromServer.o src/readFromServer.cpp
	


#Clean the build directory
clean: 
	rm -f bin/*
