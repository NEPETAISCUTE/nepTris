#include <board.h>

void placeTiles(Board board, Piece p, uint8_t rot, int8_t piecex, int8_t piecey) {
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(pieceGfx[p-1][rot][i * 4 + j] == 'X') board[piecey + i][piecex + j] = p;
		}
	}
}

uint8_t checklines(Board board, uint8_t* arr) {
	int bufOffset = 0;
	for(int y = BOARD_HEIGHT - 1; y >= 0; y--) {
		bool checkLine = true;
		for(int x = 0; x < BOARD_WIDTH; x++) {
			if(board[y][x] == PIECE_NONE) {
				checkLine = false;
				break;
			}
		}
		if(checkLine) arr[bufOffset++] = y;
	}
	return bufOffset;
}

uint8_t findtop(Board board) {
	uint8_t highest = 21;
	for(int y = BOARD_HEIGHT - 1; y >= 0; y--) {
		for(int x = 0; x < BOARD_WIDTH; x++) {
			if(board[y][x] != PIECE_NONE) {
				highest = y;
				break;
			}
		}
	}
	return highest;
}
