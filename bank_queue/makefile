CXX = clang++
CXXFLAGS = -g -std=c++11 -Wall -fstandalone-debug
EXEC = test
SOURCE = test.cpp bank_queue.cpp
OBJECT = test.o bank_queue.o

LDFLAG = -lpthread

all:
	$(CXX) $(SOURCE) $(CXXFLAGS) $(LDFLAG) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
