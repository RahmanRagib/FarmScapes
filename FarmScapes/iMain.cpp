#define _CRT_SECURE_NO_WARNINGS

#include "iGraphics.h"
#include "bitmap_loader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")


// ============================================================
// SCREEN
// ============================================================

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 48
#define PLAYER_HEIGHT 48


// ============================================================
// GAME STATES
// ============================================================

#define STATE_MENU 0
#define STATE_LEVEL_1 1
#define STATE_SETTINGS 2
#define STATE_CREDITS 3
#define STATE_LOADING 4
#define STATE_TOWN 5
#define STATE_LEVEL_2 6
#define STATE_LOADING_LEVEL2 7
#define STATE_LEVEL_3 8
#define STATE_LOADING_LEVEL3 9
#define STATE_SAVE_MENU 10

// ============================================================
// GLOBAL GAME VARIABLES
// ============================================================

int gameState = STATE_MENU;

int loadingTimer = 0;

int musicOn = 1;

int eKeyPressedLastFrame = 0;

int currentSaveSlot = 1;


// ============================================================
// TOWN VARIABLES
// ============================================================

int playerX = 360;
int playerY = 270;

int playerSpeed = 8;

int currentSeason = 0;
int seasonTimer = 40;

int showDialogue = 0;

char dialogueText[200] = "";
char npcName[50] = "";

int level2Unlocked = 1;
int level3Unlocked = 1;


// ============================================================
// LEVEL 2 RANCHMAN
// ============================================================

int ranchmanX = 400;
int ranchmanY = 30;
int ranchCollectionTimer = 30;
bool ranchCollectionTimerRunning = false;

bool ranchFailed = false;
int ranchFailedMessageTimer = 0;

bool ranchLevelCompleted = false;
int ranchCompleteMessageTimer = 0;

// ============================================================
// INCLUDE GAME HEADERS
// ============================================================

#include "toggleMusic.h"
#include "menu.h"
#include "settings.h"
#include "credits.h"
#include "loading.h"

#include "updatecropgrowth.h"
#include "drawlevel1.h"

#include "animalgrowth.h"

#include "loading2.h"
#include "drawlevel2.h"
#include "drawTown.h"

#include "drawlevel3.h"

#include "saveSystem.h"


// ============================================================
// LEVEL 2 GOLD
// ============================================================

int playerGold = 0;


// ============================================================
// ANIMAL VARIABLES
// ============================================================

int henCount = 0;
int cowCount = 0;
int sheepCount = 0;

struct Animal hens[MAX_ANIMALS_PER_TYPE];
struct Animal cows[MAX_ANIMALS_PER_TYPE];
struct Animal sheep[MAX_ANIMALS_PER_TYPE];


// ============================================================
// LEVEL 2 INVENTORY
// ============================================================

int countFeed = 5;

int countEgg = 0;
int countMilk = 0;
int countWool = 0;


// ============================================================
// LEVEL 2 PRICES
// ============================================================

int feedBuyPrice = 5;

int eggSellPrice = 15;
int milkSellPrice = 30;
int woolSellPrice = 45;

int henBuyPrice = 30;
int cowBuyPrice = 100;
int sheepBuyPrice = 70;


// ============================================================
// RANCH TOOLS
// ============================================================

int selectedRanchTool = 1;
int isRanchMarketOpen = 0;


// ============================================================
// RANCH TIMER
// ============================================================

int ranchTimer = 0;
int isRanchTimerActive = 0;


// ============================================================
// LEVEL 1 CROP PRICES
// ============================================================

int riceBuyPrice = 5;
int riceSellPrice = 10;

int tomatoBuyPrice = 15;
int tomatoSellPrice = 20;

int berryBuyPrice = 25;
int berrySellPrice = 30;


// ============================================================
// STARTING SEEDS
// ============================================================

int seedRice = 9;
int seedTomato = 0;
int seedBerry = 0;


// ============================================================
// CROP INVENTORY
// ============================================================

