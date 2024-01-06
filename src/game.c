#include <game.h>
#include <color.h>
#include <render.h>
#include <util.h>
#include <save.h>

#ifdef DEBUG
const Vec2D ttsmapOffset = {0,17};
const Vec2D ttsmapSize = {10, 5};
const char* tripleTSpinMap = "XX        " \
							 "X         " \
							 "X XXXXXXXX" \
							 "X  XXXXXXX" \
							 "X XXXXXXXX";

const Vec2D acmapOffset = {0, 20};
const Vec2D acmapSize = {10, 2};
const char* acMap = 		 "   XXXXXXX" \
							 "X XXXXXXXX";
void dbgSetBoard(Board b, const char* map, Vec2D offset, Vec2D size) {
	for(int8_t y = 0; y < size.y; y++) {
		for(int8_t x = 0; x < size.x; x++) {
			b[y+offset.y][x+offset.x] = map[y * BOARD_WIDTH + x] == 'X' ? PIECE_I : PIECE_NONE;
		}
	}
}

#endif

void gameReset(Game* g) {
	memset(g->board, PIECE_NONE, sizeof(Board));
	g->state = GSTATE_START;
	g->pieceRot = GAME_SPAWNROT;
	g->piecex = GAME_SPAWNX;
	g->piecey = GAME_SPAWNY;
	g->swap = PIECE_NONE;
	g->canSwap = true;
	g->gravity = 1;
	g->pcnt = G_INC_CNT;
	g->score = 0;
	g->isHighScore = false;
	g->isAnim = false;
	g->specialActName = NULL;
	g->combo = 0;
	g->lastDifficult = false;
	g->currentDifficult = false;
	g->isOnGround = false;
	g->maxLockTimer = DEFAULT_LOCK_TIME;
	g->lockTimer = g->maxLockTimer;
	g->maxMoveResetCnt = DEFAULT_MOVE_RESET_COUNTER;
	g->moveResetCnt = g->maxMoveResetCnt;

	g->scoreAdd = 0;
	g->scoreAddTimer = 0;
	bagShuffle(&g->bag);

	g->currentPiece = bagpick(&g->bag); //PIECE_T; 
	g->nextPiece = bagpick(&g->bag);
	g->input = ERR;
	g->holdBuffer = false;
	g->dbg = false;
	//dbgSetBoard(g->board, acMap, acmapOffset, acmapSize);
}

Game* gameInit() {	
	initscr();
	curs_set(0);
	raw();
	noecho();
	timeout(FRAME_RATE);
	keypad(stdscr, TRUE);

	Game* g = malloc(sizeof(Game));

	start_color();
	use_default_colors();

	g->colorSupport = has_colors();
	g->colorChangeSupport = can_change_color();
	if(g->colorSupport && g->colorChangeSupport && COLORS >= 8 && COLOR_PAIRS >= 8) {
		saveColors(g->tclr);
		setColors(gamePltt);
		setPairs(gameColorpairs);
	}


	if(g == NULL) return NULL;

	g->scoreWin = NULL;

	g->boardWin = newwin(BOARD_HEIGHT + 2, (BOARD_WIDTH * 2) + 2, 0, 0);
	if(!g->boardWin) {
		gameQuit(g, false, -1);
		return NULL;
	}
	g->scoreWin = newwin(SCOREWIN_HEIGHT, SCOREWIN_WIDTH, 0, (BOARD_WIDTH * 2) + 2);
	if(!g->scoreWin) {
		gameQuit(g, false, -1);
		return NULL;
	}
	/*g->nextWin = newwin(10, 10, 0, (BOARD_WIDTH * 2) + 12);
	if(!g->nextWin) {
		gameQuit(g, false, -1);
		return NULL;
	}	
	g->holdWin = newwin(10, 10, 10, (BOARD_WIDTH * 2) + 12);
	if(!g->holdWin) {
		gameQuit(g, false, -1);
		return NULL;
	}*/
	bagInit(&g->bag);
	g->highScore = getHighScore();
	g->shouldQuit = false;
	g->chickenMode = false;
	gameReset(g);
	return g;
}

