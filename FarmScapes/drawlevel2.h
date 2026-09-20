#ifndef DRAWLEVEL2_H
#define DRAWLEVEL2_H

#include <stdio.h>
#include <string.h>
#include "animalgrowth.h"

// ============================================================
// LEVEL 2 EXTERNAL VARIABLES
// ============================================================

extern int gameState;
extern int playerGold;

extern int ranchTimer;
extern int isRanchTimerActive;

extern int countFeed;
extern int countEgg, countMilk, countWool;

extern int feedBuyPrice;
extern int eggSellPrice, milkSellPrice, woolSellPrice;

extern int henBuyPrice, cowBuyPrice, sheepBuyPrice;

extern int selectedRanchTool;     // 1 = Feed, 2 = Collect
extern int isRanchMarketOpen;

extern int ranchmanX;
extern int ranchmanY;

extern int ranchCollectionTimer;
extern bool ranchCollectionTimerRunning;
extern bool ranchFailed;
extern int ranchFailedMessageTimer;
extern bool ranchLevelCompleted;
extern int ranchCompleteMessageTimer;

// ============================================================
// PEN PROXIMITY CHECKERS
// ============================================================

inline int isNearHenPen()
{
	return (ranchmanX >= 80 && ranchmanX <= 220);
}

inline int isNearCowPen()
{
	return (ranchmanX >= 280 && ranchmanX <= 450);
}

inline int isNearSheepPen()
{
	return (ranchmanX >= 550 && ranchmanX <= 720);
}


// ============================================================
// ROAD CONSTRAINT
// ============================================================

inline int isValidRoad(int x)
{
	// Ranchman can move horizontally across the bottom road
	if (x >= 20 && x <= 740)
		return 1;

	return 0;
}


// ============================================================
// MOVE RANCH MAN
// ============================================================

inline void moveRanchMan(int dx, int dy)
{
	int newX = ranchmanX + dx;
	int newY = ranchmanY + dy;

	// Horizontal movement
	if (isValidRoad(newX))
		ranchmanX = newX;

	// Vertical movement is intentionally restricted.
	// ranchmanY remains fixed.
}


// ============================================================
// COLLECTION TIMER HELPERS
// ============================================================

inline int hasAnyRanchProduce()
{
	for (int i = 0; i < henCount; i++)
	{
		if (hens[i].isAlive && hens[i].hasProduce)
			return 1;
	}

	for (int i = 0; i < cowCount; i++)
	{
		if (cows[i].isAlive && cows[i].hasProduce)
			return 1;
	}

	for (int i = 0; i < sheepCount; i++)
	{
		if (sheep[i].isAlive && sheep[i].hasProduce)
			return 1;
	}

	return 0;
}

inline void resetAnimalsAfterFailedCollection()
{
	for (int i = 0; i < henCount; i++)
	{
		if (hens[i].isAlive)
		{
			hens[i].fedState = 0;
			hens[i].hasProduce = 0;
			hens[i].produceTimer = 0;
		}
	}

	for (int i = 0; i < cowCount; i++)
	{
		if (cows[i].isAlive)
		{
			cows[i].fedState = 0;
			cows[i].hasProduce = 0;
			cows[i].produceTimer = 0;
		}
	}

	for (int i = 0; i < sheepCount; i++)
	{
		if (sheep[i].isAlive)
		{
			sheep[i].fedState = 0;
			sheep[i].hasProduce = 0;
			sheep[i].produceTimer = 0;
		}
	}
}


// ============================================================
// FEED ANIMALS IN CURRENT PEN
// ============================================================

inline void feedAnimalsByRanchMan()
{
	if (countFeed <= 0)
		return;

	// HEN PEN
	if (isNearHenPen())
	{
		for (int i = 0; i < henCount; i++)
		{
			if (hens[i].isAlive &&
				hens[i].fedState == 0 &&
				countFeed > 0)
			{
				countFeed--;

				hens[i].fedState = 1;

				ranchTimer = 20;
				isRanchTimerActive = 1;
			}
		}
	}

	// COW PEN
	else if (isNearCowPen())
	{
		for (int i = 0; i < cowCount; i++)
		{
			if (cows[i].isAlive &&
				cows[i].fedState == 0 &&
				countFeed > 0)
			{
				countFeed--;

				cows[i].fedState = 1;

				ranchTimer = 20;
				isRanchTimerActive = 1;
			}
		}
	}

	// SHEEP PEN
	else if (isNearSheepPen())
	{
		for (int i = 0; i < sheepCount; i++)
		{
			if (sheep[i].isAlive &&
				sheep[i].fedState == 0 &&
				countFeed > 0)
			{
				countFeed--;

				sheep[i].fedState = 1;

				ranchTimer = 20;
				isRanchTimerActive = 1;
			}
		}
	}
}


