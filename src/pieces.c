#include <pieces.h>

const ColorPairName pieceClr[PIECE_CNT] = {
	[PIECE_NONE] = COLOR_DEFAULT, //none
	COLOR_PIECE_I, //PIECE_I
	COLOR_PIECE_O, //PIECE_O
	COLOR_PIECE_T, //PIECE_T
	COLOR_PIECE_S, //PIECE_S
	COLOR_PIECE_Z, //PIECE_Z
	COLOR_PIECE_J, //PIECE_J
	COLOR_PIECE_L //PIECE_L
};

const char* pieceGfx[PIECE_CNT][ROT_CNT] = {
	{"    " //PIECE_I
	 "XXXX"
	 "    "
	 "    ",
	 "  X "
	 "  X "
	 "  X "
	 "  X ",
	 "    "
	 "    "
	 "XXXX"
	 "    ",
	 " X  "
	 " X  "
	 " X  "
	 " X  "},

	{" XX " //PIECE_O
	 " XX "
	 "    "
     "    ",
	 " XX "
	 " XX "
	 "    "
	 "    ",
	 " XX "
	 " XX "
	 "    "
	 "    ",
	 " XX "
	 " XX "
	 "    "
	 "    "},

	{"OXO " //PIECE_T
	 "XXX "
	 "O O "
	 "    ",
	 "OXO "
	 " XX "
	 "OXO "
	 "    ",
	 "O O "
	 "XXX "
	 "OXO "
	 "    ",
	 "OXO "
	 "XX  "
	 "OXO "
	 "    "},

	{" XX " //PIECE_S
	 "XX  "
	 "    "
	 "    ",
	 " X  "
	 " XX "
	 "  X "
	 "    ",
	 "    "
	 " XX "
	 "XX  "
	 "    ",
	 "X   "
	 "XX  "
	 " X  "
	 "    "},

	{"XX  " //PIECE_Z
	 " XX "
	 "    "
	 "    ",
	 "  X "
	 " XX "
	 " X  "
	 "    ",
	 "    "
	 "XX  "
	 " XX "
	 "    ",
	 " X  "
	 "XX  "
	 "X   "
	 "    "},

	{"X   " //PIECE_J
	 "XXX "
	 "    "
	 "    ",
	 " XX "
	 " X  "
	 " X  "
	 "    ",
	 "    "
	 "XXX "
	 "  X "
	 "    ",
	 " X  "
	 " X  "
     "XX  "
	 "    "},

	{"  X " //PIECE_L
	 "XXX "
	 "    "
	 "    ",
	 " X  "
	 " X  "
	 " XX "
	 "    ",
	 "    "
	 "XXX "
	 "X   "
	 "    ",
	 "XX  "
	 " X  "
	 " X  "
	 "    "}
};
