#ifndef BOARD_H
#define BOARD_H

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 22

#include <pieces.h>

typedef Piece Board[BOARD_HEIGHT][BOARD_WIDTH];

//returns the length of the buffer it filled
uint8_t checklines(Board board, uint8_t* arr);
void placeTiles(Board board, Piece p, uint8_t rot, int8_t piecex, int8_t piecey);
uint8_t findtop(Board board);

#endif
