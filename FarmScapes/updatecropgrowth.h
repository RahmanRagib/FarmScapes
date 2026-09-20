#ifndef UPDATECROPGROWTH_H
#define UPDATECROPGROWTH_H

#include <stdio.h>

#define STATE_LEVEL_1 1

#define CROP_EMPTY 0
#define CROP_PLOWED 1
#define CROP_SEEDED 2
#define CROP_WATERED 3
#define CROP_READY 4
#define CROP_ROTTEN 5
#define TOMATO_READY 6
#define BERRY_TREE 7
#define BERRY_READY 8

#define GRID_ROWS 3
#define GRID_COLS 3

#define MAX_INVENTORY_CAP 50

struct Tile {
	int x, y;
	int state;
	int growTimer;
	int spoilTimer;
	int cropType; // 0 = Rice, 1 = Tomato, 2 = Berry
};

extern int gameState;
extern int playerGold;
extern int batchTimer;
extern int batchActive;
extern int hasRottenCrop;
extern Tile farmGrid[GRID_ROWS][GRID_COLS];

// --- INVENTORY & SEEDS ---
extern int seedRice;
extern int seedTomato;
extern int seedBerry;

extern int cropRiceCount;
extern int cropTomatoCount;
extern int cropBerryCount;

// --- DYNAMIC PRICING ---
extern int riceBuyPrice, riceSellPrice;
extern int tomatoBuyPrice, tomatoSellPrice;
extern int berryBuyPrice, berrySellPrice;

// --- UI STATES ---
extern int isMarketOpen;
extern int massPlowUnlocked;
extern int showCapWarning;

inline void updateCropGrowth() {
	if (gameState != STATE_LEVEL_1) return;

	for (int r = 0; r < GRID_ROWS; r++) {
		for (int c = 0; c < GRID_COLS; c++) {
			Tile *t = &farmGrid[r][c];

			// Tile-specific crop growth calculation
			if (t->state == CROP_WATERED) {
				t->growTimer++;

				if (t->cropType == 2) { // Berry
					if (t->growTimer >= 2) {
						t->state = BERRY_READY;
					}
				}
				else if (t->cropType == 1) { // Tomato
					if (t->growTimer >= 3) {
						t->state = TOMATO_READY;
					}
				}
				else { // Rice
					if (t->growTimer >= 5) {
						t->state = CROP_READY;
					}
				}
			}
		}
	}

	// Batch Rot Timer Handling
	if (batchActive) {
		batchTimer--;

		if (batchTimer <= 0) {
			batchActive = 0;
			hasRottenCrop = 1;
			playerGold -= 20;
			if (playerGold < 0) playerGold = 0;

			for (int r = 0; r < GRID_ROWS; r++) {
				for (int c = 0; c < GRID_COLS; c++) {
					int s = farmGrid[r][c].state;
					if (s == CROP_SEEDED || s == CROP_WATERED ||
						s == CROP_READY || s == TOMATO_READY || s == BERRY_TREE || s == BERRY_READY) {
						farmGrid[r][c].state = CROP_ROTTEN;
					}
				}
			}
		}
	}
}

inline void initFarmGrid() {
	int startX = 230; // Shifted a little bit left
	int startY = 150; // Shifted a little bit down
	int tileSize = 70;
	int spacingX = 95;
	int spacingY = 90;

	for (int r = 0; r < GRID_ROWS; r++) {
		for (int c = 0; c < GRID_COLS; c++) {
			farmGrid[r][c].x = startX + c * spacingX;
			farmGrid[r][c].y = startY + r * spacingY;
			farmGrid[r][c].state = CROP_EMPTY;
			farmGrid[r][c].growTimer = 0;
			farmGrid[r][c].spoilTimer = 0;
			farmGrid[r][c].cropType = 0;
		}
	}
}

#endif // UPDATECROPGROWTH_H