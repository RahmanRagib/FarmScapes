#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>

// ==========================================
// LEVEL 3 ASSETS & VARIABLES
// ==========================================

int fishingBgImg;
int farmman3Img; // Updated character asset variable
int baitImg;
int fishImgs[4];
int garbageImgs[3];

// farmman3 Initial Position (Starting on the path, ready to walk straight down the pier)
int farmman3X = 470;
int farmman3Y = 450;
int farmman3Speed = 6;

#define FARM_MAN_WIDTH 40
#define FARM_MAN_HEIGHT 50

// Structure for Underwater Items (Fish or Garbage)
struct UnderwaterItem {
	int x;
	int y;
	int category; // 0 = Fish, 1 = Garbage
	int subType;  // 0-3 for fish, 0-2 for garbage
	int isAlive;
};

#define MAX_WATER_ITEMS 6
struct UnderwaterItem waterItems[MAX_WATER_ITEMS];

// Timer-based Fishing & Challenge Logic
int fishingTimer = 0;
int isLineCast = 0;
int fishBiteReady = 0;
int fisheryCatchCount = 0;
int lastHookedCategory = 0; // 0 = Fish, 1 = Garbage
char fisheryMsg[100] = "Walk straight down the pier and press Space to fish!";
// ==========================================
// BOUNDARY CHECK: Straight Vertical Pier & Road Only
// ==========================================
int canWalkLevel3(int x, int y)
{
	// Vertical column allowing straight movement from the road down the pier
	if (x >= 440 && x <= 520 && y >= 100 && y <= 500)
		return 1;

	return 0;
}


// ==========================================
// INITIALIZE LEVEL 3
// ==========================================
void initLevel3()
{
		srand(time(NULL));

		// Spawn underwater items around the lower part of the screen near the pier end
		for (int i = 0; i < MAX_WATER_ITEMS; i++) {
			waterItems[i].x = 250 + rand() % 350;
			waterItems[i].y = 80 + rand() % 180;

			// 70% chance for fish, 30% chance for garbage
			if (rand() % 10 < 7) {
				waterItems[i].category = 0;
				waterItems[i].subType = rand() % 4;
			}
			else {
				waterItems[i].category = 1;
				waterItems[i].subType = rand() % 3;
			}
			waterItems[i].isAlive = 1;
		}
	}


// ==========================================
// DRAW LEVEL 3
// ==========================================
void drawLevel3()
{
	// 1. Draw Background using iShowBMPAlternative (matching Level 2 format)
	iShowBMPAlternative(0, 0, (char*)"assets/fishing_bg.bmp");

	// 2. Draw Underwater Items (Fish & Garbage) using iShowBMP2 with transparent masking
	for (int i = 0; i < MAX_WATER_ITEMS; i++) {
		if (waterItems[i].isAlive) {
			if (waterItems[i].category == 0) {
				// Fish selection based on subType (0 to 3)
				char fishPath[32];
				sprintf_s(fishPath, sizeof(fishPath), "assets/fish%d.bmp", waterItems[i].subType + 1);
				iShowBMP2(waterItems[i].x, waterItems[i].y, fishPath, 0);
			}
			else {
				// Garbage selection based on subType (0 to 2)
				char garbPath[32];
				sprintf_s(garbPath, sizeof(garbPath), "assets/garbage%d.bmp", waterItems[i].subType + 1);
				iShowBMP2(waterItems[i].x, waterItems[i].y, garbPath, 0);
			}
		}
	}

	// 3. Draw farmman3 on the pier
	iShowBMP2(farmman3X, farmman3Y, (char*)"assets/farmman3.bmp", 0);

	// 4. Draw Fishing Line and Bait Straight In Front (Downward into the water)
	if (isLineCast) {
		iSetColor(240, 240, 240);
		int hookX = farmman3X + (FARM_MAN_WIDTH / 2); // Center of character
		int hookY = farmman3Y - 45;                  // Casts straight down in front

		// Vertical line extending downward from character to bait
		iLine(farmman3X + (FARM_MAN_WIDTH / 2), farmman3Y, hookX, hookY);

		// Draw Bait image at the line end
		iShowBMP2(hookX - 8, hookY - 8, (char*)"assets/bait.bmp", 0);
	}

	// ==========================================
	// 5. TOP HUD & STATUS BAR
	// ==========================================
	iSetColor(40, 40, 40);
	iFilledRectangle(0, 550, SCREEN_WIDTH, 50);

	iSetColor(255, 255, 255);
	char hudText[100];
	sprintf_s(hudText, sizeof(hudText), "Gold: %d    Fish Caught: %d", playerGold, fisheryCatchCount);
	iText(20, 565, hudText, GLUT_BITMAP_HELVETICA_18);

	// Status Message Display
	iSetColor(255, 255, 0);
	iText(220, 530, fisheryMsg, GLUT_BITMAP_HELVETICA_18);

	// Navigation Buttons
	iSetColor(200, 150, 50);
	iFilledRectangle(545, 552, 110, 34);
	iFilledRectangle(670, 552, 110, 34);

	iSetColor(0, 0, 0);
	iText(560, 562, (char*)"Town", GLUT_BITMAP_HELVETICA_12);
	iText(685, 562, (char*)"Main Menu", GLUT_BITMAP_HELVETICA_12);

	// Title
	iSetColor(255, 255, 255);
	iText(330, 565, (char*)"Level 3: Fishery", GLUT_BITMAP_HELVETICA_18);
}

