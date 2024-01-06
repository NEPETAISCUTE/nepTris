#include <save.h>

bool saveDirExists() {
	struct stat st = {0};

	char path[128];
	strcpy(path, getenv("HOME"));
	strcat(path, "/.local/share/nepTris");
	return stat(path, &st) != -1;
}

uint64_t getHighScore() {
	if(!saveDirExists()) return 0;
	char path[128];
	strcpy(path, getenv("HOME"));
	strcat(path, "/.local/share/nepTris/save");
	FILE* f = fopen(path, "rb");
	if(f == NULL) return 0;
	uint64_t highScore = 0;
	fread(&highScore, sizeof(uint64_t), 1, f);
	fclose(f);

	return highScore;
}

void saveHighScore(uint64_t highScore) {
	char path[128];
	strcpy(path, getenv("HOME"));
	strcat(path, "/.local/share/nepTris");
	if(!saveDirExists()) {
		mkdir(path, 0700);
	}
	strcat(path, "/save");
	
	FILE* f = fopen(path, "wb");
	if(f == NULL) return;
	fwrite(&highScore, 4, 1, f);
	fclose(f);
}
