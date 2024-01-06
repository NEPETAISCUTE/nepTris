#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <keys.h>
#include <bag.h>
#include <pieces.h>
#include <collision.h>
#include <rand.h>

#define VERSION "1.1.0"

#define SCOREWIN_WIDTH 15
#define SCOREWIN_HEIGHT 20

#define GAME_SPAWNX 3
#define GAME_SPAWNY 0
#define GAME_SPAWNROT 0

#define G_INC_CNT 60
#define G_MAX_DROP 20
#define G_MAX_MOVE 30

#define DEFAULT_LOCK_TIME 30
#define DEFAULT_MOVE_RESET_COUNTER 15

typedef enum _GState {
	GSTATE_START,
	GSTATE_GAME,
	GSTATE_OVER,
	GSTATE_QUIT
} GState;

typedef struct _Game {
	WINDOW* boardWin;
	WINDOW* scoreWin;

	Board board;
	Board tmpBoard;

	GState state;

	bool firstFrame;
	Color tclr[16];

	uint64_t gravity;
	uint64_t lastGravity;
	uint8_t gravcnt;
	uint8_t pcnt;

	uint64_t score;
	uint64_t scoreAdd;
	uint64_t highScore;

	uint64_t scoreAddTimer;
	bool currentDifficult;
	bool lastDifficult;
	bool isDifficult;

	bool isHighScore;

	bool chickenMode;
	bool isPaused;

	Piece currentPiece;
	uint8_t pieceRot;
	int8_t piecex;
	int8_t piecey;

	TSpinType tspin;
	char* specialActName;
	uint64_t specialActTimer;
	uint64_t combo;

	bool isOnGround;
	uint8_t maxLockTimer;
	uint8_t lockTimer;
	uint8_t maxMoveResetCnt;
	uint8_t moveResetCnt;

	bool isAllClear;

	int8_t hardDropy;

	int8_t newPiecex;
	int8_t newPiecey;
	int8_t newPieceRot;

	Piece nextPiece;
	Piece swap;
	bool canSwap;

	Bag bag;

	bool shouldQuit;

	bool isAnim;
	uint8_t animTimer;

	uint8_t dropTimer;

	int input;

	bool colorSupport;
	bool colorChangeSupport;

	bool holdBuffer;

	bool dbg; //temporary debug flag to test stuff with gdb
} Game;

void gameReset(Game* g);
Game* gameInit();
void gameQuit(Game* g, bool endNcurses, int errcode);
void drawGameInfo(Game* g);
void updateDraw(Game* g);
void gameUpdate(Game* g);

void runStateStart(Game* g);
void runStateGame(Game* g);
void runStateOver(Game* g);

void swapPiece(Game* g);
void softDrop(Game* g, bool gravity);


void calcHardDropPos(Game* g);
void hardDrop(Game* g);
void rotatePiece(Game* g);

void updateHorzPos(Game* g);
bool handleRot(Game* g);
void checkGround(Game* g);
void updateVertPos(Game* g);
void updatePos(Game* g);
void getNewPiece(Game* g);

//returns the amount of lines that were filled
int handleLines(Game* g);
#endif
