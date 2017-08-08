.PHONY: all clean buildclean

CFLAGS := -Wall -pedantic -O2
LDFLAGS := -O2

all: ja2l

ja2l: ja2l.o options.o pipe.o
ja2l.o: ja2l.c cleanup.h options.h pipe.h
pipe.o: pipe.c cleanup.h
options.o: options.c

clean: buildclean
	$(RM) ja2l
buildclean:
	$(RM) *.o
