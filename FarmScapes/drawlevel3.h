#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>

// ==========================================
// LEVEL 3 EXTERNAL VARIABLES & ASSETS
// ==========================================

// Static position for the character on the right side of the pier platform
int farmman3X = 530;
int farmman3Y = 190;

#define FARM_MAN_WIDTH 40
#define FARM_MAN_HEIGHT 50

// Structure for Underwater Items (Fish or Garbage)
struct UnderwaterItem {
	int x;
	int y;
	int speedX;
	int speedY;
	int category; // 0 = Fish, 1 = Garbage
	int subType;  // 0 = fish1, 1 = fish2, 2 = fish3, 3 = fish4 (or 0-2 for garbage)
	int isAlive;
};

#define MAX_WATER_ITEMS 7
struct UnderwaterItem waterItems[MAX_WATER_ITEMS];

// Fishing & Catch Mechanics
int fisheryCatchCount = 0;
char fisheryMsg[120] = "Press 'e' or click when items swim near the red hook!";

// ==========================================
// INITIALIZE LEVEL 3
// ==========================================
void initLevel3()
{
	srand(time(NULL));

	fisheryCatchCount = 0;
	strcpy_s(fisheryMsg, sizeof(fisheryMsg), "Press 'e' or click when items swim near the red hook!");

	// Spawn items strictly restricted to the water area on the right side
	for (int i = 0; i < MAX_WATER_ITEMS; i++) {
		waterItems[i].x = 420 + rand() % 280;
		waterItems[i].y = 50 + rand() % 160;

		// Fast zipping speeds for both fish and garbage
		waterItems[i].speedX = (rand() % 2 == 0 ? 1 : -1) * (8 + rand() % 6);
		waterItems[i].speedY = (rand() % 2 == 0 ? 1 : -1) * (7 + rand() % 5);

		// Mix of fish and garbage (e.g., 5 fish types / garbage types)
		if (rand() % 10 < 6) {
			waterItems[i].category = 0; // Fish
			waterItems[i].subType = rand() % 4; // 0 to 3 (fish1 to fish4)
		}
		else {
			waterItems[i].category = 1; // Garbage
			waterItems[i].subType = rand() % 3; // 0 to 2 (garbage1 to garbage3)
		}
		waterItems[i].isAlive = 1;
	}
}

// ==========================================
// DRAW LEVEL 3
// ==========================================
void drawLevel3()
{
	// 1. Background
	iShowBMPAlternative(0, 0, (char*)"assets/fishing_bg.bmp");

	// 2. Draw Fast-Moving Fish & Garbage ONLY in the Right-Side Water Zone
	for (int i = 0; i < MAX_WATER_ITEMS; i++) {
		if (waterItems[i].isAlive) {
			if (waterItems[i].category == 0) {
				char fishPath[32];
				sprintf_s(fishPath, sizeof(fishPath), "assets/fish%d.bmp", waterItems[i].subType + 1);
				iShowBMP2(waterItems[i].x, waterItems[i].y, fishPath, 0);
			}
			else {
				char garbPath[32];
				sprintf_s(garbPath, sizeof(garbPath), "assets/garbage%d.bmp", waterItems[i].subType + 1);
				iShowBMP2(waterItems[i].x, waterItems[i].y, garbPath, 0);
			}
		}
	}

	// 3. Draw Static Character on the Bottom Right Pier
	iShowBMP2(farmman3X, farmman3Y, (char*)"assets/farmman3.bmp", 0);

	// 4. Draw Bold, Red-Colored Hook and Line in the Water
	iSetColor(220, 20, 60); // Crimson Red color for the line and hook
	int hookX = farmman3X + 15;
	int hookY = farmman3Y - 35;

	// Thicker line effect using parallel offset or direct drawing
	iLine(farmman3X + (FARM_MAN_WIDTH / 2), farmman3Y, hookX, hookY);
	iLine(farmman3X + (FARM_MAN_WIDTH / 2) + 1, farmman3Y, hookX + 1, hookY);

	// Draw Bold Red Hook marker / Bait asset
	iFilledCircle(hookX, hookY, 6); // Bold red hook head
	iShowBMP2(hookX - 10, hookY - 14, (char*)"assets/bait.bmp", 0);

	// ==========================================
	// 5. TOP HUD & STATUS BAR
	// ==========================================
	iSetColor(40, 40, 40);
	iFilledRectangle(0, 550, SCREEN_WIDTH, 50);

	iSetColor(255, 255, 255);
	char hudText[100];
	sprintf_s(hudText, sizeof(hudText), "Gold: %d    Fish Caught: %d", playerGold, fisheryCatchCount);
	iText(20, 565, hudText, GLUT_BITMAP_HELVETICA_18);

	// Status Message Display in BLACK font
	iSetColor(0, 0, 0);
	iText(160, 525, fisheryMsg, GLUT_BITMAP_HELVETICA_12);

	// Navigation Buttons
	iSetColor(200, 150, 50);
	iFilledRectangle(545, 552, 110, 34);
	iFilledRectangle(670, 552, 110, 34);

	iSetColor(0, 0, 0);
	iText(560, 562, (char*)"Town", GLUT_BITMAP_HELVETICA_12);
	iText(685, 562, (char*)"Main Menu", GLUT_BITMAP_HELVETICA_12);

	iSetColor(255, 255, 255);
	iText(330, 565, (char*)"Level 3: Fishery", GLUT_BITMAP_HELVETICA_18);
}

