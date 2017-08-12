.PHONY: all check install install-strip uninstall clean buildclean

CFLAGS := -O2
LDFLAGS := -O2

override CFLAGS += -std=c11 -Wall -pedantic

INSTALL := install
INSTALL_PROGRAM := $(INSTALL)
INSTALL_DATA := $(INSTALL) -m 644

prefix := /usr/local
exec_prefix := $(prefix)
bindir := $(exec_prefix)/bin
datarootdir := $(prefix)/share
mandir := $(datarootdir)/man
man1dir := $(mandir)/man1

all: ja2l

ja2l: ja2l.o myio.o options.o pipe.o
ja2l.o: ja2l.c cleanup.h options.h pipe.h
pipe.o: pipe.c cleanup.h
options.o: options.c
myio.o: myio.c

check: all
	./test

install: all
	$(INSTALL_PROGRAM) ja2l $(DESTDIR)$(bindir)/
	-$(INSTALL_DATA) ja2l.1 $(DESTDIR)$(man1dir)/
install-strip:
	$(MAKE) INSTALL_PROGRAM='$(INSTALL_PROGRAM) -s' install
uninstall:
	$(RM) $(DESTDIR)$(bindir)/ja2l $(DESTDIR)$(man1dir)/ja2l.1

clean: buildclean
	$(RM) ja2l
buildclean:
	$(RM) *.o
