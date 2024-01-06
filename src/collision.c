#include <collision.h>

KickTable kickTable = {
	{{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}},
	{{0,0}, {1,0}, {1,1}, {0,-2}, {1,-2}},
	{{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}},
	{{0,0}, {-1,0}, {-1,1}, {0,-2}, {-1,-2}}
};

KickTable kickTableI = {
	{{0,0}, {-2,0}, {1,0}, {-2,1}, {1,-2}},
	{{0,0}, {-1,0}, {2,0}, {-1,-2}, {2,1}},
	{{0,0}, {2,0}, {-1,0}, {2,-1}, {-1,2}},
	{{0,0}, {1,0}, {-2,0}, {1,2}, {-2,-1}}
};

bool collideBorderRight(Piece p, uint8_t rot, int8_t piecex) {
	for (int x = 3; x >= 0; x--) {
		if (piecex + x <= BOARD_WIDTH - 1) return false;	 // if we are inside the board we can't be outside of it

		// if  we are "outside" we need to check if the string rep of the piece is part of this column
		for (int y = 0; y < 4; y++) {
			if (pieceGfx[p-1][rot][x + y * 4] == 'X') return true;  // we've found a part of the piece outside of the board
		}
	}

	return false;  // we managed to check the entire piece without finding anything outside the right side
}

bool collideBorderLeft(Piece p, uint8_t rot, int8_t piecex) {
	for (int x = 0; x < 3; x++) {
		if (piecex + x >= 0) return false;	// if we are inside the board we can't be outside of it

		// if the current column is outside the board we need to check if it's empty
		for (int y = 0; y < 4; y++) {
			if (pieceGfx[p-1][rot][x + y * 4] == 'X') return true;  // we've found a part of the piece outside of the board
		}
	}

	return false;  // we managed to check the entire piece without finding anything outside the right side
}

// returns true if it collides
bool collideHorzBorders(Piece p, uint8_t rot, int8_t piecex) {
	if (piecex < 0) return collideBorderLeft(p, rot, piecex); // some part of the piece is on the left border
	if (piecex > (BOARD_WIDTH - 1) - 3) return collideBorderRight(p, rot, piecex); // some part of the piece is on the right border
	return false;
}

// returns true if it collides
bool collideVertBorders(Piece p, uint8_t rot, int8_t piecey) {
	if (piecey <= (BOARD_HEIGHT - 1) - 3) return false; // we can't be outside of the board
	
	for (int y = 3; y >= 0; y--) {
		if (piecey + y <= BOARD_HEIGHT - 1) return false; // we are inside the board and the check goes upwards
		
		// now we need to check if the row is empty
		for (int x = 0; x < 4; x++) {
			if (pieceGfx[p-1][rot][x + y * 4] == 'X') return true; // the row is outside and not empty
		}
	}

	return false; // we managed to check every rows and couldn't find any collisions
}

bool collideBoard(Piece board[BOARD_HEIGHT][BOARD_WIDTH], Piece p, uint8_t rot, int8_t piecex, int8_t piecey) {
	for(int y = 0; y < 4; y++) {
		for(int x = 0; x < 4; x++) {
			if(pieceGfx[p-1][rot][y * 4 + x] == 'X' && board[piecey+y][piecex+x] != PIECE_NONE) return true;
		}
	}	
	return false;
}

int countCorners(Board board, uint8_t rot, int8_t piecex, int8_t piecey) {
	int cnt = 0;
	for(int x = 0; x < 4; x++) {
		for(int y = 0; y < 4; y++) {
			if(pieceGfx[PIECE_T-1][rot][y * 4 + x] == 'O') {
				if(piecey + y >= BOARD_HEIGHT || piecex + x >= BOARD_WIDTH || piecex+x < 0) cnt++;
				else if(board[piecey+y][piecex+x] != PIECE_NONE) cnt++;
			}
		}
	}
	return cnt;
}

TSpinType checkTSpin(Board board, uint8_t rot, int8_t piecex, int8_t piecey) {
	int corners =countCorners(board, rot, piecex, piecey);

	if(corners < 3) return TSPIN_NONE;
	return TSPIN_REGULAR;
}
