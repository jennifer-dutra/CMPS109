#######################################################################
#
# Copyright (C) 2018 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without
# the express written permission of the copyright holder.
#
#######################################################################

CFLAGS = -Wall -Isrc -lpthread -lcrypt -g
CC = g++ -std=c++17

SRC=$(wildcard src/*.cc)

all: crack test

crack: src/main.cc crack.o
	$(CC) -o $@ $^ $(CFLAGS)

test: src/test.cc crack.o
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	@rm crack test

submit: clean
	@tar czvf ~/CMPS109-Lab9.tar.gz \
	    src/*.cc src/*.h