void gameQuit(Game* g, bool quit, int errcode) {
	if(g != NULL) {
		if(has_colors() && can_change_color() && COLORS > 8 && COLOR_PAIRS > 8) setColors(g->tclr);
		if(g->boardWin) delwin(g->boardWin);
		if(g->scoreWin) delwin(g->scoreWin);
		//if(g->nextWin) delwin(g->scoreWin);
		//if(g->holdWin) delwin(g->scoreWin);
		free(g);
	}

	if(quit) {
		endwin();
		exit(errcode);
	}
}

void gameUpdate(Game* g) {
	if(g->input == GK_RESET) gameReset(g);
	if(g->input == GK_CHICKEN) g->chickenMode ^= 1;
	GState state = g->state;
	if(g->firstFrame) erase();
	switch(g->state) {
		case GSTATE_START:
			runStateStart(g);
			break;
		case GSTATE_GAME:
			runStateGame(g);
			break;
		case GSTATE_OVER:
			if(g->firstFrame) {
				g->score += g->scoreAdd;
				if(g->score > g->highScore) {
					g->highScore = g->score;
					g->isHighScore = true;
					saveHighScore(g->highScore);
				}
			}
			runStateOver(g);
		default:
			break;
	}
	if(g->firstFrame) g->firstFrame = false;
	if(g->state != state) g->firstFrame = true;
	if(g->input == GK_QUIT) g->shouldQuit = true;
	updateDraw(g);
}

void updateDraw(Game* g) {
	drawGameInfo(g);
	box(g->boardWin, 0, 0);
	box(g->scoreWin, 0, 0);

	wnoutrefresh(g->scoreWin);
	wnoutrefresh(g->boardWin);
	doupdate();
	werase(g->scoreWin);
	werase(g->boardWin);
}

void drawGameInfo(Game* g) {
	mvprintw(24, 0, "press left and right to move the piece");
	mvprintw(25, 0, "press down to softdrop the piece, and up to harddrop it");
	mvprintw(26, 0, "press %c to rotate the piece counterclockwise and %c to rotate it clockwise", GK_ROTATE_CCW, GK_ROTATE_CLW);
	mvprintw(27, 0, "press %c to hold the piece (second slot on the small window at the right of the board)", GK_HOLD);
	mvprintw(28, 0, "press %c to toggle chicken mode : %s", GK_CHICKEN, g->chickenMode ? "enabled " : "disabled");
	mvprintw(29, 0, "press %c to pause/unpause, %c to quit, and %c to reset the game", GK_PAUSE, GK_QUIT, GK_RESET);
	mvprintw(30, 0, "made by nepecam");
	#ifdef DEBUG
	mvprintw(31, 0, "lock timer: %u ", g->lockTimer);
	mvprintw(32, 0, "lock reset counter: %u ", g->moveResetCnt);
	#endif

	mvwprintw(g->scoreWin, 1, SCOREWIN_WIDTH/2 - 2, "NEXT");
	mvwprintw(g->scoreWin, 5, SCOREWIN_WIDTH/2 - 2, "HOLD");
}

