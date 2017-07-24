#define _GNU_SOURCE

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "cleanup.h"
#include "options.h"
#include "pipe.h"

int main(int argc, char *argv[]) {
  _cleanup_free_ char *line = NULL;
  size_t len = 0;
  ssize_t read;

  handleOptions(argc, argv);
  if (exitAfterOptions != -1) {
    return exitAfterOptions;
  }

  growPipe(STDIN_FILENO);
  growPipe(STDOUT_FILENO);

  while ((read = getline(&line, &len, input)) != -1) {
    line[read-2] = 0;
    printf("%s\n", line);
  }

  return 0;
}
