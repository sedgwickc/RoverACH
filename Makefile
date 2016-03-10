CC=g++
DEBUG=-g 
FLAGS=-Wall
LINK=

all: RoverACH.o

RoverACH.o: RoverACH.cpp
	$(CC) $(FLAGS) $(DEBUG) -c $^ -o $@
