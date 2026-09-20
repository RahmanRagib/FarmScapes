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
#define STATE_SETTING 2
#define STATE_CREDITS 3
#define STATE_LOADING 4
#define STATE_TOWN 5
#define STATE_LEVEL_2 6
#define STATE_LOADING_LEVEL2 7
#define STATE_LEVEL_3 8
#define STATE_LOADING_LEVEL3 9
#define STATE_PLAY_CHOICE 10
#define STATE_SLOT_MENU 11
#define STATE_STORYLINE 12

// ============================================================
// GLOBAL GAME VARIABLES
// ============================================================

int gameState = STATE_MENU;
int loadingTimer = 0;
int musicOn = 1;
int eKeyPressedLastFrame = 0;
int currentSaveSlot = 1;
int slotActionMode = 0;
int slotMenuBgImage;
int storyState = 1;

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
#include "loading3.h"
#include "drawlevel2.h"
#include "drawTown.h"

#include "drawlevel3.h"
#include "saveSystem.h"
#include "storyline.h"

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

	if (gameState == STATE_STORYLINE) {
		drawStoryline();
	}
	else if (gameState == STATE_MENU)
		drawMenu();
	else if (gameState == STATE_PLAY_CHOICE)
	{
		// 1. Draw Background Image
		iSetColor(255, 255, 255);
		iShowImage(0, 0, 800, 600, slotMenuBgImage);

		// BUTTON 1: NEW GAME
		iSetColor(160, 90, 40);
		iFilledRectangle(320, 360, 160, 45);
		iSetColor(220, 160, 90);
		iRectangle(320, 360, 160, 45);

		iSetColor(40, 20, 10);
		iText(371, 377, "NEW GAME", GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(370, 376, "NEW GAME", GLUT_BITMAP_HELVETICA_12);

		// BUTTON 2: LOAD GAME
		iSetColor(160, 90, 40);
		iFilledRectangle(320, 300, 160, 45);
		iSetColor(220, 160, 90);
		iRectangle(320, 300, 160, 45);

		iSetColor(40, 20, 10);
		iText(369, 317, "LOAD GAME", GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(368, 316, "LOAD GAME", GLUT_BITMAP_HELVETICA_12);

		// BUTTON 3: DELETE GAME
		iSetColor(160, 90, 40);
		iFilledRectangle(320, 240, 160, 45);
		iSetColor(220, 160, 90);
		iRectangle(320, 240, 160, 45);

		iSetColor(40, 20, 10);
		iText(363, 257, "DELETE GAME", GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(362, 256, "DELETE GAME", GLUT_BITMAP_HELVETICA_12);

		// BUTTON 4: MAIN MENU
		iSetColor(140, 75, 30);
		iFilledRectangle(340, 175, 120, 40);
		iSetColor(200, 140, 75);
		iRectangle(340, 175, 120, 40);

		iSetColor(40, 20, 10);
		iText(369, 190, "MAIN MENU", GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(368, 189, "MAIN MENU", GLUT_BITMAP_HELVETICA_12);
	}
	else if (gameState == STATE_SLOT_MENU)
	{
		// Draw Background Image
		iSetColor(255, 255, 255);
		iShowImage(0, 0, 800, 600, slotMenuBgImage);

		// TOP TITLE
		iSetColor(40, 20, 10);
		if (slotActionMode == 2) iText(251, 491, "SELECT SLOT TO LOAD", GLUT_BITMAP_TIMES_ROMAN_24);
		else if (slotActionMode == 3) iText(231, 491, "SELECT SLOT TO DELETE", GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(210, 145, 80);
		if (slotActionMode == 2) iText(250, 490, "SELECT SLOT TO LOAD", GLUT_BITMAP_TIMES_ROMAN_24);
		else if (slotActionMode == 3) iText(230, 490, "SELECT SLOT TO DELETE", GLUT_BITMAP_TIMES_ROMAN_24);

		char slotText[50];

		// SLOT 1
		iSetColor(160, 90, 40);
		iFilledRectangle(290, 400, 220, 45);
		iSetColor(220, 160, 90);
		iRectangle(290, 400, 220, 45);

		sprintf(slotText, "RESUME SLOT 1 (Gold: %d)", getSlotScore(1));
		iSetColor(40, 20, 10);
		iText(331, 416, slotText, GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(330, 415, slotText, GLUT_BITMAP_HELVETICA_12);

		// SLOT 2
		iSetColor(160, 90, 40);
		iFilledRectangle(290, 330, 220, 45);
		iSetColor(220, 160, 90);
		iRectangle(290, 330, 220, 45);

		sprintf(slotText, "RESUME SLOT 2 (Gold: %d)", getSlotScore(2));
		iSetColor(40, 20, 10);
		iText(331, 346, slotText, GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(330, 345, slotText, GLUT_BITMAP_HELVETICA_12);

		// SLOT 3
		iSetColor(160, 90, 40);
		iFilledRectangle(290, 260, 220, 45);
		iSetColor(220, 160, 90);
		iRectangle(290, 260, 220, 45);

		sprintf(slotText, "RESUME SLOT 3 (Gold: %d)", getSlotScore(3));
		iSetColor(40, 20, 10);
		iText(331, 276, slotText, GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(330, 275, slotText, GLUT_BITMAP_HELVETICA_12);

		// BACK BUTTON
		iSetColor(140, 75, 30);
		iFilledRectangle(340, 170, 120, 40);
		iSetColor(200, 140, 75);
		iRectangle(340, 170, 120, 40);

		iSetColor(40, 20, 10);
		iText(386, 186, "BACK", GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(385, 185, "BACK", GLUT_BITMAP_HELVETICA_12);
	}
	else if (gameState == STATE_LOADING)
		drawLoading();

	else if (gameState == STATE_LOADING_LEVEL2)
		drawLevel2Loading();

	else if (gameState == STATE_LOADING_LEVEL3)
		drawLevel3Loading();

	else if (gameState == STATE_TOWN)
		drawTown();

	else if (gameState == STATE_LEVEL_1)
		drawLevel1();

	else if (gameState == STATE_LEVEL_2)
		drawLevel2();

	else if (gameState == STATE_LEVEL_3)
		drawLevel3();

	else if (gameState == STATE_SETTING)
		drawSettings();

	else if (gameState == STATE_CREDITS)
		drawCredits();
}

// ============================================================
// MOUSE
// ============================================================

void iMouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		// ==========================================
		// MAIN MENU
		// ==========================================
		if (gameState == STATE_MENU)
		{
			if (mx >= 290 && mx <= 510 && my >= 410 && my <= 480)
			{
				gameState = STATE_PLAY_CHOICE;
				loadingTimer = 0;
				return;
			}
			else if (mx >= 290 && mx <= 510 && my >= 320 && my <= 390)
			{
				gameState = STATE_SETTING;
				return;
			}
			else if (mx >= 290 && mx <= 510 && my >= 230 && my <= 300)
			{
				gameState = STATE_CREDITS;
				return;
			}
			else if (mx >= 290 && mx <= 510 && my >= 140 && my <= 210)
			{
				mciSendString("close bgmusic", NULL, 0, NULL);
				exit(0);
			}
		}

		// ==========================================
		// PLAY CHOICE MENU (NEW / LOAD / DELETE)
		// ==========================================
		else if (gameState == STATE_PLAY_CHOICE)
		{
			if (mx >= 320 && mx <= 480 && my >= 360 && my <= 405)
			{
				slotActionMode = 1;
				gameState = STATE_SLOT_MENU;
				return;
			}
			else if (mx >= 320 && mx <= 480 && my >= 300 && my <= 345)
			{
				slotActionMode = 2;
				gameState = STATE_SLOT_MENU;
				return;
			}
			else if (mx >= 320 && mx <= 480 && my >= 240 && my <= 285)
			{
				slotActionMode = 3;
				gameState = STATE_SLOT_MENU;
				return;
			}
			else if (mx >= 340 && mx <= 460 && my >= 175 && my <= 215)
			{
				gameState = STATE_MENU;
				return;
			}
		}

		// ==========================================
		// SLOT MENU
		// ==========================================
		else if (gameState == STATE_SLOT_MENU)
		{
			if (mx >= 300 && mx <= 550 && my >= 400 && my <= 440)
			{
				currentSaveSlot = 1;
				handleSlotAction(1);
				return;
			}
			else if (mx >= 300 && mx <= 550 && my >= 330 && my <= 370)
			{
				currentSaveSlot = 2;
				handleSlotAction(2);
				return;
			}
			else if (mx >= 300 && mx <= 550 && my >= 260 && my <= 300)
			{
				currentSaveSlot = 3;
				handleSlotAction(3);
				return;
			}
			else if (mx >= 350 && mx <= 450 && my >= 170 && my <= 210)
			{
				gameState = STATE_PLAY_CHOICE;
				return;
			}
		}

		// ==========================================
		// SETTINGS MENU
		// ==========================================
		else if (gameState == STATE_SETTING)
		{
			if (mx >= 290 && mx <= 510 && my >= 340 && my <= 410)
			{
				toggleMusic();
				return;
			}
			else if (mx >= 290 && mx <= 510 && my >= 220 && my <= 290)
			{
				gameState = STATE_MENU;
				return;
			}
		}

		// ==========================================
		// CREDITS MENU
		// ==========================================
		else if (gameState == STATE_CREDITS)
		{
			if (mx >= 290 && mx <= 510 && my >= 140 && my <= 210)
			{
				gameState = STATE_MENU;
				return;
			}
		}

		// ==========================================
		// LOADING LEVEL 3
		// ==========================================
		else if (gameState == STATE_LOADING_LEVEL3)
		{
			initLevel3();
			gameState = STATE_LEVEL_3;
			loadingTimer = 0;
			return;
		}

		// ==========================================
		// TOWN
		// ==========================================
		else if (gameState == STATE_TOWN)
		{
			if (mx >= 670 && mx <= 780 && my >= 20 && my <= 60)
			{
				gameState = STATE_MENU;
				return;
			}
		}

		// ==========================================
		// LEVEL 1
		// ==========================================
		else if (gameState == STATE_LEVEL_1)
		{
			if (showCapWarning)
				showCapWarning = 0;

			// 1. If Market is Open
			if (isMarketOpen)
			{
				if (mx >= 600 && mx <= 680 && my >= 80 && my <= 110)
				{
					isMarketOpen = 0;
					return;
				}

				// Sell crops
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

				// Buy seeds
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

				// Buy Mass Plow upgrade
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

			// 2. Top Navigation Bar
			if (mx >= 340 && mx <= 410 && my >= 552 && my <= 586)
			{
				saveGameProgress(currentSaveSlot);
				return;
			}
			else if (mx >= 420 && mx <= 520 && my >= 552 && my <= 586)
			{
				isMarketOpen = 1;
				return;
			}
			else if (mx >= 535 && mx <= 655 && my >= 552 && my <= 586)
			{
				gameState = STATE_TOWN;
				return;
			}
			else if (mx >= 670 && mx <= 780 && my >= 552 && my <= 586)
			{
				gameState = STATE_PLAY_CHOICE;
				return;
			}

			// 3. Toolbar Tools
			if (massPlowUnlocked && mx >= 90 && mx <= 160 && my >= 28 && my <= 72)
			{
				for (int r = 0; r < GRID_ROWS; r++)
				{
					for (int c = 0; c < GRID_COLS; c++)
					{
						if (farmGrid[r][c].state == CROP_EMPTY)
						{
							farmGrid[r][c].state = CROP_PLOWED;
						}
					}
				}
				return;
			}
			else if (mx >= 170 && mx <= 260 && my >= 28 && my <= 72)
			{
				selectedTool = 1; // Plow
				return;
			}
			else if (mx >= 290 && mx <= 380 && my >= 28 && my <= 72)
			{
				selectedTool = 2; // Plant
				return;
			}
			else if (mx >= 410 && mx <= 500 && my >= 28 && my <= 72)
			{
				selectedTool = 3; // Water
				return;
			}
			else if (mx >= 530 && mx <= 630 && my >= 28 && my <= 72)
			{
				selectedTool = 4; // Harvest
				return;
			}

			// 4. Grid Interactions
			for (int r = 0; r < GRID_ROWS; r++)
			{
				for (int c = 0; c < GRID_COLS; c++)
				{
					Tile *t = &farmGrid[r][c];

					if (mx >= t->x && mx <= t->x + 70 && my >= t->y && my <= t->y + 70)
					{
						if (selectedTool == 1)
						{
							if (t->state == CROP_EMPTY || t->state == CROP_ROTTEN)
							{
								t->state = CROP_PLOWED;
								t->growTimer = 0;
							}
						}
						else if (selectedTool == 2)
						{
							if (t->state == CROP_PLOWED)
							{
								t->state = CROP_SEEDED;
								t->growTimer = 0;
								t->cropType = 0;
							}
						}
						else if (selectedTool == 3)
						{
							if (t->state == CROP_SEEDED)
							{
								t->state = CROP_WATERED;
								t->growTimer = 0;
							}
						}
						else if (selectedTool == 4)
						{
							if (t->state == CROP_READY || t->state == TOMATO_READY || t->state == BERRY_READY)
							{
								t->state = CROP_EMPTY;
								cropRiceCount++;
								playerGold += 15;
							}
						}
					}
				}
			}
		}

		// ==========================================
		// LEVEL 2
		// ==========================================
		else if (gameState == STATE_LEVEL_2)
		{
			if (my >= 550 && my <= 590)
			{
				if (mx >= 320 && mx <= 420) { saveGameProgress(currentSaveSlot); return; }
				if (mx >= 430 && mx <= 530) { isRanchMarketOpen = !isRanchMarketOpen; return; }
				if (mx >= 545 && mx <= 655) { gameState = STATE_TOWN; return; }
				if (mx >= 670 && mx <= 780) { gameState = STATE_MENU; return; }
			}

			if (isRanchMarketOpen)
			{
				if (mx >= 600 && mx <= 680 && my >= 80 && my <= 120)
				{
					isRanchMarketOpen = 0;
					return;
				}

				if (mx >= 310 && mx <= 390)
				{
					if (my >= 330 && my <= 365 && countEgg > 0)
					{
						countEgg--;
						playerGold += eggSellPrice;
						return;
					}
					else if (my >= 315 && my <= 330 && countMilk > 0)
					{
						countMilk--;
						playerGold += milkSellPrice;
						return;
					}
					else if (my >= 245 && my <= 280 && countWool > 0)
					{
						countWool--;
						playerGold += woolSellPrice;
						return;
					}

					if (playerGold >= 200 && !ranchLevelCompleted)
					{
						ranchLevelCompleted = true;
						ranchCompleteMessageTimer = 3;
						level3Unlocked = 1;
					}
				}

				if (mx >= 600 && mx <= 680)
				{
					if (my >= 370 && my <= 390)
					{
						if (playerGold >= feedBuyPrice)
						{
							playerGold -= feedBuyPrice;
							countFeed++;
						}
						return;
					}
					else if (my >= 320 && my <= 340)
					{
						if (playerGold >= henBuyPrice)
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
						return;
					}
					else if (my >= 270 && my <= 290)
					{
						if (playerGold >= cowBuyPrice)
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
						return;
					}
					else if (my >= 210 && my <= 240)
					{
						if (playerGold >= sheepBuyPrice)
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
						return;
					}
				}
				return;
			}

			if (my >= 475 && my <= 535)
			{
				if (mx >= 530 && mx <= 660)
				{
					selectedRanchTool = 1;
					return;
				}
				if (mx >= 661 && mx <= 790)
				{
					selectedRanchTool = 2;
					return;
				}
			}

			if (isNearHenPen())
			{
				for (int i = 0; i < henCount; i++)
				{
					if (hens[i].isAlive && mx >= hens[i].x - 10 && mx <= hens[i].x + 58 && my >= hens[i].y - 10 && my <= hens[i].y + 58)
					{
						if (selectedRanchTool == 1 && countFeed > 0 && hens[i].fedState == 0)
						{
							countFeed--;
							hens[i].fedState = 1;
							ranchTimer = 20;
							isRanchTimerActive = 1;
							return;
						}
						else if (selectedRanchTool == 2 && hens[i].hasProduce)
						{
							hens[i].hasProduce = 0;
							hens[i].produceTimer = 0;
							countEgg++;
							if (!hasAnyRanchProduce() && !ranchLevelCompleted) ranchCollectionTimerRunning = false;
							return;
						}
					}
				}
			}
			else if (isNearCowPen())
			{
				for (int i = 0; i < cowCount; i++)
				{
					if (cows[i].isAlive && mx >= cows[i].x - 10 && mx <= cows[i].x + 58 && my >= cows[i].y - 10 && my <= cows[i].y + 58)
					{
						if (selectedRanchTool == 1 && countFeed > 0 && cows[i].fedState == 0)
						{
							countFeed--;
							cows[i].fedState = 1;
							ranchTimer = 20;
							isRanchTimerActive = 1;
							return;
						}
						else if (selectedRanchTool == 2 && cows[i].hasProduce)
						{
							cows[i].hasProduce = 0;
							cows[i].produceTimer = 0;
							countMilk++;
							if (!hasAnyRanchProduce() && !ranchLevelCompleted) ranchCollectionTimerRunning = false;
							return;
						}
					}
				}
			}
			else if (isNearSheepPen())
			{
				for (int i = 0; i < sheepCount; i++)
				{
					if (sheep[i].isAlive && mx >= sheep[i].x - 10 && mx <= sheep[i].x + 58 && my >= sheep[i].y - 10 && my <= sheep[i].y + 58)
					{
						if (selectedRanchTool == 1 && countFeed > 0 && sheep[i].fedState == 0)
						{
							countFeed--;
							sheep[i].fedState = 1;
							ranchTimer = 20;
							isRanchTimerActive = 1;
							return;
						}
						else if (selectedRanchTool == 2 && sheep[i].hasProduce)
						{
							sheep[i].hasProduce = 0;
							sheep[i].produceTimer = 0;
							countWool++;
							if (!hasAnyRanchProduce() && !ranchLevelCompleted) ranchCollectionTimerRunning = false;
							return;
						}
					}
				}
			}
		}

		// ==========================================
		// LEVEL 3 (FISHING)
		// ==========================================
		else if (gameState == STATE_LEVEL_3)
		{
			if (my >= 550 && my <= 590)
			{
				if (mx >= 320 && mx <= 420)
				{
					saveGameProgress(currentSaveSlot);
					return;
				}
				else if (mx >= 430 && mx <= 530)
				{
					isFishMarketOpen = !isFishMarketOpen;
					return;
				}
				else if (mx >= 545 && mx <= 655)
				{
					gameState = STATE_TOWN;
					return;
				}
				else if (mx >= 670 && mx <= 780)
				{
					gameState = STATE_MENU;
					return;
				}
			}

			handleLevel3MouseClick(mx, my);
			return;
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

	if (gameState == STATE_LEVEL_2 && !isRanchMarketOpen)
	{
		int step = 8;
		if (isKeyPressed('a') || isKeyPressed('A')) moveRanchMan(-step, 0);
		if (isKeyPressed('d') || isKeyPressed('D')) moveRanchMan(step, 0);
	}

	if (gameState == STATE_LEVEL_3)
	{
		if (isKeyPressed('w') || isKeyPressed('W')) handleLevel3Keyboard('w');
		if (isKeyPressed('s') || isKeyPressed('S')) handleLevel3Keyboard('s');
		if (isKeyPressed('a') || isKeyPressed('A')) handleLevel3Keyboard('a');
		if (isKeyPressed('d') || isKeyPressed('D')) handleLevel3Keyboard('d');
	}

	if (gameState == STATE_TOWN)
	{
		// Check for 'E'/'e' or Enter key ('\r')
		int eIsDown = isKeyPressed('e') || isKeyPressed('E') || isKeyPressed('\r');

		if (eIsDown && !eKeyPressedLastFrame)
		{
			if (showDialogue)
			{
				showDialogue = 0;
				if (strcmp(npcName, "Nadira") == 0)
				{
					gameState = STATE_LEVEL_1;
				}
				else if (strcmp(npcName, "Ragib") == 0 && level2Unlocked)
				{
					gameState = STATE_LOADING_LEVEL2;
				}
				else if (strcmp(npcName, "Anika") == 0 && level3Unlocked)
				{
					loadingTimer = 0;
					gameState = STATE_LOADING_LEVEL3;
				}
			}
			else
			{
				// Level 1: Nadira
				if (playerX >= 480 && playerX <= 570 && playerY >= 370 && playerY <= 460)
				{
					strcpy(npcName, "Nadira");
					strcpy(dialogueText, "Welcome to the Farm! Press E again to enter Level 1.");
					showDialogue = 1;
				}
				// Level 2: Ragib
				else if (playerX >= 450 && playerX <= 550 && playerY >= 240 && playerY <= 330)
				{
					strcpy(npcName, "Ragib");
					if (playerGold >= 0)
					{
						level2Unlocked = 1;
						strcpy(dialogueText, "You have unlocked Level 2! Press E again to enter.");
						showDialogue = 1;
					}
				}
				// Level 3: Anika (Added missing interaction bounds)
				else if (playerX >= 200 && playerX <= 320 && playerY >= 240 && playerY <= 330)
				{
					strcpy(npcName, "Anika");
					if (level3Unlocked)
					{
						strcpy(dialogueText, "Welcome to the Fishing Spot! Press E again to enter Level 3.");
						showDialogue = 1;
					}
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
	if (gameState == STATE_SETTING)
	{
		if (key == 27 || key == 8 || key == 'b' || key == 'B') // ESC, Backspace, or 'b'
		{
			gameState = STATE_MENU;
		}
		return;
	}

	if (gameState == STATE_STORYLINE)
	{
		handleStorylineKeyboard(key);
		return;
	}

	if (gameState == STATE_LOADING_LEVEL3)
	{
		initLevel3();
		gameState = STATE_LEVEL_3;
		loadingTimer = 0;
		return;
	}

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
		loadingTimer = 0;
		gameState = STATE_LOADING_LEVEL3;
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
	else if (gameState == STATE_LOADING_LEVEL3)
	{
		loadingTimer += 2;
		if (loadingTimer >= 100) { initLevel3(); gameState = STATE_LEVEL_3; loadingTimer = 0; }
	}
}

void gameTimerTick() {
	if (gameState == STATE_STORYLINE) {
		updateStoryline();
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

	// Set Timers
	iSetTimer(1000, updateCropGrowth);
	iSetTimer(1000, updateAnimalGrowth);
	iSetTimer(1000, updateSeasonTimer);
	iSetTimer(1000, updateRanchTimer);
	iSetTimer(100, updateLevel3Logic);
	iSetTimer(50, updateLoading);
	iSetTimer(33, gameTimerTick);
	iSetTimer(20, iAnim);

	// MUST BE CALLED BEFORE LOADING ANY IMAGES
	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "FarmScapes - 2D Farming Simulator");

	// Load images ONLY AFTER iInitialize creates the OpenGL window
	slotMenuBgImage = iLoadImage("assets/loadscreen.bmp");
	initStorylineAssets();
	printf("DEBUG: Loaded slotMenuBgImage ID = %d\n", slotMenuBgImage);

	iStart();

	return 0;
}