// ==========================================
// TIMER UPDATE FUNCTION (Fast Movement)
// ==========================================
void updateFisheryTimer()
{
	if (gameState != STATE_LEVEL_3)
		return;

	for (int i = 0; i < MAX_WATER_ITEMS; i++) {
		if (waterItems[i].isAlive) {
			waterItems[i].x += waterItems[i].speedX;
			waterItems[i].y += waterItems[i].speedY;

			// Keep strictly within right-side water boundaries
			if (waterItems[i].x < 410 || waterItems[i].x > 710) {
				waterItems[i].speedX *= -1;
			}
			if (waterItems[i].y < 30 || waterItems[i].y > 210) {
				waterItems[i].speedY *= -1;
			}
		}
	}
}

// ==========================================
// CATCH & PENALTY LOGIC
// ==========================================
void handleFisheryAction()
{
	int hookX = farmman3X + 15;
	int hookY = farmman3Y - 35;
	int caughtSomething = 0;

	for (int i = 0; i < MAX_WATER_ITEMS; i++) {
		if (waterItems[i].isAlive) {
			int distX = abs(waterItems[i].x - hookX);
			int distY = abs(waterItems[i].y - hookY);

			// Check if item is close enough to the hook
			if (distX < 55 && distY < 55) {
				caughtSomething = 1;

				if (waterItems[i].category == 0) {
					// SUCCESS: Fish caught! Gradual tiered pricing (Fish4 highest down to Fish1)
					int earnedGold = 10;
					if (waterItems[i].subType == 3) earnedGold = 50;       // Fish 4 (Highest)
					else if (waterItems[i].subType == 2) earnedGold = 35; // Fish 3
					else if (waterItems[i].subType == 1) earnedGold = 25; // Fish 2
					else if (waterItems[i].subType == 0) earnedGold = 15; // Fish 1 (Lowest)

					fisheryCatchCount++;
					playerGold += earnedGold;
					sprintf_s(fisheryMsg, sizeof(fisheryMsg), "Caught Fish %d! +%d Gold!", waterItems[i].subType + 1, earnedGold);
				}
				else {
					// PENALTY: Garbage caught! Subtracts gold and loses bait/attempt
					int penalty = 25;
					playerGold -= penalty;
					if (playerGold < 0) playerGold = 0;
					sprintf_s(fisheryMsg, sizeof(fisheryMsg), "Yuck! Caught GARBAGE! -%d Gold & Bait Lost!", penalty);
				}

				// Respawn item in the right-side water
				waterItems[i].x = 420 + rand() % 280;
				waterItems[i].y = 50 + rand() % 160;
				waterItems[i].category = (rand() % 10 < 6) ? 0 : 1;
				waterItems[i].subType = (waterItems[i].category == 0) ? (rand() % 4) : (rand() % 3);
				break;
			}
		}
	}

	if (!caughtSomething) {
		sprintf_s(fisheryMsg, sizeof(fisheryMsg), "Missed! Wait for items to swim near the red hook.");
	}
}

void handleLevel3Keyboard(unsigned char key)
{
	if (gameState != STATE_LEVEL_3)
		return;

	if (key == 'e' || key == 'E') {
		handleFisheryAction();
	}
}

// Compatibility variables for iMain.cpp bindings
int farmman3Speed = 5;
int canWalkLevel3(int x, int y) {
	return 0;
}

#endif