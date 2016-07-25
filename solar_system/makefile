CXX = clang++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
EXEC = solarsystem
SOURCE = main.cpp solar_system.cpp
OBJECT = main.o solar_system.o

LDFLAG = -lglut -lGL -lGLU

all:
	$(CXX) $(SOURCE) $(LDFLAG) -o $(EXEC)

clean:
	rm -f $(EXEC) *.gdb *.o