int cropRiceCount = 0;
int cropTomatoCount = 0;
int cropBerryCount = 0;


// ============================================================
// LEVEL 1 VARIABLES
// ============================================================

int isMarketOpen = 0;
int massPlowUnlocked = 0;
int showCapWarning = 0;
int selectedTool = 0;
int batchTimer = 0;
int batchActive = 0;
int hasRottenCrop = 0;


// ============================================================
// FARM GRID
// ============================================================

Tile farmGrid[GRID_ROWS][GRID_COLS];


// ============================================================
// BOUNDARY CHECK
// ============================================================

int isWithinBounds(int x, int y)
{
	if (x < 0 || x > SCREEN_WIDTH - PLAYER_WIDTH)
		return 0;

	if (y < 0 || y > SCREEN_HEIGHT - PLAYER_HEIGHT)
		return 0;

	return 1;
}

void updatePlayer()
{
}


// ============================================================
// DRAW
// ============================================================

void iDraw()
{
	iClear();

	if (gameState == STATE_MENU)
		drawMenu();
	else if (gameState == STATE_SAVE_MENU) // <--- ADD THIS BLOCK HERE
	{
		iSetColor(30, 30, 30);
		iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		iSetColor(255, 255, 255);
		iText(310, 500, "SELECT SAVE SLOT", GLUT_BITMAP_HELVETICA_18);

		// Slot 1 Button & Text
		// (If you have button images, you can use iShowImage here instead of background rectangles)
		if (checkIfSlotExists(1))
			iText(340, 418, "SLOT 1: [LOAD / PLAY]", GLUT_BITMAP_HELVETICA_12);
		else
			iText(350, 418, "SLOT 1: [EMPTY - NEW]", GLUT_BITMAP_HELVETICA_12);

		// Slot 2 Button & Text
		if (checkIfSlotExists(2))
			iText(340, 328, "SLOT 2: [LOAD / PLAY]", GLUT_BITMAP_HELVETICA_12);
		else
			iText(350, 328, "SLOT 2: [EMPTY - NEW]", GLUT_BITMAP_HELVETICA_12);

		// Slot 3 Button & Text
		if (checkIfSlotExists(3))
			iText(340, 238, "SLOT 3: [LOAD / PLAY]", GLUT_BITMAP_HELVETICA_12);
		else
			iText(350, 238, "SLOT 3: [EMPTY - NEW]", GLUT_BITMAP_HELVETICA_12);

		// Back Button Text
		iText(365, 143, "MAIN MENU", GLUT_BITMAP_HELVETICA_12);
		iText(200, 75, "Tip: Click a slot to Load/New. Press 'D' + Slot Number to Delete.", GLUT_BITMAP_HELVETICA_10);
	}
	else if (gameState == STATE_LOADING)
		drawLoading();

	else if (gameState == STATE_LOADING_LEVEL2)
		drawLevel2Loading();

	else if (gameState == STATE_TOWN)
		drawTown();

	else if (gameState == STATE_LEVEL_1)
		drawLevel1();

	else if (gameState == STATE_LEVEL_2)
		drawLevel2();

	else if (gameState == STATE_LEVEL_3)
		drawLevel3();

	else if (gameState == STATE_SETTINGS)
		drawSettings();

	else if (gameState == STATE_CREDITS)
		drawCredits();
}


// ============================================================
// MOUSE
// ============================================================

