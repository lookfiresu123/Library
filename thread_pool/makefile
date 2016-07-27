CXX = clang++
CXXFLAGS = -g -std=c++11 -Wall -fstandalone-debug
EXEC = test
SOURCE = test.cpp thread_pool.cpp
OBJECT = test.o thread_pool.o

LDFLAG = -lpthread

all:
	$(CXX) $(SOURCE) $(CXXFLAGS) $(LDFLAG) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
