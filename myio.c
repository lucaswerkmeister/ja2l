#define _POSIX_C_SOURCE 1

#include <assert.h>
#include <stdio.h>

#include "myio.h"

FILE *fdopenw(int fd) {
  assert(fd != 0);
  switch (fd) {
  case 1: return stdout;
  case 2: return stderr;
  default: return fdopen(fd, "w");
  }
}
