#ifndef DRAWLEVEL2_H
#define DRAWLEVEL2_H

#include <stdio.h>
#include <string.h>
#include "animalgrowth.h"

// --- LEVEL 2 EXTERNS ---
extern int gameState;
extern int playerGold;

extern int countFeed;
extern int countEgg, countMilk, countWool;

extern int feedBuyPrice;
extern int eggSellPrice, milkSellPrice, woolSellPrice;
extern int henBuyPrice, cowBuyPrice, sheepBuyPrice;

extern int selectedRanchTool; // 1 = Feed, 2 = Collect
extern int isRanchMarketOpen;

// Farm Man Position
int farmmanX = 400;
int farmmanY = 130;

// --- ROAD CONSTRAINT CHECKER ---
// Returns 1 if the target (x, y) is on a valid road/path, 0 otherwise
inline int isValidRoad(int x, int y) {
	// Main horizontal road at the bottom
	if (y >= 115 && y <= 155 && x >= 80 && x <= 720) return 1;
	// Path to Left Pen (Hens)
	if (x >= 120 && x <= 160 && y >= 115 && y <= 200) return 1;
	// Path to Middle Pen (Cows)
	if (x >= 330 && x <= 370 && y >= 115 && y <= 280) return 1;
	// Path to Right Pen (Sheep)
	if (x >= 630 && x <= 670 && y >= 115 && y <= 250) return 1;

	return 0;
}

// --- MOVE FARM MAN WITH ROAD RESTRICTION ---
inline void moveFarmMan(int dx, int dy) {
	int newX = farmmanX + dx;
	int newY = farmmanY + dy;
	if (isValidRoad(newX, newY)) {
		farmmanX = newX;
		farmmanY = newY;
	}
}

// --- COLLECT PRODUCE WHEN NEAR ANIMALS ---
inline void collectProduceByFarmMan() {
	// Check Hens
	for (int i = 0; i < henCount; i++) {
		if (hens[i].isAlive && hens[i].hasProduce) {
			int distSq = (farmmanX - hens[i].x) * (farmmanX - hens[i].x) + (farmmanY - hens[i].y) * (farmmanY - hens[i].y);
			if (distSq <= 2500) { // Within ~50 pixels distance
				countEgg++;
				hens[i].hasProduce = 0;
				hens[i].produceTimer = 0;
			}
		}
	}
	// Check Cows
	for (int i = 0; i < cowCount; i++) {
		if (cows[i].isAlive && cows[i].hasProduce) {
			int distSq = (farmmanX - cows[i].x) * (farmmanX - cows[i].x) + (farmmanY - cows[i].y) * (farmmanY - cows[i].y);
			if (distSq <= 2500) {
				countMilk++;
				cows[i].hasProduce = 0;
				cows[i].produceTimer = 0;
			}
		}
	}
	// Check Sheep
	for (int i = 0; i < sheepCount; i++) {
		if (sheep[i].isAlive && sheep[i].hasProduce) {
			int distSq = (farmmanX - sheep[i].x) * (farmmanX - sheep[i].x) + (farmmanY - sheep[i].y) * (farmmanY - sheep[i].y);
			if (distSq <= 2500) {
				countWool++;
				sheep[i].hasProduce = 0;
				sheep[i].produceTimer = 0;
			}
		}
	}
}