// ============================================================
// COLLECT PRODUCE
// ============================================================

inline void collectProduceByRanchMan()
{
	// HENS
	if (isNearHenPen())
	{
		for (int i = 0; i < henCount; i++)
		{
			if (hens[i].isAlive &&
				hens[i].hasProduce)
			{
				countEgg++;

				hens[i].hasProduce = 0;
				hens[i].produceTimer = 0;
			}
		}
	}

	// COWS
	else if (isNearCowPen())
	{
		for (int i = 0; i < cowCount; i++)
		{
			if (cows[i].isAlive &&
				cows[i].hasProduce)
			{
				countMilk++;

				cows[i].hasProduce = 0;
				cows[i].produceTimer = 0;
			}
		}
	}

	// SHEEP
	else if (isNearSheepPen())
	{
		for (int i = 0; i < sheepCount; i++)
		{
			if (sheep[i].isAlive &&
				sheep[i].hasProduce)
			{
				countWool++;

				sheep[i].hasProduce = 0;
				sheep[i].produceTimer = 0;
			}
		}
	}

	// Stop the collection timer once every available product is collected (if level incomplete)
	if (!hasAnyRanchProduce() && !ranchLevelCompleted)
	{
		ranchCollectionTimerRunning = false;
	}
}


// ============================================================
// INITIALIZE LEVEL 2
// ============================================================

inline void initLevel2()
{
	ranchmanX = 400;
	ranchmanY = 30;

	ranchCollectionTimer = 30;
	ranchCollectionTimerRunning = false;

	ranchFailed = false;
	ranchFailedMessageTimer = 0;

	ranchLevelCompleted = false;
	ranchCompleteMessageTimer = 0;

	// Inventory
	countFeed = 5;

	countEgg = 0;
	countMilk = 0;
	countWool = 0;

	// Prices
	feedBuyPrice = 5;

	eggSellPrice = 15;
	milkSellPrice = 30;
	woolSellPrice = 45;

	henBuyPrice = 30;
	cowBuyPrice = 100;
	sheepBuyPrice = 70;

	// Ranchman starting position
	ranchmanX = 400;
	ranchmanY = 30;

	// Reset all animals
	for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++)
	{
		hens[i].isAlive = 0;
		cows[i].isAlive = 0;
		sheep[i].isAlive = 0;
	}

	// ========================================================
	// HENS
	// ========================================================

	henCount = 2;

	hens[0].x = 100;
	hens[0].y = 170;
	hens[0].type = ANIMAL_HEN;
	hens[0].fedState = 0;
	hens[0].produceTimer = 0;
	hens[0].hasProduce = 0;
	hens[0].isAlive = 1;

	hens[1].x = 170;
	hens[1].y = 170;
	hens[1].type = ANIMAL_HEN;
	hens[1].fedState = 0;
	hens[1].produceTimer = 0;
	hens[1].hasProduce = 0;
	hens[1].isAlive = 1;


	// ========================================================
	// COW
	// ========================================================

	cowCount = 1;

	cows[0].x = 310;
	cows[0].y = 260;
	cows[0].type = ANIMAL_COW;
	cows[0].fedState = 0;
	cows[0].produceTimer = 0;
	cows[0].hasProduce = 0;
	cows[0].isAlive = 1;


	// ========================================================
	// SHEEP
	// ========================================================

	sheepCount = 1;

	sheep[0].x = 610;
	sheep[0].y = 230;
	sheep[0].type = ANIMAL_SHEEP;
	sheep[0].fedState = 0;
	sheep[0].produceTimer = 0;
	sheep[0].hasProduce = 0;
	sheep[0].isAlive = 1;
}


// ============================================================
// RENDER ANIMAL
// ============================================================

