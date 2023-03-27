CC ?= cc
PREFIX ?= /usr/local

all: netsh

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

install: netsh
	cp -f netsh $(PREFIX)/bin/netsh

clean:
	rm -f netsh

.PHONY: all install clean
