# Makefile for a C++ project with main.cpp

CXX = g++              # C++ compiler
CXXFLAGS = -Wall -I./problems -I./solvers -I/home/goose/UofT/CSC494/eigen-3.4.0 -std=c++14 

SRCDIR = .
PROBLEM_DIR = ./problems

SOURCES = $(SRCDIR)/main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

PROGRAM = chungus

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(PROGRAM)