void iMouse(int button, int state, int mx, int my)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;

	if (gameState == STATE_MENU)
	{
		if (mx >= 290 && mx <= 510 && my >= 410 && my <= 480)
		{
			gameState = STATE_SAVE_MENU;
			loadingTimer = 0;
		}
		else if (mx >= 290 && mx <= 510 && my >= 320 && my <= 390)
		{
			gameState = STATE_SETTINGS;
		}
		else if (mx >= 290 && mx <= 510 && my >= 230 && my <= 300)
		{
			gameState = STATE_CREDITS;
		}
		else if (mx >= 290 && mx <= 510 && my >= 140 && my <= 210)
		{
			mciSendString("close bgmusic", NULL, 0, NULL);
			exit(0);
		}
	}
	else if (gameState == STATE_PLAY_CHOICE)
	{
		// 1. New Game Button Click
		if (mx >= 250 && mx <= 550 && my >= 350 && my <= 400)
		{
			slotActionMode = 1; // New Game mode
			gameState = STATE_SLOT_MENU;
		}
		// 2. Load Game Button Click
		else if (mx >= 250 && mx <= 550 && my >= 280 && my <= 330)
		{
			slotActionMode = 2; // Load Game mode
			gameState = STATE_SLOT_MENU;
		}
		// 3. Delete Game Button Click
		else if (mx >= 250 && mx <= 550 && my >= 210 && my <= 260)
		{
			slotActionMode = 3; // Delete Game mode
			gameState = STATE_SLOT_MENU;
		}
		// Back Button Click
		else if (mx >= 325 && mx <= 475 && my >= 140 && my <= 190)
		{
			gameState = STATE_MENU;
		}
	}
	else if (gameState == STATE_SLOT_MENU)
	{
		// Slot 1 Click
		if (mx >= 250 && mx <= 550 && my >= 400 && my <= 460)
		{
			currentSaveSlot = 1;
			handleSlotAction(1);
		}
		// Slot 2 Click
		else if (mx >= 250 && mx <= 550 && my >= 310 && my <= 370)
		{
			currentSaveSlot = 2;
			handleSlotAction(2);
		}
		// Slot 3 Click
		else if (mx >= 250 && mx <= 550 && my >= 220 && my <= 280)
		{
			currentSaveSlot = 3;
			handleSlotAction(3);
		}
		// Back Button Click
		else if (mx >= 325 && mx <= 475 && my >= 130 && my <= 170)
		{
			gameState = STATE_PLAY_CHOICE;
		}
	}
	// ==========================================
	else if (gameState == STATE_SETTINGS)
	{
		if (mx >= 290 && mx <= 510 && my >= 340 && my <= 410)
		{
			toggleMusic();
		}
		else if (mx >= 290 && mx <= 510 && my >= 220 && my <= 290)
		{
			gameState = STATE_MENU;
		}
	}
	else if (gameState == STATE_SETTINGS)
	{
		if (mx >= 290 && mx <= 510 && my >= 340 && my <= 410)
		{
			toggleMusic();
		}
		else if (mx >= 290 && mx <= 510 && my >= 220 && my <= 290)
		{
			gameState = STATE_MENU;
		}
	}
	else if (gameState == STATE_CREDITS)
	{
		if (mx >= 290 && mx <= 510 && my >= 140 && my <= 210)
		{
			gameState = STATE_MENU;
		}
	}
	else if (gameState == STATE_TOWN)
	{
		if (mx >= 670 && mx <= 780 && my >= 20 && my <= 60)
		{
			gameState = STATE_MENU;
			return;
		}
	}
	else if (gameState == STATE_LEVEL_1)
	{
		if (showCapWarning)
			showCapWarning = 0;

		if (mx >= 420 && mx <= 520 && my >= 552 && my <= 586)
		{
			isMarketOpen = !isMarketOpen;
			return;
		}
		if (mx >= 535 && mx <= 655 && my >= 552 && my <= 586)
		{
			gameState = STATE_TOWN;
			return;
		}
		if (mx >= 670 && mx <= 780 && my >= 552 && my <= 586)
		{
			gameState = STATE_MENU;
			return;
		}

		if (isMarketOpen)
		{
			if (mx >= 600 && mx <= 680 && my >= 80 && my <= 110)
			{
				isMarketOpen = 0;
				return;
			}
			if (mx >= 330 && mx <= 395 && my >= 370 && my <= 392 && cropRiceCount > 0)
			{
				cropRiceCount--;
				playerGold += riceSellPrice;
			}
			else if (mx >= 330 && mx <= 395 && my >= 330 && my <= 352 && cropTomatoCount > 0)
			{
				cropTomatoCount--;
				playerGold += tomatoSellPrice;
			}
			else if (mx >= 330 && mx <= 395 && my >= 290 && my <= 312 && cropBerryCount > 0)
			{
				cropBerryCount--;
				playerGold += berrySellPrice;
			}

			if (mx >= 600 && mx <= 665 && my >= 370 && my <= 392 && playerGold >= riceBuyPrice)
			{
				playerGold -= riceBuyPrice;
				seedRice++;
			}
			else if (mx >= 600 && mx <= 665 && my >= 330 && my <= 352 && playerGold >= tomatoBuyPrice)
			{
				playerGold -= tomatoBuyPrice;
				seedTomato++;
			}
			else if (mx >= 600 && mx <= 665 && my >= 290 && my <= 312 && playerGold >= berryBuyPrice)
			{
				playerGold -= berryBuyPrice;
				seedBerry++;
			}

			if (!massPlowUnlocked && mx >= 380 && mx <= 510 && my >= 188 && my <= 214)
			{
				if (playerGold >= 1500)
				{
					playerGold -= 1500;
					massPlowUnlocked = 1;
				}
			}
			return;
		}

		if (massPlowUnlocked && mx >= 90 && mx <= 160 && my >= 28 && my <= 72)
		{
			for (int r = 0; r < GRID_ROWS; r++)
			{
				for (int c = 0; c < GRID_COLS; c++)
				{
					if (farmGrid[r][c].state == CROP_EMPTY || farmGrid[r][c].state == CROP_ROTTEN)
					{
						farmGrid[r][c].state = CROP_PLOWED;
						farmGrid[r][c].growTimer = 0;
					}
				}
			}
			return;
		}

		if (my >= 20 && my <= 80)
		{
			if (mx >= 170 && mx <= 260) selectedTool = 1;
			if (mx >= 290 && mx <= 380) selectedTool = 2;
			if (mx >= 410 && mx <= 500) selectedTool = 3;
			if (mx >= 530 && mx <= 630) selectedTool = 4;
		}

		for (int r = 0; r < GRID_ROWS; r++)
		{
			for (int c = 0; c < GRID_COLS; c++)
			{
				Tile *t = &farmGrid[r][c];

				if (mx >= t->x && mx <= t->x + 80 && my >= t->y && my <= t->y + 80)
				{
					if (selectedTool == 1)
					{
						if (t->state == CROP_EMPTY || t->state == CROP_ROTTEN)
						{
							t->state = CROP_PLOWED;
							t->growTimer = 0;
						}
					}
					else if (selectedTool == 2 && t->state == CROP_PLOWED)
					{
						if (seedBerry > 0)
						{
							seedBerry--;
							t->cropType = 2;
							t->state = BERRY_TREE;
							t->growTimer = 0;
							if (!batchActive) { batchActive = 1; batchTimer = 20; }
						}
						else if (seedTomato > 0)
						{
							seedTomato--;
							t->cropType = 1;
							t->state = CROP_SEEDED;
							t->growTimer = 0;
							if (!batchActive) { batchActive = 1; batchTimer = 20; }
						}
						else if (seedRice > 0)
						{
							seedRice--;
							t->cropType = 0;
							t->state = CROP_SEEDED;
							t->growTimer = 0;
							if (!batchActive) { batchActive = 1; batchTimer = 20; }
						}
					}
					else if (selectedTool == 3)
					{
						if (t->state == CROP_SEEDED || t->state == BERRY_TREE)
						{
							t->state = CROP_WATERED;
							t->growTimer = 0;
						}
					}
					else if (selectedTool == 4)
					{
						if (t->state == CROP_READY)
						{
							if (cropRiceCount < MAX_INVENTORY_CAP) { t->state = CROP_EMPTY; cropRiceCount++; }
							else { showCapWarning = 1; }
						}
						else if (t->state == TOMATO_READY)
						{
							if (cropTomatoCount < MAX_INVENTORY_CAP) { t->state = CROP_EMPTY; cropTomatoCount++; }
							else { showCapWarning = 1; }
						}
						else if (t->state == BERRY_READY)
						{
							if (cropBerryCount < MAX_INVENTORY_CAP) { t->state = CROP_EMPTY; cropBerryCount++; }
							else { showCapWarning = 1; }
						}

						int activeCrops = 0;
						for (int r2 = 0; r2 < GRID_ROWS; r2++)
						{
							for (int c2 = 0; c2 < GRID_COLS; c2++)
							{
								int s = farmGrid[r2][c2].state;
								if (s == CROP_SEEDED || s == CROP_WATERED || s == CROP_READY || s == TOMATO_READY || s == BERRY_TREE || s == BERRY_READY)
								{
									activeCrops++;
								}
							}
						}
						if (activeCrops == 0)
						{
							batchActive = 0; batchTimer = 0; hasRottenCrop = 0;
						}
					}
				}
			}
		}
	}
	else if (gameState == STATE_LEVEL_2)
	{
		if (my >= 552 && my <= 586)
		{
			if (mx >= 430 && mx <= 530) { isRanchMarketOpen = !isRanchMarketOpen; return; }
			if (mx >= 545 && mx <= 655) { gameState = STATE_TOWN; return; }
			if (mx >= 670 && mx <= 780) { gameState = STATE_MENU; return; }
		}

		if (isRanchMarketOpen)
		{
			if (mx >= 600 && mx <= 680 && my >= 90 && my <= 120)
			{
				isRanchMarketOpen = 0;
				return;
			}

			if (mx >= 320 && mx <= 385)
			{
				if (my >= 395 && my <= 417 && countEgg > 0) { countEgg--; playerGold += eggSellPrice; }
				if (my >= 345 && my <= 367 && countMilk > 0) { countMilk--; playerGold += milkSellPrice; }
				if (my >= 295 && my <= 317 && countWool > 0) { countWool--; playerGold += woolSellPrice; }

				if (playerGold >= 200 && !ranchLevelCompleted)
				{
					ranchLevelCompleted = true;
					ranchCompleteMessageTimer = 3;
					level3Unlocked = 1;
				}
			}

			if (mx >= 610 && mx <= 675)
			{
				if (my >= 395 && my <= 417 && playerGold >= feedBuyPrice)
				{
					playerGold -= feedBuyPrice;
					countFeed++;
				}
				else if (my >= 345 && my <= 367 && playerGold >= henBuyPrice)
				{
					for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++)
					{
						if (!hens[i].isAlive)
						{
							playerGold -= henBuyPrice;
							hens[i].isAlive = 1;
							hens[i].x = 70 + (i % 2) * 55;
							hens[i].y = 150 + (i / 2) * 55;
							hens[i].fedState = 0; hens[i].produceTimer = 0; hens[i].hasProduce = 0;
							if (i + 1 > henCount) henCount = i + 1;
							break;
						}
					}
				}
				else if (my >= 295 && my <= 317 && playerGold >= cowBuyPrice)
				{
					for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++)
					{
						if (!cows[i].isAlive)
						{
							playerGold -= cowBuyPrice;
							cows[i].isAlive = 1;
							cows[i].x = 380 + (i % 2) * 60;
							cows[i].y = 220 + (i / 2) * 60;
							cows[i].fedState = 0; cows[i].produceTimer = 0; cows[i].hasProduce = 0;
							if (i + 1 > cowCount) cowCount = i + 1;
							break;
						}
					}
				}
				else if (my >= 245 && my <= 267 && playerGold >= sheepBuyPrice)
				{
					for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++)
					{
						if (!sheep[i].isAlive)
						{
							playerGold -= sheepBuyPrice;
							sheep[i].isAlive = 1;
							sheep[i].x = 580 + (i % 2) * 60;
							sheep[i].y = 180 + (i / 2) * 60;
							sheep[i].fedState = 0; sheep[i].produceTimer = 0; sheep[i].hasProduce = 0;
							if (i + 1 > sheepCount) sheepCount = i + 1;
							break;
						}
					}
				}
			}
			return;
		}

		if (my >= 485 && my <= 521)
		{
			if (mx >= 543 && mx <= 655) { selectedRanchTool = 1; return; }
			if (mx >= 667 && mx <= 779) { selectedRanchTool = 2; return; }
		}

		if (isNearHenPen())
		{
			for (int i = 0; i < henCount; i++)
			{
				if (hens[i].isAlive && mx >= hens[i].x && mx <= hens[i].x + 48 && my >= hens[i].y && my <= hens[i].y + 48)
				{
					if (selectedRanchTool == 1 && countFeed > 0 && hens[i].fedState == 0)
					{
						countFeed--; hens[i].fedState = 1; ranchTimer = 20; isRanchTimerActive = 1;
					}
					else if (selectedRanchTool == 2 && hens[i].hasProduce)
					{
						hens[i].hasProduce = 0; hens[i].produceTimer = 0; countEgg++;
						if (!hasAnyRanchProduce() && !ranchLevelCompleted) ranchCollectionTimerRunning = false;
					}
				}
			}
		}
		else if (isNearCowPen())
		{
			for (int i = 0; i < cowCount; i++)
			{
				if (cows[i].isAlive && mx >= cows[i].x && mx <= cows[i].x + 48 && my >= cows[i].y && my <= cows[i].y + 48)
				{
					if (selectedRanchTool == 1 && countFeed > 0 && cows[i].fedState == 0)
					{
						countFeed--; cows[i].fedState = 1; ranchTimer = 20; isRanchTimerActive = 1;
					}
					else if (selectedRanchTool == 2 && cows[i].hasProduce)
					{
						cows[i].hasProduce = 0; cows[i].produceTimer = 0; countMilk++;
						if (!hasAnyRanchProduce() && !ranchLevelCompleted) ranchCollectionTimerRunning = false;
					}
				}
			}
		}
		else if (isNearSheepPen())
		{
			for (int i = 0; i < sheepCount; i++)
			{
				if (sheep[i].isAlive && mx >= sheep[i].x && mx <= sheep[i].x + 48 && my >= sheep[i].y && my <= sheep[i].y + 48)
				{
					if (selectedRanchTool == 1 && countFeed > 0 && sheep[i].fedState == 0)
					{
						countFeed--; sheep[i].fedState = 1; ranchTimer = 20; isRanchTimerActive = 1;
					}
					else if (selectedRanchTool == 2 && sheep[i].hasProduce)
					{
						sheep[i].hasProduce = 0; sheep[i].produceTimer = 0; countWool++;
						if (!hasAnyRanchProduce() && !ranchLevelCompleted) ranchCollectionTimerRunning = false;
					}
				}
			}
		}
	}
	else if (gameState == STATE_LEVEL_3)
	{
		if (my >= 552 && my <= 586)
		{
			if (mx >= 545 && mx <= 655) { gameState = STATE_TOWN; return; }
			if (mx >= 670 && mx <= 780) { gameState = STATE_MENU; return; }
		}
	}
}

