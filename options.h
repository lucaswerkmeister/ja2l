#pragma once

/* Input stream to be used. */
extern FILE *input;

/*
 * Handle command line arguments (options and also non-option arguments).
 * Returns -1 on success;
 * any other return code means that the caller should exit with this code.
 */
int handleOptions(int argc, char *argv[]);
