#ifndef DRAWLEVEL1_H
#define DRAWLEVEL1_H

#include <stdio.h>

extern int playerGold;
extern int selectedTool;
extern int batchTimer;
extern int batchActive;
extern Tile farmGrid[3][3];

extern int seedRice, seedTomato, seedBerry;
extern int cropRiceCount, cropTomatoCount, cropBerryCount;
extern int isMarketOpen;
extern int massPlowUnlocked;
extern int showCapWarning;

extern int riceBuyPrice, riceSellPrice;
extern int tomatoBuyPrice, tomatoSellPrice;
extern int berryBuyPrice, berrySellPrice;

inline void drawMarketplaceUI() {

	iSetColor(50, 28, 14);
	iFilledRectangle(100, 70, 600, 440);


	iSetColor(140, 95, 45);
	iRectangle(100, 70, 600, 440);


	iShowBMPAlternative2(280, 465, "assets/market.bmp", 0xFFFFFF);


	iShowBMPAlternative2(130, 435, "assets/gold.bmp", 0xFFFFFF);
	char buf[64];
	sprintf_s(buf, sizeof(buf), "$%d", playerGold);
	iSetColor(240, 200, 80);
	iText(270, 442, buf, GLUT_BITMAP_HELVETICA_18);



	iShowBMPAlternative2(130, 400, "assets/scrops.bmp", 0xFFFFFF);

	iSetColor(230, 210, 170);
	sprintf_s(buf, sizeof(buf), "Rice: %d/%d (Sell $%d)", cropRiceCount, MAX_INVENTORY_CAP, riceSellPrice);
	iText(130, 375, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(45, 130, 55); iFilledRectangle(330, 370, 65, 22);
	iSetColor(255, 255, 255); iText(342, 376, "SELL", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Tomato: %d/%d (Sell $%d)", cropTomatoCount, MAX_INVENTORY_CAP, tomatoSellPrice);
	iText(130, 335, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(45, 130, 55); iFilledRectangle(330, 330, 65, 22);
	iSetColor(255, 255, 255); iText(342, 336, "SELL", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Berry: %d/%d (Sell $%d)", cropBerryCount, MAX_INVENTORY_CAP, berrySellPrice);
	iText(130, 295, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(45, 130, 55); iFilledRectangle(330, 290, 65, 22);
	iSetColor(255, 255, 255); iText(342, 296, "SELL", GLUT_BITMAP_HELVETICA_12);

	// Buy Seeds Section
	iShowBMPAlternative2(430, 400, "assets/bseeds.bmp", 0xFFFFFF);

	iSetColor(230, 210, 170);
	sprintf_s(buf, sizeof(buf), "Rice Seeds: %d ($%d)", seedRice, riceBuyPrice);
	iText(430, 375, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(35, 105, 175); iFilledRectangle(600, 370, 65, 22);
	iSetColor(255, 255, 255); iText(612, 376, "BUY", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Tomato Seeds: %d ($%d)", seedTomato, tomatoBuyPrice);
	iText(430, 335, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(35, 105, 175); iFilledRectangle(600, 330, 65, 22);
	iSetColor(255, 255, 255); iText(612, 336, "BUY", GLUT_BITMAP_HELVETICA_12);

	sprintf_s(buf, sizeof(buf), "Berry Seeds: %d ($%d)", seedBerry, berryBuyPrice);
	iText(430, 295, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(35, 105, 175); iFilledRectangle(600, 290, 65, 22);
	iSetColor(255, 255, 255); iText(612, 296, "BUY", GLUT_BITMAP_HELVETICA_12);

	// Special Upgrades Section
	iShowBMPAlternative2(130, 235, "assets/upgrade.bmp", 0xFFFFFF);

	if (!massPlowUnlocked) {
		iSetColor(230, 210, 170);
		iText(130, 195, "Mass-Plow All Tiles ($1500 gold)", GLUT_BITMAP_HELVETICA_12);
		iSetColor(170, 45, 45); iFilledRectangle(380, 188, 130, 26);
		iSetColor(255, 255, 255); iText(388, 195, "UNLOCK ($1500)", GLUT_BITMAP_HELVETICA_10);
	} else {
		iSetColor(80, 220, 100);
		iText(130, 195, "Mass-Plow Unlocked! (Use toolbar button)", GLUT_BITMAP_HELVETICA_12);
	}

	// Close Button
	iSetColor(160, 40, 40);
	iFilledRectangle(600, 80, 80, 30);
	iSetColor(255, 255, 255);
	iText(618, 90, "CLOSE", GLUT_BITMAP_HELVETICA_12);
}
inline void drawLevel1() {
	iSetColor(255, 255, 255);
	iShowBMPAlternative(0, 0, "assets/mainland_bg.bmp");

	// --- FARM TILES ---
	for (int r = 0; r < GRID_ROWS; r++) {
		for (int c = 0; c < GRID_COLS; c++) {
			Tile t = farmGrid[r][c];
			iSetColor(255, 255, 255);

			switch (t.state) {
			case CROP_EMPTY:
				iShowBMPAlternative(t.x, t.y, "assets/tile_grass.bmp");
				break;
			case CROP_PLOWED:
				iShowBMPAlternative(t.x, t.y, "assets/tile_plowed.bmp");
				break;
			case CROP_SEEDED:
				iShowBMPAlternative(t.x, t.y, "assets/tile_seeded.bmp");
				break;
			case CROP_WATERED:
				iShowBMPAlternative(t.x, t.y, "assets/tile_watered.bmp");
				break;
			case CROP_READY:
				iShowBMPAlternative(t.x, t.y, "assets/tile_ready.bmp");
				break;
			case TOMATO_READY:
				iShowBMPAlternative(t.x, t.y, "assets/tile_tomato_ripe.bmp");
				break;
			case BERRY_TREE:
				iShowBMPAlternative(t.x, t.y, "assets/tile_seeded.bmp");
				break;
			case BERRY_READY:
				iShowBMPAlternative(t.x, t.y, "assets/tile_berry.bmp");
				break;
			case CROP_ROTTEN:
				iSetColor(80, 50, 20);
				iFilledRectangle(t.x, t.y, 80, 80);
				iSetColor(255, 0, 0);
				iText(t.x + 15, t.y + 35, "ROTTEN!", GLUT_BITMAP_HELVETICA_12);
				break;
			}
		}
	}

	// --- INVENTORY FULL WARNING ---
	if (showCapWarning) {
		iSetColor(220, 40, 40);
		iFilledRectangle(150, 480, 500, 40);
		iSetColor(255, 255, 255);
		iRectangle(150, 480, 500, 40);
		iText(160, 494, "INVENTORY FULL (50)! Please sell your existing crops in Market.", GLUT_BITMAP_HELVETICA_12);
	}

	// --- BOTTOM TOOLBAR ---
	int barX = massPlowUnlocked ? 80 : 150;
	int barWidth = massPlowUnlocked ? 640 : 500;
	iSetColor(190, 155, 110);
	iFilledRectangle(barX, 20, barWidth, 60);

	// Tool buttons
	if (selectedTool == 1) iSetColor(45, 160, 55); else iSetColor(120, 100, 80);
	iFilledRectangle(170, 28, 90, 44);
	iSetColor(255, 255, 255);
	iText(195, 44, "PLOW", GLUT_BITMAP_HELVETICA_12);

	if (selectedTool == 2) iSetColor(45, 160, 55); else iSetColor(120, 100, 80);
	iFilledRectangle(290, 28, 90, 44);
	iSetColor(255, 255, 255);
	iText(312, 44, "PLANT", GLUT_BITMAP_HELVETICA_12);

	if (selectedTool == 3) iSetColor(45, 160, 55); else iSetColor(120, 100, 80);
	iFilledRectangle(410, 28, 90, 44);
	iSetColor(255, 255, 255);
	iText(430, 44, "WATER", GLUT_BITMAP_HELVETICA_12);

	if (selectedTool == 4) iSetColor(45, 160, 55); else iSetColor(120, 100, 80);
	iFilledRectangle(530, 28, 100, 44);
	iSetColor(255, 255, 255);
	iText(542, 44, "HARVEST", GLUT_BITMAP_HELVETICA_10);

	if (massPlowUnlocked) {
		iSetColor(140, 50, 180);
		iFilledRectangle(90, 28, 70, 44);
		iSetColor(255, 255, 255);
		iText(95, 48, "MASS", GLUT_BITMAP_HELVETICA_10);
		iText(95, 34, "PLOW", GLUT_BITMAP_HELVETICA_10);
	}

	// --- TOP HUD BAR (Drawn near last) ---
	iSetColor(40, 40, 40);
	iFilledRectangle(0, 540, 800, 60);

	// Total Gold Display
	iSetColor(255, 215, 0);
	char hudStr[64];
	sprintf_s(hudStr, sizeof(hudStr), "Total: $%d", playerGold);
	iText(10, 572, hudStr, GLUT_BITMAP_HELVETICA_12);

	// ALWAYS VISIBLE TIMER (X: 10, Y: 548 - Below Gold)
	iSetColor(255, 255, 0);
	if (batchActive) {
		sprintf_s(hudStr, sizeof(hudStr), "Timer: %ds", batchTimer);
	}
	else {
		sprintf_s(hudStr, sizeof(hudStr), "Timer: Idle");
	}
	iText(10, 548, hudStr, GLUT_BITMAP_HELVETICA_12);

	// Compact Inventory Info (X: 110 to 410)
	iSetColor(255, 255, 255);
	sprintf_s(hudStr, sizeof(hudStr), "Seeds: R:%d T:%d B:%d | Crops: R:%d T:%d B:%d",
		seedRice, seedTomato, seedBerry, cropRiceCount, cropTomatoCount, cropBerryCount);
	iText(110, 562, hudStr, GLUT_BITMAP_HELVETICA_10);

	// Buttons (X: 420 to 780)
	iSetColor(45, 130, 180);
	iFilledRectangle(420, 552, 100, 34);
	iSetColor(255, 255, 255);
	iRectangle(420, 552, 100, 34);
	iText(442, 564, "MARKET", GLUT_BITMAP_HELVETICA_12);

	iSetColor(40, 160, 120);
	iFilledRectangle(535, 552, 120, 34);
	iSetColor(255, 255, 255);
	iRectangle(535, 552, 120, 34);
	iText(547, 564, "Explore Town", GLUT_BITMAP_HELVETICA_12);

	iSetColor(170, 45, 45);
	iFilledRectangle(670, 552, 110, 34);
	iSetColor(255, 255, 255);
	iRectangle(670, 552, 110, 34);
	iText(705, 564, "MENU", GLUT_BITMAP_HELVETICA_12);

	// Marketplace Overlay
	if (isMarketOpen) {
		drawMarketplaceUI();
	}
}
#endif // DRAWLEVEL1_H