void iMouseMove(int mx, int my) {}
void iPassiveMouseMove(int mx, int my) {}


// ============================================================
// RANCH TIMER
// ============================================================

void updateRanchTimer()
{
	if (gameState != STATE_LEVEL_2 || isRanchMarketOpen)
		return;

	if (hasAnyRanchProduce() && !ranchCollectionTimerRunning)
	{
		ranchCollectionTimer = 30;
		ranchCollectionTimerRunning = true;
	}

	if (ranchCollectionTimerRunning)
	{
		if (ranchCollectionTimer > 0) ranchCollectionTimer--;
		if (ranchCollectionTimer <= 0)
		{
			ranchCollectionTimer = 0;
			ranchCollectionTimerRunning = false;
			ranchFailed = true;
			ranchFailedMessageTimer = 3;
			resetAnimalsAfterFailedCollection();
		}
	}

	if (ranchFailedMessageTimer > 0)
	{
		ranchFailedMessageTimer--;
		if (ranchFailedMessageTimer <= 0) { ranchFailedMessageTimer = 0; ranchFailed = false; }
	}

	if (ranchCompleteMessageTimer > 0)
	{
		ranchCompleteMessageTimer--;
		if (ranchCompleteMessageTimer <= 0) { ranchCompleteMessageTimer = 0; }
	}
}