void runStateStart(Game* g) {
	wattron(g->boardWin, A_BOLD);
	mvwprintw(g->boardWin, BOARD_HEIGHT/2 - 3, BOARD_WIDTH - 2, "nepTris");
	mvwprintw(g->boardWin, BOARD_HEIGHT/2 - 2, BOARD_WIDTH - 4, "Press Enter");
	mvwprintw(g->boardWin, BOARD_HEIGHT/2, BOARD_WIDTH - 3, "Hi Score");
	mvwprintw(g->boardWin, BOARD_HEIGHT/2 + 1, BOARD_WIDTH + 1 - (getDigitCnt(g->highScore)/2), "%lu", g->highScore);
	if(!g->colorSupport) mvwprintw(g->boardWin, BOARD_HEIGHT/2 + 2, BOARD_WIDTH - 5, "your terminal\n doesn't support\n colors");
	else if(!g->colorChangeSupport) mvwprintw(g->boardWin, BOARD_HEIGHT/2 + 2, BOARD_WIDTH - 5, "your terminal\n doesn't support\n color editing");
	mvwprintw(g->boardWin, BOARD_HEIGHT, 1, "%s", VERSION);
	if(g->input == GK_START) g->state = GSTATE_GAME;
	wattroff(g->boardWin, A_BOLD);
}

void runStateGame(Game* g) {
	if(g->input == GK_PAUSE) g->isPaused ^= 1;
	if(g->isPaused) {
		wattron(g->boardWin, A_BOLD);	
		mvwprintw(g->boardWin, BOARD_HEIGHT/2 - 1, BOARD_WIDTH - 2, "Paused");
		wattroff(g->boardWin, A_BOLD);	
		return;
	}
	if(!g->isAnim) {
		g->newPiecex = g->piecex;
		g->newPiecey = g->piecey;
		g->newPieceRot = g->pieceRot;

		if(g->input == GK_ROTATE_CLW || g->input == GK_ROTATE_CCW) rotatePiece(g);

		if(g->input == GK_HOLD || g->holdBuffer) swapPiece(g);
		g->holdBuffer = false;

		if(g->input == GK_G_UP) g->gravity++;
		if(g->input == GK_G_DOWN) g->gravity--;

		if(g->input == GK_LEFT) g->newPiecex = g->piecex - 1;
		if(g->input == GK_RIGHT) g->newPiecex = g->piecex + 1;


		updateHorzPos(g);

		calcHardDropPos(g);
		checkGround(g);

		int8_t fallSpeed;
		if( g->gravity < 20) fallSpeed = 60 - (g->gravity - 1) * 3;
		else fallSpeed = 0;
		if(g->gravcnt >= fallSpeed || g->input == KEY_DOWN) softDrop(g, g->input >= fallSpeed);
		if(g->gravity != g->lastGravity) {
			g->lastGravity = g->gravity;
			if(g->gravity > G_MAX_DROP) {
				if(g->maxLockTimer > 0) g->maxLockTimer -= 2;
				if(g->maxMoveResetCnt > 0) g->maxMoveResetCnt--;
			}
		}
		if(g->input == GK_HARD_DROP) hardDrop(g);
		updateVertPos(g);
		if(g->isAnim) return;
		g->gravcnt++;
		if(g->dropTimer > 0) g->dropTimer--;
			
		drawBoard(g->boardWin, g->board);
		if(g->chickenMode) drawTetraminoShadow(g->boardWin, g->piecex, g->hardDropy, g->currentPiece, g->pieceRot);
		drawTetramino(g->boardWin, g->piecex, g->piecey, g->currentPiece, g->pieceRot);
	} else {
		if(g->input == GK_HOLD) g->holdBuffer = true;
		drawBoard(g->boardWin, g->tmpBoard);
		if(g->isAllClear) mvwprintw(g->boardWin, BOARD_HEIGHT/2, BOARD_WIDTH-4, "All Clear");
		g->animTimer--;
		if(g->animTimer == 0) g->isAnim = false;
	}
	if(g->specialActName != NULL) {
		mvwprintw(g->scoreWin, 16, 1, "%s %s", g->specialActName, (g->lastDifficult && g->currentDifficult && g->isDifficult) ? "B2B" : "");
		g->specialActTimer--;
		if(g->specialActTimer == 0) {
			g->specialActName = NULL;
		}
	}
	if(g->combo > 1) mvwprintw(g->scoreWin, 17, 1, "Combo X %lu", g->combo);
	drawTetramino(g->scoreWin, SCOREWIN_WIDTH/2 - 6, 1, g->nextPiece, 0);
	drawTetramino(g->scoreWin, SCOREWIN_WIDTH/2 - 6, 6, g->swap, 0);	
	mvwprintw(g->scoreWin, 11, 1, "left %u", g->pcnt);
	mvwprintw(g->scoreWin, 12, 1, "level %lu", g->gravity);
	mvwprintw(g->scoreWin, 13, 1, "score");
	mvwprintw(g->scoreWin, 14, 1, "%lu", g->score);
	if(g->scoreAdd > 0) {
		mvwprintw(g->scoreWin, 15, 1, "+%lu", g->scoreAdd);
		if(g->scoreAddTimer == 0) {
			g->score += g->scoreAdd;
			g->scoreAdd = 0;
		}
		g->scoreAddTimer--;
	}
}

