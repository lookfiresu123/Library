CXX = clang++
CXXFLAGS = -std=c++11 -g -Wall -fstandalone-debug -pedantic
EXEC = test
SOURCE = test.cpp shoot.cpp
OBJECT = test.o shoot.o
LDFLAGS = -lglut -lGL -lGLU

all:
	$(CXX) $(SOURCE) $(CXXFLAGS) $(LDFLAGS) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
