#define _GNU_SOURCE

#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  // read and discard first line
  read = getline(&line, &len, input);
  if (read == -1) {
    error(0, 0, "malformed input: empty");
    return 1;
  }
  if (strncmp("[\n", line, read+1) != 0) {
    error(0, 0, "malformed input: first line is not a single left bracket (‘[’)");
    return 1;
  }

  // copy array element lines
  while ((read = getline(&line, &len, input)) != -1) {
    if (read < 2) {
      error(0, 0, "malformed input: input line too short");
      return 1;
    }
    if (line[read-2] == ',') {
      // normal line, trim trailing comma
      line[read-2] = 0;
      printf("%s\n", line);
    } else {
      if (line[read-1] == '\n') {
        // last line of array
        printf("%s", line);
        break;
      } else {
        error(0, 0, "malformed input: premature EOF");
        return 1;
      }
    }
  }

  // read and discard last line
  read = getline(&line, &len, input);
  if (read == -1) {
    error(0, 0, "malformed input: final right bracket (‘]’) missing");
    return 1;
  }
  if (strncmp("]\n", line, read+1) != 0) {
    error(0, 0, "malformed input: last line is not a single right bracket (‘]‘)");
    return 1;
  }

  read = getline(&line, &len, input);
  if (read != -1) {
    error(0, 0, "malformed input: extra input after end of array");
    return 1;
  }

  return 0;
}
