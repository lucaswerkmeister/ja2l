/***
    This file mostly consists of definitions from several header files
    from the systemd project (in the src/basic/ tree). systemd is
    licensed under the GNU LGPLv2.1+.
 ***/

#pragma once

#include <stdio.h>
#include <stdlib.h>

#define _cleanup_(x) __attribute__((cleanup(x)))

static inline void freep(void *p) {
  free(*(void**) p);
}
#define _cleanup_free_ _cleanup_(freep)

static inline void fclosep(FILE **f) {
  if (*f) {
    fclose(*f);
  }
}
#define _cleanup_fclose_ _cleanup_(fclosep)
