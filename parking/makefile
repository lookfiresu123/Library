CXX = clang++
CXXFLAGS = -std=c++11 -g -Wall -fstandalone-debug -pedantic
EXEC = test
SOURCE = test.cpp parking.cpp
OBJECT = test.o parking.o
LDFLAGS = -lpthread

all:
	$(CXX) $(SOURCE) $(CXXFLAGS) $(LDFLAGS) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
