# #!/usr/bin/make -f

# #-------------------------------------------------------------------------------
# # 1. Setting up necessary variables for the build process
# #-------------------------------------------------------------------------------

# # The compilers that we will be using (we will only use g++)
# CC := gcc
# CXX := g++

# # $(wildcard *.h) finds all file names with patterns (random string + ".h")
# HDRS := $(wildcard *.h)		
# SRCS := $(wildcard *.cpp)	
# OBJS := $(SRCS:.cpp=.o)		# replaces .cpp extension to .o (e.g., main.cpp -> main.o)
# 				# and stores the names to OBJS

# # $(wildcard Server*.h) finds all file names with patterns ("Server" + random string + ".h")
# SVR_HDRS := $(wildcard Server*.h)	
# SVR_SRCS := $(wildcard Server*.cpp)
# SVR_OBJS := $(SVR_SRCS:.cpp=.o)

# # $(wildcard Client*.h) finds all file names with patterns ("Client" + random string + ".h")
# CLNT_HDRS := $(wildcard Client*.h)
# CLNT_SRCS := $(wildcard Client*.cpp)
# CLNT_OBJS := $(CLNT_SRCS:.cpp=.o)

# # $(filter-out X Y Z, A) removes X Y and Z from A if X Y Z are found in A
# CMN_HDRS := $(filter-out $(SVR_HDRS) $(CLNT_HDRS), $(HDRS))
# CMN_SRCS := $(filter-out $(SVR_SRCS) $(CLNT_SRCS), $(SRCS))
# CMN_OBJS := $(CMN_SRCS:.cpp=.o)


# # -Wall prints: all warnings
# # -std=c++11: use of C++11
# CFLAGS := -Wall -std=c++11 

# # -pthread: use of posix threads (necessary to use std::thread or pthreads)
# LFLAGS := -pthread 

# # we are building two target binaries: server and client
# TARGET := server client

# #-------------------------------------------------------------------------------
# # 2. What to build and how to build them
# #-------------------------------------------------------------------------------

# # The build rules are defined as follows:
# # target : prerequisite files
# #	How to build the target
# # 1. Make program will first check whether the target exists in the folder.
# # 2. If not it will check for prerequisite files.
# # 3. If the prerequisite files all exist in the folder the program will execute
# #    the next line to build the target.
# # 4. If any of the prerequisite file does not exist, the program will look for 
# #    rules to create the prerequisite file: it will look for rules where the
# #    prerequisite file is the target.


# # "all:" is the starting point of the build process unless specified otherwise.
# # 1. The make program will look for targets (i.e., server and client) to build
# # 2. If the target is not found in the folder it will look for
# #    rules to build the target (it will look for "server: xxx" and "client: xxx"
# #    which should define the rules to build the server and the client)

# all: $(TARGET)

# # "debug:" allows the g++ compiler to embed debugging information that can be
# # used for gdb debugger. To use this mode type "make debug." This command will 
# # set the necessary compilation flag. Other build sequence is the same as
# # "all" above that is triggered by the default "make" command.
# debug: DFLAGS := -ggdb -DDEBUG
# debug: $(TARGET)

# # "server:" defines rules to build the server.
# # 1. To build the server binary we need compiled object files for the server.
# #    We defined them above and the necessary object files are:
# #    - $(SVR_OBJS) that includes all server specific object files and
# #    - $(CMN_OBJS) that include all common object files that are used for both
# #      server and client programs.
# # 2. The next line $(CXX) $(LFALGS) -o $@ $^ defines how to create server
# #    binary. This line translates to 
# #
# #      g++ -pthread -o server ServerXXX1.o ServerXXX2.o ... Common1.o ...
# #
# #    $@ is a macro for the string behind the colon (i.e., server).
# #    $^ is a macro for all the string after the colon (i.e., $(SVR_OBJS) $(CMN_OBJS))
# #
# # 3. When you start compiling, however, you will only have .cpp and .h files
# #    in the folder the make program won't be able to execute the g++ command and
# #    it will look for another rules to create the object (.o) files

# server: $(SVR_OBJS) $(CMN_OBJS)
# 	$(CXX) $(LFLAGS) -o $@ $^ 

# # This rule defines how to build the server specific object files.
# # To build object files, corresponding source code files (i.e., cpp and h files) are
# # necessary. 
# # Since we will have all the files the build command will be executed
# #   g++ -Wall -std=c++0x -c ServerXXX1.cpp ServerXXX2.cpp ...

# $(SVR_OBJS): $(SVR_SRCS) $(SVR_HDRS)
# 	$(CXX) $(CFLAGS) $(DFLAGS) -c $(SVR_SRCS)


# # Same applies to the client program.
# client: $(CLNT_OBJS) $(CMN_OBJS)
# 	$(CXX) $(LFLAGS) -o $@ $^ 

# $(CLNT_OBJS): $(CLNT_SRCS) $(CLNT_HDRS)
# 	$(CXX) $(CFLAGS) $(DFLAGS) -c $(CLNT_SRCS)
	

# # This rule compiles the common source code into object files.
# $(CMN_OBJS): $(CMN_SRCS) $(CMN_HDRS)
# 	$(CXX) $(CFLAGS) $(DFLAGS) -c $(CMN_SRCS)

# # This defines how you will clean up the compiled files.
# # You can type "make clean" in the command line to delete all compiled files
# # which include .o files and the compiled binary.

# clean:
# 	rm -rf *.o $(TARGET)

# # ".PHONY:" indicates "clean" and "debug" are not target files to build but 
# # rather special commands.

# .PHONY: clean debug



CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pthread -O2
INCLUDES = -I.

# Source files
CLIENT_SRC = customer_client.cpp
SERVER_SRC = factory_server.cpp

# Header dependencies
HEADERS = Common.h SocketComm.h ClientStub.h ServerStub.h ExpertQueue.h

# Targets
all: client server

client: $(CLIENT_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o client $(CLIENT_SRC)

server: $(SERVER_SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o server $(SERVER_SRC)

# Clean build artifacts
clean:
	rm -f client server
	rm -rf experiment_results

# Clean everything including results
cleanall: clean
	rm -rf experiment_results

# Quick test with 4 customers, 100 orders each
test: client server
	@echo "Starting test server..."
	./server 12345 2 &
	@sleep 2
	@echo "Running test client..."
	./client 127.0.0.1 12345 4 100 0
	@echo "Stopping server..."
	@pkill -f "./server" || true
	@sleep 1

# Run single experiment (Example: make run_exp CUSTOMERS=16 ORDERS=1000 TYPE=0 EXPERTS=0)
CUSTOMERS ?= 16
ORDERS ?= 1000
TYPE ?= 0
EXPERTS ?= 0
PORT ?= 12345

run_exp: client server
	@echo "Starting server with $(EXPERTS) experts..."
	./server $(PORT) $(EXPERTS) &
	@sleep 2
	@echo "Running client: $(CUSTOMERS) customers, $(ORDERS) orders, type=$(TYPE)"
	./client 127.0.0.1 $(PORT) $(CUSTOMERS) $(ORDERS) $(TYPE)
	@echo "Stopping server..."
	@pkill -f "./server" || true

# Run all experiments (requires bash script)
experiments: client server
	@chmod +x run_experiments.sh
	@./run_experiments.sh

# Plot results (requires Python with matplotlib and pandas)
plot:
	@python3 plot_results.py

# Full workflow: build, run experiments, plot
full: all experiments plot

.PHONY: all clean cleanall test run_exp experiments plot full