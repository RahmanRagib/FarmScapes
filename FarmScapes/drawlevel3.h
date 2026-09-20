#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Fishery States
#define FISH_STATE_WAITING   0
#define FISH_STATE_BITEN     1
#define FISH_STATE_CAUGHT    2
#define FISH_STATE_MISSED    3

// Extern variables from main application
extern int gameState;
extern int playerGold;

// ------------------------------------------------------------
// FISH INVENTORY & PRICING SYSTEM (fish1 to fish6)
// ------------------------------------------------------------
inline int countFish1 = 0;
inline int countFish2 = 0;
inline int countFish3 = 0;
inline int countFish4 = 0;
inline int countFish5 = 0;
inline int countFish6 = 0;

// Default prices (Easily customizable)
inline int sellFish1Price = 10, buyFish1Price = 15;
inline int sellFish2Price = 20, buyFish2Price = 25;
inline int sellFish3Price = 30, buyFish3Price = 35;
inline int sellFish4Price = 45, buyFish4Price = 50;
inline int sellFish5Price = 65, buyFish5Price = 75;
inline int sellFish6Price = 90, buyFish6Price = 100;

inline char fishNames[6][10] = { "fish1", "fish2", "fish3", "fish4", "fish5", "fish6" };

// ------------------------------------------------------------
// GAMEPLAY STATE VARIABLES
// ------------------------------------------------------------
inline int fishingState = FISH_STATE_WAITING;
inline int waitTimer = 0;           // Timer until a fish bites (0 to 15s)
inline int reactionTimer = 0;       // Window to react when red.bmp appears
inline int resultDisplayTimer = 0;  // Display duration for caught/missed states

inline int caughtFishIndex = -1;    // Index 1..6 for fish1..fish6
inline int isFishMarketOpen = 0;

// Reset timer & return to waiting stance
inline void startWaitingForBite() {
	fishingState = FISH_STATE_WAITING;
	// Random wait timer between 10 to 150 ticks (1.0 to 15.0 seconds at 100ms intervals)
	waitTimer = 10 + (rand() % 141);
}

// Initialize Level 3
inline void initLevel3() {
	reactionTimer = 0;
	resultDisplayTimer = 0;
	caughtFishIndex = -1;
	isFishMarketOpen = 0;
	startWaitingForBite();
}

// Increment inventory when fish is caught
inline void addFishToInventory(int fishIdx) {
	switch (fishIdx) {
		case 1: countFish1++; break;
		case 2: countFish2++; break;
		case 3: countFish3++; break;
		case 4: countFish4++; break;
		case 5: countFish5++; break;
		case 6: countFish6++; break;
	}
}

// Update loop called every 100ms via iSetTimer
inline void updateLevel3Logic() {
	if (gameState != 8) return; // 8 = STATE_LEVEL_3

	// 1. Waiting phase (0 to 15 seconds)
	if (fishingState == FISH_STATE_WAITING) {
		if (waitTimer > 0) {
			waitTimer--;
		} else {
			// 70% chance a fish bites, 30% chance nothing bites
			if ((rand() % 100) < 70) {
				fishingState = FISH_STATE_BITEN;
				reactionTimer = 15; // 1.5 second reaction window
			} else {
				fishingState = FISH_STATE_MISSED;
				resultDisplayTimer = 20; // Show "nothing_bit.bmp" for 2 seconds
			}
		}
	}
	// 2. Fish Bite phase (assets/red.bmp is active)
	else if (fishingState == FISH_STATE_BITEN) {
		if (reactionTimer > 0) {
			reactionTimer--;
		} else {
			// Player didn't click in time -> Fish runs away
			fishingState = FISH_STATE_MISSED;
			resultDisplayTimer = 20;
		}
	}
	// 3. Display Result phase (Caught / Missed)
	else if (fishingState == FISH_STATE_CAUGHT || fishingState == FISH_STATE_MISSED) {
		if (resultDisplayTimer > 0) {
			resultDisplayTimer--;
		} else {
			startWaitingForBite();
		}
	}
}

