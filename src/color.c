#include <color.h>
#include <ncurses.h>

void saveColors(Color c[COLCNT]) {
	for(int i = 0; i < COLCNT; i++) {
		color_content(i, &c[i].r, &c[i].g, &c[i].b);
	}
}

void setColors(const Color c[COLCNT]) {
	for(int i = 0; i < COLCNT; i++) {
		init_color(i, c[i].r, c[i].g, c[i].b);
	}
}

void setPairs(const ColorPair colorpairs[PAIRCNT]) {
	for(int i = 0; i < PAIRCNT; i++) {
		init_pair(i + 1, colorpairs[i].fg, colorpairs[i].bg);
	}
}

const ColorPair gameColorpairs[PAIRCNT] = {
	{COLOR_WHITE, COLOR_BLACK}, //default
	{COLOR_WHITE, COLOR_RED}, //Z
	{COLOR_WHITE, COLOR_GREEN}, //S
	{COLOR_WHITE, COLOR_YELLOW}, //O
	{COLOR_WHITE, COLOR_BLUE}, //J
	{COLOR_WHITE, COLOR_MAGENTA}, //T
	{COLOR_WHITE, COLOR_CYAN}, //I
	{COLOR_WHITE, COLOR_ORANGE}, //L
	{COLOR_WHITE, COLOR_GREY},
	{COLOR_BLACK, COLOR_BLACK}
};

const Color gamePltt[COLCNT] = {
	{0, 0, 0}, 			//COLOR_BLACK
	{1000, 0, 0}, 		//COLOR_RED
	{0, 1000, 0}, 		//COLOR_GREEN
	{1000, 1000, 0}, 	//COLOR_YELLOW
	{0, 0, 1000}, 		//COLOR_BLUE
	{1000, 0, 1000}, 	//COLOR_MAGENTA
	{0, 1000, 1000}, 	//COLOR_CYAN
	{1000, 1000, 1000}, //COLOR_WHITE
	{937, 474, 129}, 	//COLOR_ORANGE
	{600, 600, 600} //COLOR_GREY
};
