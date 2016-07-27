CXX = clang++
CXXFLAGS=-g -std=c++11 -Wall -fstandalone-debug -pedantic
EXEC = test
SOURCE = test.cpp solar_system.cpp
OBJECT = test.o solar_system.o

LDFLAG = -lglut -lGL -lGLU

all:
	$(CXX) $(SOURCE) $(CXXFLAGS) $(LDFLAG) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