inline void renderAnimal(struct Animal *a, const char* bmpPath)
{
	if (!a->isAlive)
		return;

	// Animal sprite
	iShowBMP2(
		a->x,
		a->y,
		(char*)bmpPath,
		0
		);

	// Badge position
	int badgeX = a->x + 14;
	int badgeY = a->y + 52;

	// Produce available
	if (a->hasProduce)
	{
		iShowBMP2(
			badgeX,
			badgeY,
			(char*)"assets/green.bmp",
			0
			);
	}

	// Animal needs feeding
	else if (a->fedState == 0)
	{
		iShowBMP2(
			badgeX,
			badgeY,
			(char*)"assets/red.bmp",
			0
			);
	}
}


// ============================================================
// RANCH MARKET UI
// ============================================================

inline void drawRanchMarketUI()
{
	// Main window
	iSetColor(50, 28, 14);
	iFilledRectangle(100, 70, 600, 440);

	iSetColor(140, 95, 45);
	iRectangle(100, 70, 600, 440);

	char buf[64];

	// Title
	iSetColor(240, 200, 80);

	sprintf_s(
		buf,
		sizeof(buf),
		"RANCH MARKET (Gold: $%d)",
		playerGold
		);

	iText(
		280,
		470,
		buf,
		GLUT_BITMAP_HELVETICA_18
		);


	// ========================================================
	// SELL EGGS
	// ========================================================

	sprintf_s(
		buf,
		sizeof(buf),
		"Eggs: %d (Sell $%d)",
		countEgg,
		eggSellPrice
		);

	iSetColor(255, 255, 255);
	iText(
		130,
		400,
		buf,
		GLUT_BITMAP_HELVETICA_12
		);

	iSetColor(45, 130, 55);
	iFilledRectangle(320, 395, 65, 22);

	iSetColor(255, 255, 255);
	iText(
		332,
		401,
		(char*)"SELL",
		GLUT_BITMAP_HELVETICA_12
		);


	// ========================================================
	// SELL MILK
	// ========================================================

	sprintf_s(
		buf,
		sizeof(buf),
		"Milk: %d (Sell $%d)",
		countMilk,
		milkSellPrice
		);

	iSetColor(255, 255, 255);
	iText(
		130,
		350,
		buf,
		GLUT_BITMAP_HELVETICA_12
		);

	iSetColor(45, 130, 55);
	iFilledRectangle(320, 345, 65, 22);

	iSetColor(255, 255, 255);
	iText(
		332,
		351,
		(char*)"SELL",
		GLUT_BITMAP_HELVETICA_12
		);


	// ========================================================
	// SELL WOOL
	// ========================================================

	sprintf_s(
		buf,
		sizeof(buf),
		"Wool: %d (Sell $%d)",
		countWool,
		woolSellPrice
		);

	iSetColor(255, 255, 255);
	iText(
		130,
		300,
		buf,
		GLUT_BITMAP_HELVETICA_12
		);

	iSetColor(45, 130, 55);
	iFilledRectangle(320, 295, 65, 22);

	iSetColor(255, 255, 255);
	iText(
		332,
		301,
		(char*)"SELL",
		GLUT_BITMAP_HELVETICA_12
		);


	// ========================================================
	// BUY FEED
	// ========================================================

	sprintf_s(
		buf,
		sizeof(buf),
		"Animal Feed: %d ($%d)",
		countFeed,
		feedBuyPrice
		);

	iSetColor(255, 255, 255);
	iText(
		420,
		400,
		buf,
		GLUT_BITMAP_HELVETICA_12
		);

	iSetColor(35, 105, 175);
	iFilledRectangle(610, 395, 65, 22);

	iSetColor(255, 255, 255);
	iText(
		622,
		401,
		(char*)"BUY",
		GLUT_BITMAP_HELVETICA_12
		);


	// ========================================================
	// BUY HEN
	// ========================================================

	sprintf_s(
		buf,
		sizeof(buf),
		"Buy Hen ($%d)",
		henBuyPrice
		);

	iSetColor(255, 255, 255);
	iText(
		420,
		350,
		buf,
		GLUT_BITMAP_HELVETICA_12
		);

	iSetColor(35, 105, 175);
	iFilledRectangle(610, 345, 65, 22);

	iSetColor(255, 255, 255);
	iText(
		622,
		351,
		(char*)"BUY",
		GLUT_BITMAP_HELVETICA_12
		);


	// ========================================================
	// BUY COW
	// ========================================================

	sprintf_s(
		buf,
		sizeof(buf),
		"Buy Cow ($%d)",
		cowBuyPrice
		);

	iSetColor(255, 255, 255);
	iText(
		420,
		300,
		buf,
		GLUT_BITMAP_HELVETICA_12
		);

	iSetColor(35, 105, 175);
	iFilledRectangle(610, 295, 65, 22);

	iSetColor(255, 255, 255);
	iText(
		622,
		301,
		(char*)"BUY",
		GLUT_BITMAP_HELVETICA_12
		);


	// ========================================================
	// BUY SHEEP
	// ========================================================

	sprintf_s(
		buf,
		sizeof(buf),
		"Buy Sheep ($%d)",
		sheepBuyPrice
		);

	iSetColor(255, 255, 255);
	iText(
		420,
		250,
		buf,
		GLUT_BITMAP_HELVETICA_12
		);

	iSetColor(35, 105, 175);
	iFilledRectangle(610, 245, 65, 22);

	iSetColor(255, 255, 255);
	iText(
		622,
		251,
		(char*)"BUY",
		GLUT_BITMAP_HELVETICA_12
		);


	// ========================================================
	// CLOSE BUTTON
	// ========================================================

	iSetColor(160, 40, 40);
	iFilledRectangle(600, 90, 80, 30);

	iSetColor(255, 255, 255);
	iText(
		618,
		100,
		(char*)"CLOSE",
		GLUT_BITMAP_HELVETICA_12
		);
}