// ============================================================
// CONTINUOUS GAME LOOP
// ============================================================

void fixedUpdate()
{
	// TOWN + LEVEL 1 MOVEMENT
	if ((gameState == STATE_TOWN && !showDialogue) || gameState == STATE_LEVEL_1)
	{
		if (isKeyPressed('w') || isKeyPressed('W') || isSpecialKeyPressed(GLUT_KEY_UP))
		{
			int nextY = playerY + playerSpeed;
			if (canWalk(playerX, nextY) && isWithinBounds(playerX, nextY)) playerY = nextY;
		}
		if (isKeyPressed('s') || isKeyPressed('S') || isSpecialKeyPressed(GLUT_KEY_DOWN))
		{
			int nextY = playerY - playerSpeed;
			if (canWalk(playerX, nextY) && isWithinBounds(playerX, nextY)) playerY = nextY;
		}
		if (isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT))
		{
			int nextX = playerX - playerSpeed;
			if (canWalk(nextX, playerY) && isWithinBounds(nextX, playerY)) playerX = nextX;
		}
		if (isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT))
		{
			int nextX = playerX + playerSpeed;
			if (canWalk(nextX, playerY) && isWithinBounds(nextX, playerY)) playerX = nextX;
		}
	}

	// LEVEL 2 MOVEMENT
	if (gameState == STATE_LEVEL_2 && !isRanchMarketOpen)
	{
		int step = 8;
		if (isKeyPressed('a') || isKeyPressed('A')) moveRanchMan(-step, 0);
		if (isKeyPressed('d') || isKeyPressed('D')) moveRanchMan(step, 0);
	}

	// LEVEL 3 BOAT MOVEMENT (WASD Smooth Polling)
	if (gameState == STATE_LEVEL_3)
	{
		if (isKeyPressed('w') || isKeyPressed('W')) handleLevel3Keyboard('w');
		if (isKeyPressed('s') || isKeyPressed('S')) handleLevel3Keyboard('s');
		if (isKeyPressed('a') || isKeyPressed('A')) handleLevel3Keyboard('a');
		if (isKeyPressed('d') || isKeyPressed('D')) handleLevel3Keyboard('d');
	}

	// TOWN DIALOGUE / TRANSITION
	if (gameState == STATE_TOWN)
	{
		int eIsDown = isKeyPressed('e') || isKeyPressed('E');

		if (eIsDown && !eKeyPressedLastFrame)
		{
			if (showDialogue)
			{
				showDialogue = 0;
				if (strcmp(npcName, "Nadira") == 0) gameState = STATE_LEVEL_1;
				else if (strcmp(npcName, "Ragib") == 0 && level2Unlocked) gameState = STATE_LOADING_LEVEL2;
				else if (strcmp(npcName, "Anika") == 0 && level3Unlocked)
				{
					initLevel3();
					gameState = STATE_LEVEL_3;
				}
			}
			else
			{
				if (playerX >= 480 && playerX <= 570 && playerY >= 370 && playerY <= 460)
				{
					strcpy(npcName, "Nadira");
					strcpy(dialogueText, "Welcome to the Farm! Press E again to enter Level 1.");
					showDialogue = 1;
				}
				else if (playerX >= 450 && playerX <= 550 && playerY >= 240 && playerY <= 330)
				{
					strcpy(npcName, "Ragib");
					if (playerGold >= 100)
					{
						level2Unlocked = 1;
						strcpy(dialogueText, "You have 100 gold! Press E again to enter Level 2.");
					}
					else strcpy(dialogueText, "Welcome to the Ranch! Earn 100 gold in Level 1 first.");
					showDialogue = 1;
				}
				else if (playerX >= 530 && playerX <= 670 && playerY >= 80 && playerY <= 180)
				{
					strcpy(npcName, "Anika");
					if (level3Unlocked) strcpy(dialogueText, "Entering Fishery... Press E again to start.");
					else strcpy(dialogueText, "Welcome to the Fishery! Clear Level 2 first.");
					showDialogue = 1;
				}
			}
		}
		eKeyPressedLastFrame = eIsDown;
	}
}


