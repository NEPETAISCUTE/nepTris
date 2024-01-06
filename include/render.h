#ifndef RENDER_H
#define RENDER_H

#include <pieces.h>
#include <game.h>

#define FRAME_RATE 1000/60

void wUsePair(WINDOW* win, int pair);
void wStopUsePair(WINDOW* win, int pair);

void drawTetramino(WINDOW* win, int x, int y, Piece piece, uint8_t rot);
void drawTetraminoShadow(WINDOW* win, int x, int y, Piece piece, uint8_t rot);
void drawBoard(WINDOW* win, Piece board[BOARD_HEIGHT][BOARD_WIDTH]);
void drawText(WINDOW* win, uint8_t x, uint8_t y, const char* text);

int getDigitCnt(uint64_t val);
#endif
