# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
CC = g++
CFLAGS  = -g -Wall -std=c++11
LFLAGS  = -L/usr/lib

# All Targets
all: rest

# Tool invocations
# Executable "assignment1" depends on the files main.o Action.o Restaurant.o Customer.o Dish.o Table.o
rest: bin/main.o bin/Action.o bin/Customer.o bin/Dish.o bin/Restaurant.o bin/Table.o 
	@echo 'Building target: assignment1'
	@echo 'Invoking: C++ Linker'
	$(CC) -o bin/rest bin/main.o bin/Action.o bin/Customer.o bin/Dish.o bin/Restaurant.o bin/Table.o $(LFLAGS)
	@echo 'Finished building target: rest'
	@echo ' '

# Depends on the source and header files
bin/main.o: src/main.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/main.o src/main.cpp
	
	
# Depends on the source and header files
bin/Action.o: src/Action.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Action.o src/Action.cpp

# Depends on the source and header files 
bin/Customer.o: src/Customer.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Customer.o src/Customer.cpp
	
# Depends on the source and header files 
bin/Dish.o: src/Dish.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Dish.o src/Dish.cpp
	
# Depends on the source and header files 
bin/Restaurant.o: src/Restaurant.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Restaurant.o src/Restaurant.cpp
	
# Depends on the source and header files 
bin/Table.o: src/Table.cpp
	$(CC) $(CFLAGS) -c -Iinclude -o bin/Table.o src/Table.cpp

#Clean the build directory
clean: 
	rm -f bin/*