CC ?= cc
CFLAGS ?= -Wall -Wextra -pedantic -O3
PREFIX ?= /usr/local

all:
	$(CC) $(CFLAGS) -o sm src/*.c

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

.PHONY: all movcc clean install debug format
