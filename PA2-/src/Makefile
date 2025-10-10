TARGET=server client
SVR_SRCS=serverMain.cpp ServerStub.cpp ExpertQueue.cpp ExpertRequest.cpp
SVR_OBJS=serverMain.o ServerStub.o ExpertQueue.o ExpertRequest.o
SVR_HDRS=ServerStub.h ExpertQueue.h ExpertRequest.h
CLNT_SRCS=clientMain.cpp ClientStub.cpp
CLNT_OBJS=clientMain.o ClientStub.o
CLNT_HDRS=ClientStub.h
CMN_SRCS=Order.h RobotInfo.h
CMN_OBJS=
CMN_HDRS=Order.h RobotInfo.h
CXX=g++
CFLAGS=-Wall -std=c++11
LFLAGS=-pthread
DFLAGS=

all: server client

server: $(SVR_OBJS)
	$(CXX) $(LFLAGS) -o $@ $^

client: $(CLNT_OBJS)
	$(CXX) $(LFLAGS) -o $@ $^

%.o: %.cpp 
	$(CXX) $(CFLAGS) $(DFLAGS) -c $<

clean:
	rm -rf *.o server client

.PHONY: clean