// ------------------------------------------------------------
// MARKET UI (LEVEL 1 STYLE)
// ------------------------------------------------------------
inline void drawFishMarketUI() {
	// Market Overlay Frame
	iSetColor(20, 40, 65);
	iFilledRectangle(100, 70, 600, 440);

	iSetColor(70, 130, 180);
	iRectangle(100, 70, 600, 440);

	char buf[64];
	iSetColor(240, 200, 80);
	sprintf_s(buf, sizeof(buf), "FISHERY MARKET (Gold: $%d)", playerGold);
	iText(260, 470, buf, GLUT_BITMAP_HELVETICA_18);

	int* counts[6] = { &countFish1, &countFish2, &countFish3, &countFish4, &countFish5, &countFish6 };
	int sellPrices[6] = { sellFish1Price, sellFish2Price, sellFish3Price, sellFish4Price, sellFish5Price, sellFish6Price };
	int buyPrices[6]  = { buyFish1Price,  buyFish2Price,  buyFish3Price,  buyFish4Price,  buyFish5Price,  buyFish6Price  };

	// 6 Item Rows matching Level 1 Market positions
	for (int i = 0; i < 6; i++) {
		int rowY = 410 - (i * 55);

		// Item PNG/BMP Icon
		char fishImgPath[64];
		sprintf_s(fishImgPath, sizeof(fishImgPath), "assets/%s.bmp", fishNames[i]);
		iShowBMP2(120, rowY - 5, fishImgPath, 0);

		// Name & Inventory Count
		sprintf_s(buf, sizeof(buf), "%s (Owned: %d)", fishNames[i], *counts[i]);
		iSetColor(255, 255, 255);
		iText(175, rowY + 5, buf, GLUT_BITMAP_HELVETICA_12);

		// SELL BUTTON
		iSetColor(45, 130, 55);
		iFilledRectangle(340, rowY - 2, 80, 24);
		iSetColor(255, 255, 255);
		sprintf_s(buf, sizeof(buf), "SELL ($%d)", sellPrices[i]);
		iText(346, rowY + 4, buf, GLUT_BITMAP_HELVETICA_10);

		// BUY BUTTON
		iSetColor(180, 120, 40);
		iFilledRectangle(440, rowY - 2, 80, 24);
		iSetColor(255, 255, 255);
		sprintf_s(buf, sizeof(buf), "BUY ($%d)", buyPrices[i]);
		iText(448, rowY + 4, buf, GLUT_BITMAP_HELVETICA_10);
	}

	// CLOSE BUTTON
	iSetColor(160, 40, 40);
	iFilledRectangle(600, 90, 80, 30);
	iSetColor(255, 255, 255);
	iText(618, 100, "CLOSE", GLUT_BITMAP_HELVETICA_12);
}

// ------------------------------------------------------------
// MOUSE CLICK ROUTING & GAMEPLAY TRIGGER
// ------------------------------------------------------------
inline void handleLevel3MouseClick(int mx, int my) {
	// 1. If Market is open, handle Market clicks exclusively
	if (isFishMarketOpen) {
		// Close Button
		if (mx >= 600 && mx <= 680 && my >= 90 && my <= 120) {
			isFishMarketOpen = 0;
			return;
		}

		int* counts[6] = { &countFish1, &countFish2, &countFish3, &countFish4, &countFish5, &countFish6 };
		int sellPrices[6] = { sellFish1Price, sellFish2Price, sellFish3Price, sellFish4Price, sellFish5Price, sellFish6Price };
		int buyPrices[6]  = { buyFish1Price,  buyFish2Price,  buyFish3Price,  buyFish4Price,  buyFish5Price,  buyFish6Price  };

		for (int i = 0; i < 6; i++) {
			int rowY = 410 - (i * 55);

			// SELL CLICK
			if (mx >= 340 && mx <= 420 && my >= rowY - 2 && my <= rowY + 22) {
				if (*counts[i] > 0) {
					(*counts[i])--;
					playerGold += sellPrices[i];
				}
				return;
			}

			// BUY CLICK
			if (mx >= 440 && mx <= 520 && my >= rowY - 2 && my <= rowY + 22) {
				if (playerGold >= buyPrices[i]) {
					playerGold -= buyPrices[i];
					(*counts[i])++;
				}
				return;
			}
		}
		return; // Prevent triggering fishing action while clicking in market
	}

	// 2. Ignore gameplay clicks on the Top Header Bar
	if (my >= 540) return;

	// 3. FULL-SCREEN CLICK MECHANIC FOR FISHING
	if (fishingState == FISH_STATE_BITEN) {
		// Successful Reaction! Hook the fish
		fishingState = FISH_STATE_CAUGHT;
		resultDisplayTimer = 25; // Display pulling sequence & fish for 2.5s

		// Catch random fish (index 1 to 6)
		caughtFishIndex = 1 + (rand() % 6);
		addFishToInventory(caughtFishIndex);
	}
	else if (fishingState == FISH_STATE_CAUGHT || fishingState == FISH_STATE_MISSED) {
		// Instant-skip pop-ups on click to start waiting immediately
		startWaitingForBite();
	}
}

