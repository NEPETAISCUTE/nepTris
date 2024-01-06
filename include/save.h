#ifndef SAVE_H
#define SAVE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t getHighScore();
void saveHighScore(uint64_t highScore);

#endif
