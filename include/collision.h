#ifndef COLLISION_H
#define COLLISION_H

#include <util.h>
#include <pieces.h>
#include <stdbool.h>
#include <board.h>

typedef enum _TSpinTypes {
	TSPIN_NONE = 0,
	TSPIN_MINI,
	TSPIN_REGULAR
} TSpinType;

#define TESTCNT 5

bool collideHorzBorders(Piece p, uint8_t rot, int8_t pieceX);
bool collideVertBorders(Piece p, uint8_t rot, int8_t piecey);
bool collideBoard(Board board, Piece p, uint8_t rot, int8_t piecex, int8_t piecey);

int countCorners(Board board, uint8_t rot, int8_t piecex, int8_t piecey);
TSpinType checkTSpin(Board board, uint8_t rot, int8_t piecex, int8_t piecey);

typedef Vec2D KickTable[ROT_CNT][TESTCNT];

extern KickTable kickTable;
extern KickTable kickTableI;

#endif