void runStateOver(Game* g) {
	wattron(g->boardWin, A_BOLD);
	mvwprintw(g->boardWin, BOARD_HEIGHT/2 + 0, BOARD_WIDTH - 3, "Game Over!");
	mvwprintw(g->boardWin, BOARD_HEIGHT/2 + 2, BOARD_WIDTH - 1, "Score:");
	mvwprintw(g->boardWin, BOARD_HEIGHT/2 + 3, BOARD_WIDTH + 1 - (getDigitCnt(g->score)/2), "%lu", g->score);
	if(g->isHighScore) mvwprintw(g->boardWin, BOARD_HEIGHT/2 + 4, BOARD_WIDTH - 4, "Hi Score!");
	wattroff(g->boardWin, A_BOLD);
	if(g->input == GK_START) {
		gameReset(g);
		g->state = GSTATE_START;
	}
}

void checkGameover(Game* g) {
	if(collideBoard(g->board, g->currentPiece, g->newPieceRot, g->newPiecex, g->newPiecey)) g->state = GSTATE_OVER;
}

void swapPiece(Game* g) {
	if(!g->canSwap) return;
	if(g->swap == PIECE_NONE) {
		g->swap = g->currentPiece;
		g->currentPiece = g->nextPiece;
		g->nextPiece = bagpick(&g->bag);
	} else {
		Piece tmp = g->swap;
		g->swap = g->currentPiece;
		g->currentPiece = tmp;
	}
	g->piecex = g->newPiecex = GAME_SPAWNX;
	g->piecey = g->newPiecey = GAME_SPAWNY;
	g->pieceRot = g->newPieceRot = GAME_SPAWNROT;
	g->canSwap = false;
	g->isOnGround = false;
	g->lockTimer = 30;
	g->moveResetCnt = 15;
	checkGameover(g);
}

void rotatePiece(Game* g) {
	if(g->input == GK_ROTATE_CLW) g->newPieceRot = (g->pieceRot + 1) & ROT_MAX;
	else g->newPieceRot = (g->pieceRot - 1) & ROT_MAX;
}

void softDrop(Game* g, bool gravity) {
	if(g->input == KEY_DOWN && !g->isOnGround) g->score++;
	g->newPiecey = g->piecey + 1;
	g->gravcnt = 0;
}

void calcHardDropPos(Game* g) {
	g->hardDropy = g->piecey;
	while(!collideBoard(g->board, g->currentPiece, g->pieceRot, g->piecex, g->hardDropy) &&
		  !collideVertBorders(g->currentPiece, g->pieceRot, g->hardDropy)) {
			g->hardDropy++;
	}
	g->hardDropy--;
}

void hardDrop(Game* g) {	
	if(g->dropTimer != 0) return;
	g->score += 2 * (g->hardDropy - g->newPiecey);
	g->piecey = g->hardDropy;
	g->newPiecey = g->hardDropy + 1;
	g->isOnGround = true;
	g->lockTimer = 0;
	g->dropTimer = 10;
}

