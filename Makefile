.PHONY: all check clean buildclean

CFLAGS := -O2
LDFLAGS := -O2

override CFLAGS += -Wall -pedantic

all: ja2l

ja2l: ja2l.o myio.o options.o pipe.o
ja2l.o: ja2l.c cleanup.h options.h pipe.h
pipe.o: pipe.c cleanup.h
options.o: options.c
myio.o: myio.c

check: all
	./test

clean: buildclean
	$(RM) ja2l
buildclean:
	$(RM) *.o
