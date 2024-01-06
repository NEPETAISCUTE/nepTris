#ifndef COLOR_H
#define COLOR_H

#define COLOR_ORANGE 8
#define COLOR_GREY 9

#define PAIR_SHADOW 9

#define COLCNT 10
#define PAIRCNT 10

typedef struct _Color {
	short r;
	short g;
	short b;
} Color;

typedef struct _ColorPair {
	short fg;
	short bg;
} ColorPair;

void saveColors(Color c[COLCNT]);
void setColors(const Color c[COLCNT]);
void setPairs(const ColorPair colpairs[PAIRCNT]);

extern const ColorPair gameColorpairs[PAIRCNT];
extern const Color gamePltt[COLCNT];

#endif