void updateHorzPos(Game* g) {
	if(g->piecex == g->newPiecex && g->pieceRot == g->newPieceRot) return;
	bool isMovementPossible = false;
	if(g->piecex != g->newPiecex) {
		g->tspin = TSPIN_NONE;
		if(!collideHorzBorders(g->currentPiece, g->newPieceRot, g->newPiecex) &&
		!collideBoard(g->board, g->currentPiece, g->newPieceRot, g->newPiecex, g->piecey)) {
			g->piecex = g->newPiecex;
			g->pieceRot = g->newPieceRot;
			isMovementPossible = true;
		}
	} else isMovementPossible = handleRot(g);
	if(isMovementPossible && g->isOnGround && g->moveResetCnt > 0) {
		g->moveResetCnt--;
		g->isOnGround = collideVertBorders(g->currentPiece, g->pieceRot, g->piecey+1) ||
						collideBoard(g->board, g->currentPiece, g->pieceRot, g->piecex, g->piecey + 1);
		g->lockTimer = 30;
	}
}

void dbgBrk() {
	return;
}

bool handleRot(Game* g) {
	KickTable* kicks = &kickTable;
	if(g->currentPiece == PIECE_I) kicks = &kickTableI;
	bool isCCL = (g->pieceRot == 0 && g->newPieceRot == 3) || (g->newPieceRot < g->pieceRot && !(g->pieceRot == 3 && g->newPieceRot == 0));
	for(int i = 0; i < 5; i++) {
		Vec2D pos = (*kicks)[(g->pieceRot - isCCL)&ROT_MAX][i];
		if(isCCL) {pos.x = -pos.x; pos.y = -pos.y;}
		if(!collideHorzBorders(g->currentPiece, g->newPieceRot, g->newPiecex + pos.x) &&
		   !collideBoard(g->board, g->currentPiece, g->newPieceRot, g->newPiecex + pos.x, g->newPiecey + pos.y) &&
		   !collideVertBorders(g->currentPiece, g->newPieceRot, g->newPiecey + pos.y)) {
			g->piecex = g->newPiecex + pos.x;
			g->newPiecex = g->piecex;
			g->pieceRot = g->newPieceRot;
			g->piecey = g->newPiecey + pos.y;
			g->newPiecey = g->piecey;
			if(g->currentPiece == PIECE_T) g->tspin = checkTSpin(g->board, g->pieceRot, g->piecex, g->newPiecey);
			return true;
		}
	}
	return false;
}

void checkGround(Game* g) {
	g->isOnGround = collideVertBorders(g->currentPiece, g->pieceRot, g->piecey+1) ||
					collideBoard(g->board, g->currentPiece, g->pieceRot, g->piecex, g->piecey + 1);
}

