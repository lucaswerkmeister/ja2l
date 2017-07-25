#define _GNU_SOURCE

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "cleanup.h"
#include "pipe.h"

int pipeMaxSize(void) {
  _cleanup_free_ char *lineptr = NULL;
  size_t n;
  _cleanup_fclose_ FILE *pipeMaxSizeFile = NULL;
  ssize_t size;
  static int pipeMaxSize = -1;

  if (pipeMaxSize != -1) {
    return pipeMaxSize;
  }

  pipeMaxSizeFile = fopen("/proc/sys/fs/pipe-max-size", "r");
  if (pipeMaxSizeFile == 0) {
    error(0, errno, "fopen(/proc/sys/fs/pipe-max-size)");
    pipeMaxSize = 0;
    return 0;
  }

  size = getline(&lineptr, &n, pipeMaxSizeFile);
  if (size == -1) {
    if (errno != 0) {
      error(0, errno, "getline(/proc/sys/fs/pipe-max-size)");
    } else {
      // file exists but is empty... weird but not a problem
    }
    pipeMaxSize = 0;
    return 0;
  }

  pipeMaxSize = atoi(lineptr);
  return pipeMaxSize;
}

void growPipe(int fd) {
  int ret;

  ret = fcntl(fd, F_SETPIPE_SZ, pipeMaxSize());
  if (ret == -1 && errno != EBADF) {
    error(0, errno, "fcntl(F_SETPIPE_SZ)");
  }
}