// ============================================================
// KEYBOARD
// ============================================================

void iKeyboard(unsigned char key)
{
	if (gameState == STATE_LEVEL_3)
	{
		handleLevel3Keyboard(key);
		 if (key == 27)
		 {
			 gameState = STATE_TOWN;
		 }
		return;
	}

	if (key == '3')
	{
		initLevel3();
		gameState = STATE_LEVEL_3;
	}

	if (gameState == STATE_LEVEL_2)
	{
		if (isRanchMarketOpen)
		{
			if (key == 27) isRanchMarketOpen = 0;
			return;
		}

		if (key == '1') selectedRanchTool = 1;
		if (key == '2') selectedRanchTool = 2;

		if (key == ' ' || key == '\r')
		{
			if (selectedRanchTool == 1) feedAnimalsByRanchMan();
			else if (selectedRanchTool == 2) collectProduceByRanchMan();
		}
	}
}

void iSpecialKeyboard(unsigned char key)
{
	if (gameState == STATE_LEVEL_2 && !isRanchMarketOpen)
	{
		if (key == GLUT_KEY_LEFT) moveRanchMan(-15, 0);
		if (key == GLUT_KEY_RIGHT) moveRanchMan(15, 0);
	}
}

void iAnim()
{
	fixedUpdate();
}

