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
int slotFullWarning = 0;
int storyState = 1;
int noSaveFileWarning = 0;

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
		if (noSaveFileWarning)
		{
			// Background color (Button-er moto box color)
			iSetColor(180, 40, 40);
			iFilledRectangle(250, 120, 320, 45);

			// Border color
			iSetColor(255, 255, 255);
			iRectangle(250, 120, 320, 45);

			// Text color
			iSetColor(255, 255, 255);
			iText(270, 135, "THERE IS NO SAVED FILE PREVIOUSLY", GLUT_BITMAP_HELVETICA_12);
		}
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
		if (slotActionMode == 1) iText(221, 491, "SELECT SLOT FOR NEW GAME", GLUT_BITMAP_TIMES_ROMAN_24);
		else if (slotActionMode == 2) iText(251, 491, "SELECT SLOT TO LOAD", GLUT_BITMAP_TIMES_ROMAN_24);
		else if (slotActionMode == 3) iText(231, 491, "SELECT SLOT TO DELETE", GLUT_BITMAP_TIMES_ROMAN_24);

		iSetColor(210, 145, 80);
		if (slotActionMode == 1) iText(220, 490, "SELECT SLOT FOR NEW GAME", GLUT_BITMAP_TIMES_ROMAN_24);
		else if (slotActionMode == 2) iText(250, 490, "SELECT SLOT TO LOAD", GLUT_BITMAP_TIMES_ROMAN_24);
		else if (slotActionMode == 3) iText(230, 490, "SELECT SLOT TO DELETE", GLUT_BITMAP_TIMES_ROMAN_24);

		char slotText[50];

		// SLOT 1
		iSetColor(160, 90, 40);
		iFilledRectangle(290, 400, 220, 45);
		iSetColor(220, 160, 90);
		iRectangle(290, 400, 220, 45);

		if (checkIfSlotExists(1)) {
			sprintf(slotText, "Slot 1 (Gold: %d)", getSlotScore(1));
		}
		else {
			sprintf(slotText, "Slot 1 - Empty");
		}
		iSetColor(40, 20, 10);
		iText(331, 416, slotText, GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(330, 415, slotText, GLUT_BITMAP_HELVETICA_12);

		// SLOT 2
		iSetColor(160, 90, 40);
		iFilledRectangle(290, 330, 220, 45);
		iSetColor(220, 160, 90);
		iRectangle(290, 330, 220, 45);

		if (checkIfSlotExists(2)) {
			sprintf(slotText, "Slot 2 (Gold: %d)", getSlotScore(2));
		}
		else {
			sprintf(slotText, "Slot 2 - Empty");
		}
		iSetColor(40, 20, 10);
		iText(331, 346, slotText, GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(330, 345, slotText, GLUT_BITMAP_HELVETICA_12);

		// SLOT 3
		iSetColor(160, 90, 40);
		iFilledRectangle(290, 260, 220, 45);
		iSetColor(220, 160, 90);
		iRectangle(290, 260, 220, 45);

		if (checkIfSlotExists(3)) {
			sprintf(slotText, "Slot 3 (Gold: %d)", getSlotScore(3));
		}
		else {
			sprintf(slotText, "Slot 3 - Empty");
		}
		iSetColor(40, 20, 10);
		iText(331, 276, slotText, GLUT_BITMAP_HELVETICA_12);
		iSetColor(255, 215, 100);
		iText(330, 275, slotText, GLUT_BITMAP_HELVETICA_12);

		// --- 3 SLOTS FULL WARNING MESSAGE ---
		if (slotFullWarning == 1) {
			iSetColor(255, 0, 0); // Red color for warning
			iText(210, 225, "WARNING: All 3 slots are full! Delete one.", GLUT_BITMAP_HELVETICA_12);
			iSetColor(255, 255, 255); // Reset color
		}

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
void resetGameData()
{
	// Global / General Currency & Inventory
	playerGold = 0;

	// Level 1: Farm Crops & Seeds
	seedRice = 9;
	seedTomato = 0;
	seedBerry = 0;
	cropRiceCount = 0;
	cropTomatoCount = 0;
	cropBerryCount = 0;
	massPlowUnlocked = 0;

	// Farm Grid Reset
	for (int r = 0; r < GRID_ROWS; r++)
	{
		for (int c = 0; c < GRID_COLS; c++)
		{
			farmGrid[r][c].state = CROP_EMPTY;
			farmGrid[r][c].growTimer = 0;
			farmGrid[r][c].cropType = 0;
		}
	}

	// Level 2: Ranch & Animals Initial Setup (2 Hens, 1 Cow, 1 Sheep)
	countFeed = 5; // Starter feed dorkar hote pare
	countEgg = 0;
	countMilk = 0;
	countWool = 0;
	ranchLevelCompleted = false;
	level3Unlocked = 0;

	// Reset & Initialize Hens (2 Hens)
	henCount = 2;
	for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++)
	{
		if (i < 2) {
			hens[i].isAlive = 1;
			hens[i].x = 70 + (i % 2) * 55;
			hens[i].y = 150 + (i / 2) * 55;
			hens[i].fedState = 0;
			hens[i].produceTimer = 0;
			hens[i].hasProduce = 0;
		}
		else {
			hens[i].isAlive = 0;
			hens[i].fedState = 0;
			hens[i].produceTimer = 0;
			hens[i].hasProduce = 0;
		}
	}

	// Reset & Initialize Cows (1 Cow)
	cowCount = 1;
	for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++)
	{
		if (i < 1) {
			cows[i].isAlive = 1;
			cows[i].x = 380;
			cows[i].y = 220;
			cows[i].fedState = 0;
			cows[i].produceTimer = 0;
			cows[i].hasProduce = 0;
		}
		else {
			cows[i].isAlive = 0;
			cows[i].fedState = 0;
			cows[i].produceTimer = 0;
			cows[i].hasProduce = 0;
		}
	}

	// Reset & Initialize Sheep (1 Sheep)
	sheepCount = 1;
	for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++)
	{
		if (i < 1) {
			sheep[i].isAlive = 1;
			sheep[i].x = 580;
			sheep[i].y = 180;
			sheep[i].fedState = 0;
			sheep[i].produceTimer = 0;
			sheep[i].hasProduce = 0;
		}
		else {
			sheep[i].isAlive = 0;
			sheep[i].fedState = 0;
			sheep[i].produceTimer = 0;
			sheep[i].hasProduce = 0;
		}
	}
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
				noSaveFileWarning = 0;
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
			slotFullWarning = 0;

			// NEW GAME CLICKED
			if (mx >= 320 && mx <= 480 && my >= 360 && my <= 405)
			{
				noSaveFileWarning = 0;

				// Bulletproof Serial Slot Assignment: 1 -> 2 -> 3 (Using exact save_slot_X.txt naming)
				FILE *f1 = fopen("save_slot_1.txt", "r");
				FILE *f2 = fopen("save_slot_2.txt", "r");
				FILE *f3 = fopen("save_slot_3.txt", "r");

				if (f1 == NULL) {
					currentSaveSlot = 1;
					if (f1) fclose(f1);
				}
				else if (f2 == NULL) {
					currentSaveSlot = 2;
					if (f2) fclose(f2);
				}
				else if (f3 == NULL) {
					currentSaveSlot = 3;
					if (f3) fclose(f3);
				}
				else {
					// Jodi 3 tai file thake, tokhoni shudhu slot menu-te jabe
					if (f1) fclose(f1);
					if (f2) fclose(f2);
					if (f3) fclose(f3);

					slotFullWarning = 1;
					slotActionMode = 1;
					gameState = STATE_SLOT_MENU;
					return;
				}

				// Clean up pointers safely if they were opened
				if (f1) fclose(f1);
				if (f2) fclose(f2);
				if (f3) fclose(f3);

				// Reset state, save to the found sequential slot, and go to Storyline directly!
				resetGameData();
				saveGameProgress(currentSaveSlot);
				gameState = STATE_STORYLINE;
				return;
			}
			else if (mx >= 320 && mx <= 480 && my >= 300 && my <= 345)
			{
				// LOAD GAME: Check if ANY save file exists (Using exact save_slot_X.txt naming)
				FILE *f1 = fopen("save_slot_1.txt", "r");
				FILE *f2 = fopen("save_slot_2.txt", "r");
				FILE *f3 = fopen("save_slot_3.txt", "r");

				if (f1 == NULL && f2 == NULL && f3 == NULL)
				{
					if (f1) fclose(f1); if (f2) fclose(f2); if (f3) fclose(f3);
					noSaveFileWarning = 1;
					return;
				}
				if (f1) fclose(f1); if (f2) fclose(f2); if (f3) fclose(f3);

				noSaveFileWarning = 0;
				slotActionMode = 2;
				gameState = STATE_SLOT_MENU;
				return;
			}
			else if (mx >= 320 && mx <= 480 && my >= 240 && my <= 285)
			{
				noSaveFileWarning = 0;
				slotActionMode = 3;
				gameState = STATE_SLOT_MENU;
				return;
			}
			else if (mx >= 340 && mx <= 460 && my >= 175 && my <= 215)
			{
				noSaveFileWarning = 0;
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
				slotFullWarning = 0;
				currentSaveSlot = 1;
				handleSlotAction(1);
				return;
			}
			else if (mx >= 300 && mx <= 550 && my >= 330 && my <= 370)
			{
				slotFullWarning = 0;
				currentSaveSlot = 2;
				handleSlotAction(2);
				return;
			}
			else if (mx >= 300 && mx <= 550 && my >= 260 && my <= 300)
			{
				slotFullWarning = 0;
				currentSaveSlot = 3;
				handleSlotAction(3);
				return;
			}
			else if (mx >= 350 && mx <= 450 && my >= 170 && my <= 210)
			{
				slotFullWarning = 0;
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

			// 4. Grid Interactions (Fixed for Rice, Tomato & Berry Planting)
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
						else if (selectedTool == 2) // Plant Tool
						{
							if (t->state == CROP_PLOWED)
							{
								if (seedRice > 0)
								{
									t->state = CROP_SEEDED;
									t->growTimer = 0;
									t->cropType = 0;
									seedRice--;

									// Timer jodi age theke active na thake, tokhoni 20 theke shuru hobe
									if (!batchActive) {
										batchTimer = 20;
										batchActive = 1;
									}
								}
								else if (seedTomato > 0)
								{
									t->state = CROP_SEEDED;
									t->growTimer = 0;
									t->cropType = 1;
									seedTomato--;

									if (!batchActive) {
										batchTimer = 20;
										batchActive = 1;
									}
								}
								else if (seedBerry > 0)
								{
									t->state = CROP_SEEDED;
									t->growTimer = 0;
									t->cropType = 2;
									seedBerry--;

									if (!batchActive) {
										batchTimer = 20;
										batchActive = 1;
									}
								}
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
							// Harvest based on specific crop type
							if (t->state == CROP_READY || t->state == TOMATO_READY || t->state == BERRY_READY)
							{
								if (t->cropType == 0) cropRiceCount++;
								else if (t->cropType == 1) cropTomatoCount++;
								else if (t->cropType == 2) cropBerryCount++;

								t->state = CROP_EMPTY;
								t->cropType = 0;
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
			// Top Navigation Bar clicks (Save, Market, Town, Menu)
			if (my >= 550 && my <= 590)
			{
				if (mx >= 320 && mx <= 420) { saveGameProgress(currentSaveSlot); return; }
				if (mx >= 430 && mx <= 530) { isRanchMarketOpen = !isRanchMarketOpen; return; }
				if (mx >= 545 && mx <= 655) { gameState = STATE_TOWN; return; }
				if (mx >= 670 && mx <= 780) { gameState = STATE_MENU; return; }
			}

			// If Ranch Market is Open
			if (isRanchMarketOpen)
			{
				if (mx >= 580 && mx <= 700 && my >= 70 && my <= 130)
				{
					isRanchMarketOpen = 0;
					return;
				}

				if (mx >= 290 && mx <= 410)
				{
					if (my >= 320 && my <= 375 && countEgg > 0)
					{
						countEgg--;
						playerGold += eggSellPrice;
						return;
					}
					else if (my >= 300 && my <= 340 && countMilk > 0)
					{
						countMilk--;
						playerGold += milkSellPrice;
						return;
					}
					else if (my >= 235 && my <= 290 && countWool > 0)
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

				if (mx >= 580 && mx <= 700)
				{
					// Buy Feed (Expanded hit box)
					if (my >= 355 && my <= 405)
					{
						if (playerGold >= feedBuyPrice)
						{
							playerGold -= feedBuyPrice;
							countFeed++;
						}
						return;
					}
					// Buy Hen
					else if (my >= 305 && my <= 355)
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
					// Buy Cow
					else if (my >= 255 && my <= 305)
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
					// Buy Sheep
					else if (my >= 195 && my <= 255)
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

			// Bottom Toolbar Tool Selection (Feed Tool vs Collect Tool)
			if (my >= 475 && my <= 535)
			{
				if (mx >= 530 && mx <= 660)
				{
					selectedRanchTool = 1; // Feed Tool
					return;
				}
				if (mx >= 661 && mx <= 790)
				{
					selectedRanchTool = 2; // Collect Tool
					return;
				}
			}

			// Direct Animal Clicking Check (Timer Fixed to 30s + Jump Prevention Guard)
			// 1. Hens
			for (int i = 0; i < henCount; i++)
			{
				if (hens[i].isAlive && mx >= hens[i].x - 15 && mx <= hens[i].x + 65 && my >= hens[i].y - 15 && my <= hens[i].y + 65)
				{
					if (selectedRanchTool == 1 && countFeed > 0 && hens[i].fedState == 0)
					{
						countFeed--;
						hens[i].fedState = 1;

						if (!isRanchTimerActive) {
							ranchTimer = 30; // 30 seconds timer
							isRanchTimerActive = 1;
						}
						return;
					}
					else if (selectedRanchTool == 2 && hens[i].hasProduce)
					{
						hens[i].hasProduce = 0;
						hens[i].produceTimer = 0;
						countEgg++;
						return;
					}
				}
			}

			// 2. Cows
			for (int i = 0; i < cowCount; i++)
			{
				if (cows[i].isAlive && mx >= cows[i].x - 15 && mx <= cows[i].x + 65 && my >= cows[i].y - 15 && my <= cows[i].y + 65)
				{
					if (selectedRanchTool == 1 && countFeed > 0 && cows[i].fedState == 0)
					{
						countFeed--;
						cows[i].fedState = 1;

						if (!isRanchTimerActive) {
							ranchTimer = 30;
							isRanchTimerActive = 1;
						}
						return;
					}
					else if (selectedRanchTool == 2 && cows[i].hasProduce)
					{
						cows[i].hasProduce = 0;
						cows[i].produceTimer = 0;
						countMilk++;
						return;
					}
				}
			}

			// 3. Sheep
			for (int i = 0; i < sheepCount; i++)
			{
				if (sheep[i].isAlive && mx >= sheep[i].x - 15 && mx <= sheep[i].x + 65 && my >= sheep[i].y - 15 && my <= sheep[i].y + 65)
				{
					if (selectedRanchTool == 1 && countFeed > 0 && sheep[i].fedState == 0)
					{
						countFeed--;
						sheep[i].fedState = 1;

						if (!isRanchTimerActive) {
							ranchTimer = 30;
							isRanchTimerActive = 1;
						}
						return;
					}
					else if (selectedRanchTool == 2 && sheep[i].hasProduce)
					{
						sheep[i].hasProduce = 0;
						sheep[i].produceTimer = 0;
						countWool++;
						return;
					}
				}
			}
		
			 if (gameState == STATE_LEVEL_3)
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

// ============================================================
// CONTINUOUS GAME LOOP
// ============================================================

void fixedUpdate()
{
	// --- TOWN & LEVEL 1 MOVEMENT ---
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

	// --- LEVEL 2 MOVEMENT ---
	if (gameState == STATE_LEVEL_2 && !isRanchMarketOpen)
	{
		int step = 8;
		if (isKeyPressed('a') || isKeyPressed('A')) moveRanchMan(-step, 0);
		if (isKeyPressed('d') || isKeyPressed('D')) moveRanchMan(step, 0);
	}

	// --- LEVEL 3 MOVEMENT ---
	if (gameState == STATE_LEVEL_3)
	{
		if (isKeyPressed('w') || isKeyPressed('W')) handleLevel3Keyboard('w');
		if (isKeyPressed('s') || isKeyPressed('S')) handleLevel3Keyboard('s');
		if (isKeyPressed('a') || isKeyPressed('A')) handleLevel3Keyboard('a');
		if (isKeyPressed('d') || isKeyPressed('D')) handleLevel3Keyboard('d');
	}

	// --- TOWN NPC & LEVEL INTERACTION ---
	if (gameState == STATE_TOWN)
	{
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
				// 1. NADIRA (Top-Left)
				if (playerX >= 120 && playerX <= 240 && playerY >= 340 && playerY <= 450)
				{
					strcpy(npcName, "Nadira");
					strcpy(dialogueText, "Welcome to the Farm! Press E again to enter Level 1.");
					showDialogue = 1;
				}
				// 2. RAGIB (Bottom-Middle)
				else if (playerX >= 320 && playerX <= 460 && playerY >= 150 && playerY <= 260)
				{
					strcpy(npcName, "Ragib");
					if (level2Unlocked)
					{
						strcpy(dialogueText, "Ready for the Ranch? Press E again to enter Level 2.");
						showDialogue = 1;
					}
					else
					{
						strcpy(dialogueText, "Level 2 is locked! Complete Level 1 first.");
						showDialogue = 1;
					}
				}
				// 3. ANIKA (Top-Right)
				else if (playerX >= 550 && playerX <= 720 && playerY >= 320 && playerY <= 440)
				{
					strcpy(npcName, "Anika");
					if (level3Unlocked)
					{
						strcpy(dialogueText, "Welcome to the Boathouse! Press E again to enter Level 3.");
						showDialogue = 1;
					}
					else
					{
						strcpy(dialogueText, "Level 3 is locked! Complete previous levels first.");
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
	// --- STATE: SETTINGS ---
	if (gameState == STATE_SETTING)
	{
		if (key == 27 || key == 8 || key == 'b' || key == 'B') // ESC, Backspace, or 'b'
		{
			gameState = STATE_MENU;
		}
		return;
	}

	// --- STATE: STORYLINE ---
	if (gameState == STATE_STORYLINE)
	{
		handleStorylineKeyboard(key);
		return;
	}

	// --- STATE: TOWN ---
	if (gameState == STATE_TOWN)
	{
		// Press ESC to return to Menu
		if (key == 27)
		{
			gameState = STATE_MENU;
		}
		return;
	}

	// --- STATE: LOADING LEVEL 3 ---
	if (gameState == STATE_LOADING_LEVEL3)
	{
		initLevel3();
		gameState = STATE_LEVEL_3;
		loadingTimer = 0;
		return;
	}

	// --- STATE: LEVEL 3 ---
	if (gameState == STATE_LEVEL_3)
	{
		handleLevel3Keyboard(key);
		if (key == 27)
		{
			gameState = STATE_TOWN;
		}
		return;
	}

	// --- STATE: LEVEL 2 ---
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
	// --- TOWN MOVEMENT WITH COLLISION CHECKING ---
	if (gameState == STATE_TOWN)
	{
		int step = 10; // Movement speed in pixels

		if (key == GLUT_KEY_LEFT)
		{
			if (canWalk(playerX - step, playerY)) playerX -= step;
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			if (canWalk(playerX + step, playerY)) playerX += step;
		}
		else if (key == GLUT_KEY_UP)
		{
			if (canWalk(playerX, playerY + step)) playerY += step;
		}
		else if (key == GLUT_KEY_DOWN)
		{
			if (canWalk(playerX, playerY - step)) playerY -= step;
		}
		return;
	}

	// --- LEVEL 2 MOVEMENT ---
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