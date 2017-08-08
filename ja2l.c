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
  _cleanup_fclosev_ FILE **outputs = NULL;
  // cleanup standard streams before exit
  _cleanup_fclose_ FILE *_0 = stdin,
    *_1 = stdout,
    *_2 = stderr;

  handleOptions(argc, argv);
  if (exitAfterOptions != -1) {
    return exitAfterOptions;
  }

  growPipe(STDIN_FILENO);
  growPipe(STDOUT_FILENO);

  outputs = calloc(2, sizeof (FILE *));
  outputs[0] = stdout;

  // read and discard first line
  read = getline(&line, &len, input);
  if (read == -1) {
    error(0, 0, "malformed input: empty");
    return EXIT_FAILURE;
  }
  if (strncmp("[\n", line, read+1) != 0) {
    error(0, 0, "malformed input: first line is not a single left bracket (‘[’)");
    return EXIT_FAILURE;
  }

  // copy array element lines
  while ((read = getline(&line, &len, input)) != -1) {
    if (read < 2) {
      error(0, 0, "malformed input: input line too short");
      return EXIT_FAILURE;
    }
    if (line[read-2] == ',') {
      // normal line, trim trailing comma
      line[read-2] = 0;
      fprintf(outputs[0], "%s\n", line);
    } else {
      if (line[read-1] == '\n') {
        // last line of array
        fprintf(outputs[0], "%s", line);
        break;
      } else {
        error(0, 0, "malformed input: premature EOF");
        return EXIT_FAILURE;
      }
    }
  }

  // read and discard last line
  read = getline(&line, &len, input);
  if (read == -1) {
    error(0, 0, "malformed input: final right bracket (‘]’) missing");
    return EXIT_FAILURE;
  }
  if (strncmp("]\n", line, read+1) != 0) {
    error(0, 0, "malformed input: last line is not a single right bracket (‘]‘)");
    return EXIT_FAILURE;
  }

  read = getline(&line, &len, input);
  if (read != -1) {
    error(0, 0, "malformed input: extra input after end of array");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
