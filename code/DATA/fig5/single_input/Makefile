CC=gcc
CXX=g++

CFLAGS= -std=c++11 -O3 -g -march=native -fopenmp

CXXFLAGS=$(CFLAGS)

LDFLAGS= 
LDLIBS= -lgomp

EXE=RRN

all: $(EXE)

$(EXE): math_helper.o Timer.o

clean:
	rm -f $(EXE) *.o

redo: clean all
