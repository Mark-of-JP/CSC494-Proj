# Makefile for a C++ project with main.cpp

CXX = g++              # C++ compiler
CXXFLAGS = -std=c++11  # C++11 standard
SRC = main.cpp         # Source file
OUT = my_program       # Output executable name

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)

.PHONY: all clean