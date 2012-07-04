CC = gcc
CFLAGS = -fPIC -lcurl -O3 -Wall --ansi --shared

prefix = ${shell arc --show-prefix}

libdir = $(DESTDIR)/$(prefix)/lib/arc/site

all: curl.so

curl.so: curl.c
	$(CC) $(CFLAGS) -o $@ $<

install:
	mkdir -p $(libdir)
	cp curl.so $(libdir)/curl.so
	cp curl.arc $(libdir)/curl.arc

uninstall:
	rm -rf $(libdir)/curl.so
	rm -rf $(libdir)/curl.arc

clean:
	rm -rf curl.so

test:
	prove -fe arc t
