#include "bag.h"

#include "neprand.h"

void bagInit(Bag* b) {
	srand(time(NULL));
	for(int i = 0; i < BAG_SIZE; i++) {
		b->bag[i] = i+1;
	}
}

void bagShuffle(Bag* b) {
	for(int i = 0; i < BAG_SIZE*BAG_SIZE; i++) {
		int firstIdx = RANDRANGE(0, BAG_SIZE);
	 	int secondIdx = RANDRANGE(0, BAG_SIZE);
		Piece tmp = b->bag[firstIdx];
		b->bag[firstIdx] = b->bag[secondIdx];
		b->bag[secondIdx] = tmp;
	}
}

Piece bagpick(Bag* b) {
	Piece p = b->bag[b->bagIdx++];
	if(b->bagIdx == 7) {
		b->bagIdx = 0;
		bagShuffle(b);
	}
	return p;
}
