#include <render.h>

void wUsePair(WINDOW* win, int pair) {
	if(win == NULL) win = stdscr;
	if(can_change_color() && COLORS > 8 && COLOR_PAIRS > 8) wattron(win, COLOR_PAIR(pair));
}

void wStopUsePair(WINDOW* win, int pair) {
	if(win == NULL) win = stdscr;
	if(can_change_color() && COLORS > 8 && COLOR_PAIRS > 8) wattroff(win, COLOR_PAIR(pair));
}

void drawTetramino(WINDOW* win, DynamicPiece* p) {
	if(p->ID == PIECE_NONE) return;
	if(win == NULL) win = stdscr;
	p->rot = p->rot & 3;
	move(p->pos.y, p->pos.x);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(pieceGfx[p->ID-1][p->rot][i * 4 + j] == 'X') {
				if(p->pos.y+i < 0 || (p->pos.x * 2) + (j * 2) < 0) continue;
				wUsePair(win, pieceClr[p->ID]);
	 			mvwaddch(win, p->pos.y + i + 1, (p->pos.x * 2) + (j * 2) + 1, ' ');
				mvwaddch(win, p->pos.y + i + 1, (p->pos.x * 2) + (j * 2) + 2, ' ');
		   		wStopUsePair(win, pieceClr[p->ID]);
			}
		}
	}
}

void drawTetraminoHUD(WINDOW* win, Piece ID, int8_t posx, int8_t posy, int8_t rot) {
	if(ID == PIECE_NONE) return;
	if(win == NULL) win = stdscr;
	rot = rot & 3;
	move(posy, posx);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(pieceGfx[ID-1][rot][i * 4 + j] == 'X') {
				if(posy+i < 0 || (posx * 2) + (j * 2) < 0) continue;
				wUsePair(win, pieceClr[ID]);
	 			mvwaddch(win, posy + i + 1, (posx * 2) + (j * 2) + 1, ' ');
				mvwaddch(win, posy + i + 1, (posx * 2) + (j * 2) + 2, ' ');
		   		wStopUsePair(win, pieceClr[ID]);
			}
		}
	}
}

void drawTetraminoShadow(WINDOW* win, DynamicPiece* p) {
	if(p->ID == PIECE_NONE) return;
	if(win == NULL) win = stdscr;
	p->rot = p->rot & 3;
	move(p->hardDropy, p->pos.x);
	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			if(pieceGfx[p->ID-1][p->rot][i * 4 + j] == 'X') {
				if(p->pos.y+i < 0 || (p->pos.x * 2) + (j * 2) < 0) continue;
				wUsePair(win, PAIR_SHADOW);
	 			mvwaddch(win, p->hardDropy + i + 1, (p->pos.x * 2) + (j * 2) + 1, ' ');
				mvwaddch(win, p->hardDropy + i + 1, (p->pos.x * 2) + (j * 2) + 2, ' ');
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
