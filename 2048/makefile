CXX = clang++
CXXFLAGS=-g -std=c++11 -Wall -fstandalone-debug
EXEC = test
SOURCE = 2048.cpp
OBJECT = 2048.o

LDFLAG = -lcurses

all:
	$(CXX) $(SOURCE) $(CXXFLAGS) $(LDFLAG) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