// ============================================================
// MAIN LEVEL 2 DRAW FUNCTION
// ============================================================

inline void drawLevel2()
{
	iSetColor(255, 255, 255);

	// Background
	iShowBMPAlternative(
		0,
		0,
		(char*)"assets/level2_bg.bmp"
		);


	// ========================================================
	// DRAW ANIMALS
	// ========================================================

	for (int i = 0; i < henCount; i++)
	{
		renderAnimal(
			&hens[i],
			"assets/hen.bmp"
			);
	}

	for (int i = 0; i < cowCount; i++)
	{
		renderAnimal(
			&cows[i],
			"assets/cow.bmp"
			);
	}

	for (int i = 0; i < sheepCount; i++)
	{
		renderAnimal(
			&sheep[i],
			"assets/sheep.bmp"
			);
	}


	// ========================================================
	// DRAW RANCHMAN
	// ========================================================

	iShowBMP2(
		ranchmanX,
		ranchmanY,
		(char*)"assets/farmman1.bmp",
		0
		);


	// ========================================================
	// UPPER RIGHT TOOLBAR
	// ========================================================

	iSetColor(50, 28, 14);
	iFilledRectangle(538, 480, 246, 46);

	iSetColor(140, 95, 45);
	iRectangle(538, 480, 246, 46);


	// FEED BUTTON
	if (selectedRanchTool == 1)
		iSetColor(45, 160, 55);
	else
		iSetColor(120, 100, 80);

	iFilledRectangle(
		543,
		485,
		112,
		36
		);

	iSetColor(255, 255, 255);

	iText(
		560,
		498,
		(char*)"FEED ANIMAL",
		GLUT_BITMAP_HELVETICA_10
		);


	// COLLECT BUTTON
	if (selectedRanchTool == 2)
		iSetColor(45, 160, 55);
	else
		iSetColor(120, 100, 80);

	iFilledRectangle(
		667,
		485,
		112,
		36
		);

	iSetColor(255, 255, 255);

	iText(
		695,
		498,
		(char*)"COLLECT",
		GLUT_BITMAP_HELVETICA_10
		);


	// ========================================================
	// TOP HUD
	// ========================================================

	iSetColor(40, 40, 40);
	iFilledRectangle(
		0,
		540,
		800,
		60
		);


	// Gold
	iSetColor(255, 215, 0);

	char hudStr[64];

	sprintf_s(
		hudStr,
		sizeof(hudStr),
		"Gold: $%d",
		playerGold
		);

	iText(
		10,
		562,
		hudStr,
		GLUT_BITMAP_HELVETICA_12
		);


	// Inventory
	iSetColor(255, 255, 255);

	sprintf_s(
		hudStr,
		sizeof(hudStr),
		"Feed: %d | Eggs: %d | Milk: %d | Wool: %d",
		countFeed,
		countEgg,
		countMilk,
		countWool
		);

	iText(
		110,
		562,
		hudStr,
		GLUT_BITMAP_HELVETICA_10
		);
	// SAVE BUTTON
	iSetColor(240, 140, 30); // Orange background
	iFilledRectangle(
		320,
		552,
		100,
		34
		);

	iSetColor(255, 255, 255); // White border
	iRectangle(
		320,
		552,
		100,
		34
		);

	iSetColor(0, 0, 0); // Black text
	iText(
		355,
		564,
		(char*)"SAVE",
		GLUT_BITMAP_HELVETICA_12
		);

	// MARKET
	iSetColor(45, 130, 180);

	iFilledRectangle(
		430,
		552,
		100,
		34
		);

	iSetColor(255, 255, 255);

	iText(
		452,
		564,
		(char*)"MARKET",
		GLUT_BITMAP_HELVETICA_12
		);


	// BACK TO TOWN
	iSetColor(40, 160, 120);

	iFilledRectangle(
		545,
		552,
		110,
		34
		);

	iSetColor(255, 255, 255);

	iText(
		557,
		564,
		(char*)"Back to Town",
		GLUT_BITMAP_HELVETICA_10
		);


	// MENU
	iSetColor(170, 45, 45);

	iFilledRectangle(
		670,
		552,
		110,
		34
		);

	iSetColor(255, 255, 255);

	iText(
		705,
		564,
		(char*)"MENU",
		GLUT_BITMAP_HELVETICA_12
		);


	// ========================================================
	// COLLECTION TIMER
	// ========================================================

	if (ranchCollectionTimerRunning || hasAnyRanchProduce() || ranchLevelCompleted)
	{
		char timerText[64];

		sprintf_s(
			timerText,
			sizeof(timerText),
			"Collect Time: %02d",
			ranchCollectionTimer
			);

		if (ranchLevelCompleted)
			iSetColor(45, 160, 55);
		else
			iSetColor(220, 45, 45);

		iFilledRectangle(20, 495, 170, 30);

		iSetColor(255, 255, 255);
		iText(
			35,
			505,
			timerText,
			GLUT_BITMAP_HELVETICA_12
			);
	}

	// ========================================================
	// LEVEL 2 GOAL
	// ========================================================

	if (!ranchLevelCompleted)
	{
		char goalText[64];

		sprintf_s(
			goalText,
			sizeof(goalText),
			"Goal: $200   Current: $%d",
			playerGold
			);

		iSetColor(255, 255, 255);
		iText(
			250,
			525,
			goalText,
			GLUT_BITMAP_HELVETICA_12
			);
	}

	// ========================================================
	// FAILED MESSAGE
	// ========================================================

	if (ranchFailed)
	{
		iSetColor(55, 30, 20);
		iFilledRectangle(220, 230, 360, 130);

		iSetColor(255, 255, 255);
		iRectangle(220, 230, 360, 130);

		iSetColor(255, 220, 120);
		iText(
			320,
			315,
			(char*)"Failed to collect!",
			GLUT_BITMAP_HELVETICA_18
			);

		iSetColor(255, 255, 255);
		iText(
			275,
			280,
			(char*)"The animals are hungry again.",
			GLUT_BITMAP_HELVETICA_12
			);
	}



	// ========================================================
	// LEVEL COMPLETE MESSAGE (Temporary Pop-up)
	// ========================================================

	if (ranchCompleteMessageTimer > 0)
	{
		iSetColor(35, 90, 40);
		iFilledRectangle(200, 220, 400, 150);

		iSetColor(255, 255, 255);
		iRectangle(200, 220, 400, 150);

		iSetColor(255, 215, 0);
		iText(
			315,
			310,
			(char*)"LEVEL 2 COMPLETE!",
			GLUT_BITMAP_HELVETICA_18
			);

		iSetColor(255, 255, 255);
		iText(
			280,
			275,
			(char*)"You reached 200 gold!",
			GLUT_BITMAP_HELVETICA_12
			);
	}

	// ========================================================
	// MARKET OVERLAY
	// ========================================================

	if (isRanchMarketOpen)
		drawRanchMarketUI();
}

#endif