void updateSeasonTimer()
{
	if (seasonTimer > 0) seasonTimer--;
	else
	{
		currentSeason = (currentSeason + 1) % 3;
		seasonTimer = 40;
	}
}

void updateLoading()
{
	if (gameState == STATE_LOADING)
	{
		loadingTimer += 2;
		if (loadingTimer >= 100) { gameState = STATE_LEVEL_1; loadingTimer = 0; }
	}
	else if (gameState == STATE_LOADING_LEVEL2)
	{
		loadingTimer += 2;
		if (loadingTimer >= 100) { gameState = STATE_LEVEL_2; loadingTimer = 0; }
	}
}


// ============================================================
// MAIN
// ============================================================

int main()
{
	initFarmGrid();

	int tileSize = 80;
	int gap = 15;
	int startX = (SCREEN_WIDTH - (GRID_COLS * tileSize + (GRID_COLS - 1) * gap)) / 2;
	int startY = 175;

	for (int r = 0; r < GRID_ROWS; r++)
	{
		for (int c = 0; c < GRID_COLS; c++)
		{
			farmGrid[r][c].x = startX + c * (tileSize + gap);
			farmGrid[r][c].y = startY + r * (tileSize + gap);
		}
	}

	initLevel2();
	initLevel3();
	initAudio();

	iSetTimer(1000, updateCropGrowth);
	iSetTimer(1000, updateAnimalGrowth);
	iSetTimer(1000, updateSeasonTimer);
	iSetTimer(1000, updateRanchTimer);
	iSetTimer(50, updateLoading);
	iSetTimer(20, iAnim);

	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "FarmScapes - 2D Farming Simulator");
	iStart();

	return 0;
}