// ==========================================
// TIMER UPDATE FUNCTION
// ==========================================
void updateFisheryTimer()
{
	if (gameState != STATE_LEVEL_3)
		return;

	// Ambient floating movement for underwater items
	for (int i = 0; i < MAX_WATER_ITEMS; i++) {
		if (waterItems[i].isAlive) {
			waterItems[i].x += (rand() % 3 - 1);
			if (waterItems[i].x < 250) waterItems[i].x = 250;
			if (waterItems[i].x > 600) waterItems[i].x = 600;
		}
	}

	// Handle Casting & Bite Timers
	if (isLineCast) {
		fishingTimer--;

		if (fishingTimer == 2) {
			strcpy(fisheryMsg, "Something is tugging the bait... Get ready!");
		}

		if (fishingTimer <= 0) {
			fishBiteReady = 1;
			lastHookedCategory = (rand() % 10 < 7) ? 0 : 1;

			if (lastHookedCategory == 0) {
				strcpy(fisheryMsg, "FISH BITE! Press Space/Enter NOW!");
			}
			else {
				strcpy(fisheryMsg, "HEAVY TUG... Might be trash! Pull carefully?");
			}
		}
	}
}


// ==========================================
// KEYBOARD CONTROLS (Fishing & Penalty Logic)
// ==========================================
void handleLevel3Keyboard(unsigned char key)
{
	if (gameState != STATE_LEVEL_3)
		return;

	if (key == ' ' || key == '\r') {
		// Player must walk to the bottom edge of the vertical pier to fish
		if (farmman3Y > 150) {
			strcpy(fisheryMsg, "Walk straight down to the end of the pier to fish!");
			return;
		}

		if (!isLineCast) {
			isLineCast = 1;
			fishingTimer = 5;
			fishBiteReady = 0;
			strcpy(fisheryMsg, "Line cast forward into the water... Waiting...");
		}
		else if (fishBiteReady) {
			if (lastHookedCategory == 0) {
				// SUCCESS: Caught a Fish
				fisheryCatchCount++;
				playerGold += 35;
				strcpy(fisheryMsg, "Success! Caught a fish! +35 Gold.");
			}
			else {
				// PENALTY: Caught Garbage!
				playerGold -= 15;
				if (playerGold < 0) playerGold = 0;
				strcpy(fisheryMsg, "Yuck! You caught GARBAGE! -15 Gold penalty.");
			}

			// Reset cast state
			isLineCast = 0;
			fishBiteReady = 0;
			fishingTimer = 0;

			// Respawn item
			for (int i = 0; i < MAX_WATER_ITEMS; i++) {
				if (!waterItems[i].isAlive || rand() % 2 == 0) {
					waterItems[i].isAlive = 1;
					waterItems[i].x = 250 + rand() % 350;
					waterItems[i].category = (rand() % 10 < 7) ? 0 : 1;
					break;
				}
			}
		}
		else {
			strcpy(fisheryMsg, "Too early! Pulled before the bite.");
			isLineCast = 0;
			fishingTimer = 0;
			fishBiteReady = 0;
		}
	}
}

#endif