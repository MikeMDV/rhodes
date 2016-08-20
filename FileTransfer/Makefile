#
#             Author: Michael Marven
#       Date Created: 03/05/16
# Last Date Modified: 03/05/16
#            Purpose: Makefile for CS372 Project 2 ftserve program
#
#


CC = g++
DEBUG = -g
TARGET = ftserve
CFLAGS = -Wall -std=c++0x


all: $(TARGET)

$(TARGET) : ftserve.cpp
	$(CC) $(CFLAGS) -o $(TARGET) ftserve.cpp $(LIBS)

clean:
	rm -rf *.o $(TARGET)