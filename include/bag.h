#ifndef BAG_H
#define BAG_H

#include <time.h>
#include <pieces.h>
#include <util.h>

#define BAG_SIZE 7

typedef struct _Bag {
	randctx rctx;
	uint8_t bagIdx;
	Piece bag[BAG_SIZE];
} Bag;


void bagInit(Bag* b);
void bagShuffle(Bag* b);
Piece bagpick(Bag* b);

#endif
