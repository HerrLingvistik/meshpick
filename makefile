#This is a comment, we save g++ in a variable CC.
CC=g++
#Another comment, a flag giving all them warnings 
CFLAGS=-Wall

version=-std=c++11

all:
	$(CC) $(CFLAGS) $(version) utils/*.cpp main.cpp -lGL -lGLU -lglut -o labling
