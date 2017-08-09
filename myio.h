#pragma once

#include <stdio.h>

/*
 * Turn fd into a stream for writing,
 * making sure not to create a duplicate stream for stdout/stderr.
 */
FILE *fdopenw(int fd);
