CXX = clang++
CXXFLAGS = -std=c++11 -g -Wall -fstandalone-debug -pedantic
EXEC = test
SOURCE = test.cpp
OBJECT = test.o

LDFLAG = -lcurses

all:
	$(CXX) $(SOURCE) $(CXXFLAGS) $(LDFLAG) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