// --- INITIALIZE RANCH DATA ---
inline void initLevel2() {
	countFeed = 5;
	countEgg = 0;
	countMilk = 0;
	countWool = 0;

	feedBuyPrice = 5;
	eggSellPrice = 15;
	milkSellPrice = 30;
	woolSellPrice = 45;

	henBuyPrice = 30;
	cowBuyPrice = 100;
	sheepBuyPrice = 70;

	farmmanX = 400;
	farmmanY = 130;

	for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++) {
		hens[i].isAlive = 0;
		cows[i].isAlive = 0;
		sheep[i].isAlive = 0;
	}

	// 1. Initial Setup: 2 Hens
	henCount = 2;
	hens[0].x = 100; hens[0].y = 170; hens[0].type = ANIMAL_HEN; hens[0].fedState = 0; hens[0].produceTimer = 0; hens[0].hasProduce = 0; hens[0].isAlive = 1;
	hens[1].x = 170; hens[1].y = 170; hens[1].type = ANIMAL_HEN; hens[1].fedState = 0; hens[1].produceTimer = 0; hens[1].hasProduce = 0; hens[1].isAlive = 1;

	hens[2].x = 100; hens[2].y = 110; hens[2].type = ANIMAL_HEN; hens[2].fedState = 0; hens[2].produceTimer = 0; hens[2].hasProduce = 0; hens[2].isAlive = 0;
	hens[3].x = 170; hens[3].y = 110; hens[3].type = ANIMAL_HEN; hens[3].fedState = 0; hens[3].produceTimer = 0; hens[3].hasProduce = 0; hens[3].isAlive = 0;

	// 2. Initial Setup: 1 Cow
	cowCount = 1;
	cows[0].x = 310; cows[0].y = 260; cows[0].type = ANIMAL_COW; cows[0].fedState = 0; cows[0].produceTimer = 0; cows[0].hasProduce = 0; cows[0].isAlive = 1;

	cows[1].x = 410; cows[1].y = 260; cows[1].type = ANIMAL_COW; cows[1].fedState = 0; cows[1].produceTimer = 0; cows[1].hasProduce = 0; cows[1].isAlive = 0;
	cows[2].x = 310; cows[2].y = 180; cows[2].type = ANIMAL_COW; cows[2].fedState = 0; cows[2].produceTimer = 0; cows[2].hasProduce = 0; cows[2].isAlive = 0;
	cows[3].x = 410; cows[3].y = 180; cows[3].type = ANIMAL_COW; cows[3].fedState = 0; cows[3].produceTimer = 0; cows[3].hasProduce = 0; cows[3].isAlive = 0;

	// 3. Initial Setup: 1 Sheep
	sheepCount = 1;
	sheep[0].x = 610; sheep[0].y = 230; sheep[0].type = ANIMAL_SHEEP; sheep[0].fedState = 0; sheep[0].produceTimer = 0; sheep[0].hasProduce = 0; sheep[0].isAlive = 1;

	sheep[1].x = 690; sheep[1].y = 230; sheep[1].type = ANIMAL_SHEEP; sheep[1].fedState = 0; sheep[1].produceTimer = 0; sheep[1].hasProduce = 0; sheep[1].isAlive = 0;
	sheep[2].x = 610; sheep[2].y = 160; sheep[2].type = ANIMAL_SHEEP; sheep[2].fedState = 0; sheep[2].produceTimer = 0; sheep[2].hasProduce = 0; sheep[2].isAlive = 0;
	sheep[3].x = 690; sheep[3].y = 160; sheep[3].type = ANIMAL_SHEEP; sheep[3].fedState = 0; sheep[3].produceTimer = 0; sheep[3].hasProduce = 0; sheep[3].isAlive = 0;
}

// --- RENDER ANIMAL ---
inline void renderAnimal(struct Animal *a, const char* bmpPath) {
	if (!a->isAlive) return;

	iShowBMP2(a->x, a->y, (char*)bmpPath, 0);

	if (a->hasProduce) {
		iSetColor(255, 220, 0);
		iFilledCircle(a->x + 20, a->y + 45, 8);
		iSetColor(0, 0, 0);
		iText(a->x + 17, a->y + 41, (char*)"!", GLUT_BITMAP_HELVETICA_12);
	}
	else if (a->fedState == 0) {
		iSetColor(220, 50, 50);
		iText(a->x, a->y - 12, (char*)"Hungry", GLUT_BITMAP_HELVETICA_10);
	}
}

