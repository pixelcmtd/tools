CC ?= cc
CXX ?= c++
CFLAGS ?= -Wall -Wextra -pedantic -O3
CXXFLAGS ?= -Wall -Wextra -pedantic -std=c++2a -O3
PREFIX ?= /usr/local

all:
	$(CC) $(CFLAGS) -o sm src/*.c

cpp:
	$(CXX) $(CXXFLAGS) -o sm src/*.c

movcc:
	movcc $(CFLAGS) -o sm src/*.c

clean:
	rm -f sm

install:
	cp -f sm $(PREFIX)/bin/sm

debug:
	$(CC) $(CFLAGS) -Og -fsanitize=address -fsanitize=leak -fsanitize=undefined -o sm src/*.c

format:
	clang-format -Werror -i --style=file --verbose src/*

.PHONY: all cpp movcc clean install debug format
