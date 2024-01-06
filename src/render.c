#include <render.h>

void wUsePair(WINDOW* win, int pair) {
	if(win == NULL) win = stdscr;
	if(can_change_color() && COLORS > 8 && COLOR_PAIRS > 8) wattron(win, COLOR_PAIR(pair));
}

void wStopUsePair(WINDOW* win, int pair) {
	if(win == NULL) win = stdscr;
	if(can_change_color() && COLORS > 8 && COLOR_PAIRS > 8) wattroff(win, COLOR_PAIR(pair));
}

void drawTetramino(WINDOW* win, int x, int y, Piece piece, uint8_t rot) {
	if(piece == PIECE_NONE) return;
	if(win == NULL) win = stdscr;
	rot = rot & 3;
	move(y, x);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(pieceGfx[piece-1][rot][i * 4 + j] == 'X') {
				if(y+i < 0 || (x * 2) + (j * 2) < 0) continue;
				wUsePair(win, pieceClr[piece]);
	 			mvwaddch(win, y + i + 1, (x * 2) + (j * 2) + 1, ' ');
				mvwaddch(win, y + i + 1, (x * 2) + (j * 2) + 2, ' ');
		   		wStopUsePair(win, pieceClr[piece]);
			}
		}
	}
}

void drawTetraminoShadow(WINDOW* win, int x, int y, Piece piece, uint8_t rot) {
	if(piece == PIECE_NONE) return;
	if(win == NULL) win = stdscr;
	rot = rot & 3;
	move(y, x);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(pieceGfx[piece-1][rot][i * 4 + j] == 'X') {
				if(y+i < 0 || (x * 2) + (j * 2) < 0) continue;
				wUsePair(win, PAIR_SHADOW);
	 			mvwaddch(win, y + i + 1, (x * 2) + (j * 2) + 1, ' ');
				mvwaddch(win, y + i + 1, (x * 2) + (j * 2) + 2, ' ');
		   		wStopUsePair(win, PAIR_SHADOW);
			}
		}
	}
}

void drawBoard(WINDOW* win, Piece board[BOARD_HEIGHT][BOARD_WIDTH]) {
	if(win == NULL) win = stdscr;
	for(int i = 0; i < BOARD_HEIGHT; i++) {
		for(int j = 0; j < BOARD_WIDTH; j++) {
			if(board[i][j] > 0) {
				wUsePair(win, pieceClr[board[i][j]]);
				mvwaddch(win, i + 1, (j * 2) + 1, ' ');
				mvwaddch(win, i + 1, (j * 2) + 2, ' ');
				wStopUsePair(win, pieceClr[board[i][j]]);
			}
		}
	}
}

void drawText(WINDOW* win, uint8_t x, uint8_t y, const char* text) {
	if(win == NULL) win = stdscr;
	wattron(win, COLOR_PAIR(COLOR_WHITE));
	mvwprintw(win, y+1, x+1, text);
	wattroff(win, COLOR_PAIR(COLOR_WHITE));
}

int getDigitCnt(uint64_t val) {
	int cnt = 0;
	if(val == 0) return 1;
	while(val > 0) {
		val /= 10;
		cnt++;
	}
	return cnt;
}
