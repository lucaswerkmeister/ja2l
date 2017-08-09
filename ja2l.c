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
  /* The current line, obtained via getline(3). */
  _cleanup_free_ char *line = NULL;
  /* getline(3) argument (unused by us). */
  size_t len = 0;
  /* Bytes read by getline(3) (including newline, excluding null byte). */
  ssize_t read;
  /* null-terminated array of outputs. */
  _cleanup_fclosev_ FILE **outputs = NULL;
  /* Pointer into outputs, pointing at the next output to be used. */
  FILE **output = NULL;
  /* Unused variables to clean up standard streams before exit. */
  _cleanup_fclose_ FILE *_0 = stdin,
    *_1 = stdout,
    *_2 = stderr;

  handleOptions(argc, argv);
  if (exitAfterOptions != -1) {
    return exitAfterOptions;
  }

  outputs = calloc(2, sizeof (FILE *));
  outputs[0] = stdout;

  growPipe(STDIN_FILENO);
  for (output = &outputs[0]; *output; output++) {
    growPipe(fileno(*output));
  }
  output = &outputs[0];

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
      fprintf(*output, "%s\n", line);
    } else {
      if (line[read-1] == '\n') {
        // last line of array
        fprintf(*output, "%s", line);
        break;
      } else {
        error(0, 0, "malformed input: premature EOF");
        return EXIT_FAILURE;
      }
    }

    // advance to next output (circle back to first when last output is reached)
    if (!*++output) {
      output = &outputs[0];
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