// --- DRAW RANCH MARKET UI ---
inline void drawRanchMarketUI() {
	iSetColor(50, 28, 14);
	iFilledRectangle(100, 70, 600, 440);

	iSetColor(140, 95, 45);
	iRectangle(100, 70, 600, 440);

	char buf[64];
	iSetColor(240, 200, 80);
	sprintf_s(buf, sizeof(buf), "RANCH MARKET (Gold: $%d)", playerGold);
	iText(280, 470, buf, GLUT_BITMAP_HELVETICA_18);

	// Sell Sections
	sprintf_s(buf, sizeof(buf), "Eggs: %d (Sell $%d)", countEgg, eggSellPrice);
	iText(130, 400, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(45, 130, 55); iFilledRectangle(320, 395, 65, 22);
	iSetColor(255, 255, 255); iText(332, 401, (char*)"SELL", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Milk: %d (Sell $%d)", countMilk, milkSellPrice);
	iText(130, 350, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(45, 130, 55); iFilledRectangle(320, 345, 65, 22);
	iSetColor(255, 255, 255); iText(332, 351, (char*)"SELL", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Wool: %d (Sell $%d)", countWool, woolSellPrice);
	iText(130, 300, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(45, 130, 55); iFilledRectangle(320, 295, 65, 22);
	iSetColor(255, 255, 255); iText(332, 301, (char*)"SELL", GLUT_BITMAP_HELVETICA_12);

	// Buy Sections
	sprintf_s(buf, sizeof(buf), "Animal Feed: %d ($%d)", countFeed, feedBuyPrice);
	iText(420, 400, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(35, 105, 175); iFilledRectangle(610, 395, 65, 22);
	iSetColor(255, 255, 255); iText(622, 401, (char*)"BUY", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Buy Hen ($%d)", henBuyPrice);
	iText(420, 350, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(35, 105, 175); iFilledRectangle(610, 345, 65, 22);
	iSetColor(255, 255, 255); iText(622, 351, (char*)"BUY", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Buy Cow ($%d)", cowBuyPrice);
	iText(420, 300, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(35, 105, 175); iFilledRectangle(610, 295, 65, 22);
	iSetColor(255, 255, 255); iText(622, 301, (char*)"BUY", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Buy Sheep ($%d)", sheepBuyPrice);
	iText(420, 250, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(35, 105, 175); iFilledRectangle(610, 245, 65, 22);
	iSetColor(255, 255, 255); iText(622, 251, (char*)"BUY", GLUT_BITMAP_HELVETICA_12);

	// Close Button
	iSetColor(160, 40, 40);
	iFilledRectangle(600, 90, 80, 30);
	iSetColor(255, 255, 255);
	iText(618, 100, (char*)"CLOSE", GLUT_BITMAP_HELVETICA_12);
}

// --- MAIN DRAW FUNCTION FOR LEVEL 2 ---
inline void drawLevel2() {
	iSetColor(255, 255, 255);
	iShowBMPAlternative(0, 0, (char*)"assets/level2_bg.bmp");

	// Draw Animals
	for (int i = 0; i < henCount; i++) renderAnimal(&hens[i], "assets/hen.bmp");
	for (int i = 0; i < cowCount; i++) renderAnimal(&cows[i], "assets/cow.bmp");
	for (int i = 0; i < sheepCount; i++) renderAnimal(&sheep[i], "assets/sheep.bmp");

	// Draw Farm Man on the road
	iShowBMP2(farmmanX, farmmanY, (char*)"assets/farmman1.bmp", 0);

	// UI Toolbar at the bottom
	iSetColor(190, 155, 110);
	iFilledRectangle(250, 20, 300, 60);

	if (selectedRanchTool == 1) iSetColor(45, 160, 55); else iSetColor(120, 100, 80);
	iFilledRectangle(270, 28, 120, 44);
	iSetColor(255, 255, 255);
	iText(295, 44, (char*)"FEED ANIMAL", GLUT_BITMAP_HELVETICA_10);

	if (selectedRanchTool == 2) iSetColor(45, 160, 55); else iSetColor(120, 100, 80);
	iFilledRectangle(410, 28, 120, 44);
	iSetColor(255, 255, 255);
	iText(428, 44, (char*)"COLLECT", GLUT_BITMAP_HELVETICA_10);

	iSetColor(40, 40, 40);
	iFilledRectangle(0, 540, 800, 60);

	iSetColor(255, 215, 0);
	char hudStr[64];
	sprintf_s(hudStr, sizeof(hudStr), "Gold: $%d", playerGold);
	iText(10, 562, hudStr, GLUT_BITMAP_HELVETICA_12);

	iSetColor(255, 255, 255);
	sprintf_s(hudStr, sizeof(hudStr), "Feed: %d | Eggs: %d | Milk: %d | Wool: %d", countFeed, countEgg, countMilk, countWool);
	iText(110, 562, hudStr, GLUT_BITMAP_HELVETICA_10);

	iSetColor(45, 130, 180);
	iFilledRectangle(430, 552, 100, 34);
	iSetColor(255, 255, 255);
	iText(452, 564, (char*)"MARKET", GLUT_BITMAP_HELVETICA_12);

	iSetColor(40, 160, 120);
	iFilledRectangle(545, 552, 110, 34);
	iSetColor(255, 255, 255);
	iText(557, 564, (char*)"Back to Town", GLUT_BITMAP_HELVETICA_12);

	iSetColor(170, 45, 45);
	iFilledRectangle(670, 552, 110, 34);
	iSetColor(255, 255, 255);
	iText(705, 564, (char*)"MENU", GLUT_BITMAP_HELVETICA_12);

	if (isRanchMarketOpen) drawRanchMarketUI();
}

#endif // DRAWLEVEL2_H
