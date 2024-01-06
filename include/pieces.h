#ifndef PIECES_H
#define PIECES_H

#include <ncurses.h>
#include <color.h>
#include <util.h>

#define ROT_MAX 3
#define ROT_CNT 4

typedef enum _ColorPairName {
	COLOR_DEFAULT = 1,
	COLOR_PIECE_I = 7,
	COLOR_PIECE_O = 4,
	COLOR_PIECE_T = 6,
	COLOR_PIECE_S = 3,
	COLOR_PIECE_Z = 2,
	COLOR_PIECE_J = 5,
	COLOR_PIECE_L = 8,
	COLOR_NOTEXT = 9
} ColorPairName;

typedef enum _Piece {
	PIECE_NONE = 0,
	PIECE_I, 
	PIECE_O,
	PIECE_T,
	PIECE_S,
	PIECE_Z,
	PIECE_J,
	PIECE_L,

	PIECE_MIN = PIECE_I,
	PIECE_MAX = PIECE_L,
	PIECE_CNT, //number of pieces total
	PIECE_MIRRORING = PIECE_S //where the mirroring of the pieces starts (odd is one way, even is mirrored)
} Piece;

typedef struct _DynamicPiece {
	Piece ID;
	Vec2D pos;
	uint8_t rot;
} DynamicPiece;

extern const ColorPairName pieceClr[PIECE_CNT];

extern const char* pieceGfx[PIECE_CNT][ROT_CNT];

#endif
