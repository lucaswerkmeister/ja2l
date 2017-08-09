#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "options.h"

FILE *input;

int handleOptions(int argc, char *argv[]) {
  int c;
  struct option long_options[] = {
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' }
  };

  while (1) {
    c = getopt_long(argc, argv, "hv", long_options, NULL);
    if (c == -1) {
      break;
    }
    switch(c) {
    case 'h':
      printf("Usage: %s [FILE]\n", argv[0]);
      return 0;
    case 'v':
      printf(
        "ja2l (JSON array to lines) 0.1\n"
        "Copyright © 2017 Lucas Werkmeister\n"
        "License AGPLv3+: GNU AGPL version 3 or later <https://gnu.org/licenses/agpl.html>\n"
        "\n"
        "This is free software: you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n"
        "\n"
        "This program includes code from the systemd project,\n"
        "published under the LGPLv2.1+:\n"
        "GNU LGPL version 2.1 or later <https://gnu.org/licenses/lgpl.html>\n"
        );
      return 0;
    case '?':
      return 1;
    }
  }

  if (optind < argc) {
    if (argc == optind + 1) {
      if (strcmp(argv[optind], "-") == 0) {
        input = stdin;
      } else {
        input = fopen(argv[optind], "r");
        if (input == NULL) {
          error(0, errno, "fopen(%s)", argv[optind]);
          return 1;
        }
      }
    } else {
      error(0, 0, "too many arguments");
      fprintf(stderr, "Try ‘%s --help’ for more information.\n", argv[0]);
      return 1;
    }
  } else {
    input = stdin;
  }
  return -1;
}
