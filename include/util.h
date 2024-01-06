#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>
#include <rand.h>

typedef struct _Vec2D {
	int8_t x;
	int8_t y;
} Vec2D;

#define RANDRANGE(rctx, min, max) (rand(rctx) % (max - min) + min)

#endif
