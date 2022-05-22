CC ?= cc
PREFIX ?= /usr/local

all: netsh

netsh: netsh.c
	$(CC) $(CFLAGS) -o netsh netsh.c

install: netsh
	cp -f netsh $(PREFIX)/bin/netsh

.PHONY: all install
