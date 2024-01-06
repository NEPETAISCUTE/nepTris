#include <ncurses.h>

#include <render.h>
#include <game.h>
#include <unistd.h>
#include <util.h>

int main() {
	Game* g = gameInit();
	if(g == NULL) {
		gameQuit(g, true, -1);
	}

	while(!g->shouldQuit) {
		g->input = getch();
		gameUpdate(g);
	}

	gameQuit(g, true, 0);
	return 0;
}