void updateVertPos(Game* g) {
	if(g->piecey == g->newPiecey && !g->isOnGround) return;
	if(!collideVertBorders(g->currentPiece, g->pieceRot, g->newPiecey) &&
	   !collideBoard(g->board, g->currentPiece, g->pieceRot, g->piecex, g->newPiecey) &&
	   !g->isOnGround) {
		g->piecey = g->newPiecey;
		g->tspin = TSPIN_NONE;
		return;
	}
	if(!g->isOnGround) return;
	if(g->lockTimer > 0) {
		g->lockTimer--;
		return;
	}
	placeTiles(g->board, g->currentPiece, g->pieceRot, g->piecex, g->piecey);
	g->isAllClear = false;
	int lnCnt = handleLines(g);
	if(lnCnt != 0) {
		g->lastDifficult = g->currentDifficult;
		g->currentDifficult = false;
	}
	g->isDifficult = false;
	uint64_t lastScoreAdd = g->scoreAdd;
	uint64_t actionScore = 0;
	switch(lnCnt) {
		case 1:
			if(g->tspin == TSPIN_REGULAR) {
				actionScore += 800 * g->gravity;
				g->specialActName = "Single TSpin";
				g->currentDifficult = true;
				g->isDifficult = true;
			}
			else {
				actionScore += 100 * g->gravity;
				g->specialActName = "Single";
			}
			if(g->isAllClear) g->scoreAdd += 800 * g->gravity;
			break;
		case 2:
			if(g->tspin == TSPIN_REGULAR) {
				actionScore += 1200 * g->gravity;
				g->specialActName = "Double TSpin";
				g->currentDifficult = true;
				g->isDifficult = true;
			}
			else {
				actionScore += 300 * g->gravity;
				g->specialActName = "Double";
			}
			if(g->isAllClear) g->scoreAdd += 1200 * g->gravity;
			break;
		case 3:
			if(g->tspin == TSPIN_REGULAR) {
				actionScore += 1600 * g->gravity;
				g->specialActName = "Triple TSpin";
				g->currentDifficult = true;
				g->isDifficult = true;
			}
			else {
				actionScore += 500 * g->gravity;
				g->specialActName = "Triple";
			}
			if(g->isAllClear) g->scoreAdd += 1800 * g->gravity;
			break;
		case 4:
			actionScore += 800 * g->gravity;
			g->specialActName = "Tetris";
			g->currentDifficult = true;
			g->isDifficult = true;
			if(g->isAllClear) {
				if(g->lastDifficult && g->currentDifficult) g->scoreAdd += 3200 * g->gravity;
				else g->scoreAdd += 2000 * g->gravity;
			}
			break;
		default:
			if(g->tspin == TSPIN_REGULAR) {
				actionScore += 200 * g->gravity;
				g->specialActName = "TSpin Mini";
			}
			break;
	}
	if(g->lastDifficult && g->currentDifficult) actionScore *= 1.5;
	g->scoreAdd += actionScore;
	g->scoreAdd += 50 * g->combo * g->gravity;
	if(g->scoreAdd - lastScoreAdd > 0) g->scoreAddTimer = 60; 
	if(g->specialActName != NULL) g->specialActTimer = 120;
	if(lnCnt > 0) g->combo++;
	else g->combo = 0;
	getNewPiece(g);
	g->moveResetCnt = g->maxMoveResetCnt;
	g->isOnGround = false;
	g->lockTimer = g->maxLockTimer;
}

void getNewPiece(Game* g) {
	if(collideBoard(g->board, g->nextPiece, GAME_SPAWNROT, GAME_SPAWNX, GAME_SPAWNY)) {
		g->state = GSTATE_OVER;
		return;
	}
	g->currentPiece = g->nextPiece;
	g->nextPiece = bagpick(&g->bag);
	g->piecex = GAME_SPAWNX;
	g->piecey = GAME_SPAWNY;
	g->pieceRot = GAME_SPAWNROT;
	g->canSwap = true;
	g->pcnt--;
	if(g->pcnt == 0) {
		g->pcnt = G_INC_CNT;
		g->gravity++;
	}
}

int handleLines(Game* g) {
	uint8_t lineIdx[BOARD_HEIGHT];
	uint8_t len = checklines(g->board, lineIdx);
	if(len == 0) return 0;
	uint8_t top = findtop(g->board);
	if(top + len == BOARD_HEIGHT) g->isAllClear = true;
	for(int i = len - 1; i >= 0; i--) {
		memset(&g->board[lineIdx[i]], PIECE_NONE, BOARD_WIDTH * sizeof(Piece));
	}
	memcpy(&g->tmpBoard, &g->board, BOARD_WIDTH * BOARD_HEIGHT * sizeof(Piece));
	for(int i = len - 1; i >= 0; i--) {
		memmove(&g->board[top + 1], &g->board[top], ((lineIdx[i] - top) * BOARD_WIDTH * sizeof(Piece)));
		memset(&g->board[top++], PIECE_NONE, BOARD_WIDTH * sizeof(Piece));
	}
	g->isAnim = true;
	g->animTimer = g->isAllClear ? 60 : 30;
	return len;
}
