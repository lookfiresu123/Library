CXX = clang++
CXXFLAGS=-g -std=c++11 -Wall -fstandalone
EXEC = 2048
SOURCE = 2048.cpp
OBJECT = 2048.o

LDFLAG = -lcurses

all:
	$(CXX) $(SOURCE) $(LDFLAG) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