// ------------------------------------------------------------
// MAIN RENDER FUNCTION FOR LEVEL 3
// ------------------------------------------------------------
inline void drawLevel3() {
	// Background
	iSetColor(255, 255, 255);
	iShowBMPAlternative(0, 0, "assets/level3_bg.bmp");

	int fishermanX = 350;
	int fishermanY = 280;

	// 1. Render Fisherman Base Sprite
	if (fishingState == FISH_STATE_CAUGHT) {
		// Image 2: Fisherman pulling rod out of water
		iShowBMP2(fishermanX, fishermanY, "assets/fisherman_pulling.bmp", 0);
	} else {
		// Image 1: Fisherman standing with hook in water
		iShowBMP2(fishermanX, fishermanY, "assets/fisherman_idle.bmp", 0);
	}

	// 2. Fishing Line in Water (during waiting/biting)
	if (fishingState == FISH_STATE_WAITING || fishingState == FISH_STATE_BITEN) {
		iSetColor(200, 200, 200);
		iLine(fishermanX + 65, fishermanY + 80, fishermanX + 150, fishermanY - 80);
	}

	// 3. Red Exclamation Mark Pop-up on Fish Bite
	if (fishingState == FISH_STATE_BITEN) {
		iShowBMP2(fishermanX + 20, fishermanY + 95, "assets/red.bmp", 0);
	}

	// 4. Caught Fish Pop-up Image (assets/fish1.bmp to assets/fish6.bmp)
	if (fishingState == FISH_STATE_CAUGHT && caughtFishIndex >= 1 && caughtFishIndex <= 6) {
		char fishPath[64];
		sprintf_s(fishPath, sizeof(fishPath), "assets/fish%d.bmp", caughtFishIndex);
		iShowBMP2(fishermanX + 10, fishermanY + 110, fishPath, 0);
	}

	// 5. "Nothing Bit The Bait" Pop-up Image (200x150 at x=300, y=180)
	if (fishingState == FISH_STATE_MISSED) {
		iShowBMP2(300, 180, "assets/nothing_bit.bmp", 0);
	}

	// 6. Header Bar & HUD Navigation
	iSetColor(40, 40, 40);
	iFilledRectangle(0, 540, 800, 60);

	iSetColor(255, 215, 0);
	char hudStr[64];
	sprintf_s(hudStr, sizeof(hudStr), "Gold: $%d", playerGold);
	iText(10, 562, hudStr, GLUT_BITMAP_HELVETICA_12);

	// Navigation Header Buttons
	iSetColor(45, 130, 180);
	iFilledRectangle(430, 552, 100, 34);
	iSetColor(255, 255, 255);
	iText(452, 564, "MARKET", GLUT_BITMAP_HELVETICA_12);

	iSetColor(40, 160, 120);
	iFilledRectangle(545, 552, 110, 34);
	iSetColor(255, 255, 255);
	iText(557, 564, "Back to Town", GLUT_BITMAP_HELVETICA_10);

	iSetColor(170, 45, 45);
	iFilledRectangle(670, 552, 110, 34);
	iSetColor(255, 255, 255);
	iText(705, 564, "MENU", GLUT_BITMAP_HELVETICA_12);

	// Render Level 1 Style Market Overlay
	if (isFishMarketOpen) {
		drawFishMarketUI();
	}
}

#endif // DRAWLEVEL3_H
