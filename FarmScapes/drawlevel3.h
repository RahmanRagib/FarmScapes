#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 800
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 600
#endif

// Link global variables from main
extern int gameState;
extern int playerGold;
extern int currentSaveSlot;
extern void saveGameProgress(int slot);

// ============================================================
// LEVEL 3 DEFINITIONS & CONSTANTS
// ============================================================

#define FISH_STATE_IDLE 0
#define FISH_STATE_WAITING 1
#define FISH_STATE_HOOKED 2
#define FISH_STATE_CAUGHT 3
#define FISH_STATE_NOTHING 4
#define FISH_STATE_TOO_EARLY 5

#ifndef NUM_FISH_TYPES
#define NUM_FISH_TYPES 9
#endif

#define NUM_MARKET_FISH 6 // Only the first 6 fish are for sale in the market

#define FISH_POPUP_W 399
#define FISH_POPUP_H 300
#define FISH_POPUP_X 235 // Centered pop-up graphic position
#define FISH_POPUP_Y 150

#define FISHERMAN_HEAD_X 465
#define FISHERMAN_HEAD_Y 330

// ============================================================
// LEVEL 3 GLOBAL VARIABLES
// ============================================================

int fishingState = FISH_STATE_IDLE;
int fishingWaitTimer = 0;
int hookTimer = 0;
int resultDisplayTimer = 0;
int castPopupTimer = 0;

int lastCaughtFishType = -1;
char caughtFishName[50] = "";
int caughtFishValue = 0;

char bgIdleImage[100] = "assets/fisherman_idle.bmp";
char bgPullingImage[100] = "assets/fisherman_pulling.bmp";
char redMarkImage[100] = "assets/red2.bmp";

int fishCount[NUM_FISH_TYPES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int fishPrices[NUM_FISH_TYPES] = { 70, 40, 30, 10, 30, 50, 0, 0, 0 };

char fishNames[NUM_FISH_TYPES][20] = {
    "Goonch",
    "Perch",
    "Catfish",
    "Tilapia",
    "Rui",
    "Chitol",
    "Puffer Fish",
    "Pleco",
    "Turtle"
};

char fishImages[NUM_FISH_TYPES][100] = {
    "assets/fish1.bmp",
    "assets/fish2.bmp",
    "assets/fish3.bmp",
    "assets/fish4.bmp",
    "assets/fish5.bmp",
    "assets/fish6.bmp",
    "assets/fish7.bmp",
    "assets/fish8.bmp",
    "assets/fish9.bmp"
};

// Probability weights (Sum = 100%)
// Index 0: Goonch      (3%)  - Rare
// Index 1: Perch       (13%) - High
// Index 2: Catfish     (13%) - High
// Index 3: Tilapia     (18%) - Common
// Index 4: Rui         (10%) - Mid
// Index 5: Chitol      (4%)  - Rare
// Index 6: Puffer Fish (18%) - Common (Challenge/Trash)
// Index 7: Pleco       (18%) - Common
// Index 8: Turtle      (3%)  - Rare
static const int fishWeights[NUM_FISH_TYPES] = { 3, 13, 13, 18, 10, 4, 18, 18, 3 };

int isFishMarketOpen = 0;

// ============================================================
// PROBABILITY HELPER
// ============================================================

int getRandomFishType()
{
    int roll = rand() % 100; // Roll 0 to 99
    int accumulated = 0;

    for (int i = 0; i < NUM_FISH_TYPES; i++)
    {
        accumulated += fishWeights[i];
        if (roll < accumulated)
        {
            return i;
        }
    }
    return 3; // Default fallback to Tilapia
}

// ============================================================
// INITIALIZATION
// ============================================================

void initLevel3()
{
    fishingState = FISH_STATE_IDLE;
    fishingWaitTimer = 0;
    hookTimer = 0;
    resultDisplayTimer = 0;
    castPopupTimer = 0;
    isFishMarketOpen = 0;
    srand((unsigned int)time(NULL));
}

// ============================================================
// LOGIC UPDATES
// ============================================================

void updateLevel3Logic()
{
    if (gameState != STATE_LEVEL_3)
        return;

    if (castPopupTimer > 0)
    {
        castPopupTimer--;
    }

    // 1. Waiting for fish to bite
    if (fishingState == FISH_STATE_WAITING)
    {
        if (fishingWaitTimer > 0)
        {
            fishingWaitTimer--;
        }
        else
        {
            int chance = rand() % 100;

            if (chance < 30)
            {
                fishingState = FISH_STATE_NOTHING;
                resultDisplayTimer = 25;
            }
            else
            {
                fishingState = FISH_STATE_HOOKED;
                hookTimer = 35;
            }
        }
    }

    // 2. Fish is hooked on the line
    if (fishingState == FISH_STATE_HOOKED)
    {
        if (hookTimer > 0)
        {
            hookTimer--;
        }
        else
        {
            fishingState = FISH_STATE_NOTHING;
            resultDisplayTimer = 25;
        }
    }

    // 3. Auto-reset timer back to idle if no click occurs
    if (fishingState == FISH_STATE_CAUGHT ||
        fishingState == FISH_STATE_NOTHING ||
        fishingState == FISH_STATE_TOO_EARLY)
    {
        if (resultDisplayTimer > 0)
        {
            resultDisplayTimer--;
        }
        else
        {
            fishingState = FISH_STATE_IDLE;
        }
    }
}

// ============================================================
// DRAW LEVEL 3
// ============================================================

void drawLevel3()
{
    // 1. Pixel Art Background
    if (fishingState == FISH_STATE_CAUGHT || fishingState == FISH_STATE_HOOKED)
    {
        iShowBMP(0, 0, bgPullingImage);
    }
    else
    {
        iShowBMP(0, 0, bgIdleImage);
    }

    // 2. Navigation Top Bar Overlay
    iSetColor(40, 40, 40);
    iFilledRectangle(0, 540, SCREEN_WIDTH, 60);

    iSetColor(255, 255, 255);
    iText(10, 572, "LEVEL 3: FISHERY", GLUT_BITMAP_HELVETICA_12);

    char goldText[50];
    sprintf(goldText, "Gold: $%d", playerGold);
    iSetColor(255, 215, 0);
    iText(160, 572, goldText, GLUT_BITMAP_HELVETICA_12);

    // SAVE BUTTON
    iSetColor(240, 140, 30);
    iFilledRectangle(320, 552, 100, 34);
    iSetColor(255, 255, 255);
    iRectangle(320, 552, 100, 34);
    iSetColor(0, 0, 0);
    iText(355, 564, "SAVE", GLUT_BITMAP_HELVETICA_12);

    // MARKET BUTTON
    iSetColor(45, 130, 180);
    iFilledRectangle(430, 552, 100, 34);
    iSetColor(255, 255, 255);
    iRectangle(430, 552, 100, 34);
    iText(452, 564, "MARKET", GLUT_BITMAP_HELVETICA_12);

    // TOWN BUTTON
    iSetColor(40, 160, 120);
    iFilledRectangle(545, 552, 110, 34);
    iSetColor(255, 255, 255);
    iRectangle(545, 552, 110, 34);
    iText(557, 564, "Back to Town", GLUT_BITMAP_HELVETICA_10);

    // MENU BUTTON
    iSetColor(170, 45, 45);
    iFilledRectangle(670, 552, 110, 34);
    iSetColor(255, 255, 255);
    iRectangle(670, 552, 110, 34);
    iText(705, 564, "MENU", GLUT_BITMAP_HELVETICA_12);

    // Prompt
    iSetColor(255, 255, 255);
    iText(20, 20, "LEFT CLICK: Cast Line / Reel In Fish", GLUT_BITMAP_HELVETICA_12);

    // 3. Exclamation mark when hooked
    if (fishingState == FISH_STATE_HOOKED)
    {
        iShowBMP(FISHERMAN_HEAD_X, FISHERMAN_HEAD_Y, redMarkImage);
    }

    // 4. "LINE HAS BEEN CAST" Popup
    if (fishingState == FISH_STATE_WAITING && castPopupTimer > 0)
    {
        iSetColor(15, 25, 35);
        iFilledRectangle(220, 70, 360, 80);
        iSetColor(40, 180, 200);
        iRectangle(220, 70, 360, 80);

        iSetColor(100, 220, 255);
        iText(250, 115, "LINE HAS BEEN CAST!", GLUT_BITMAP_HELVETICA_18);
        iSetColor(200, 200, 200);
        iText(250, 90, "Waiting for a fish to bite...", GLUT_BITMAP_HELVETICA_12);
    }

    // 5. "NOTHING BIT" Popup
    if (fishingState == FISH_STATE_NOTHING)
    {
        iSetColor(15, 25, 35);
        iFilledRectangle(220, 70, 360, 80);
        iSetColor(200, 180, 60);
        iRectangle(220, 70, 360, 80);

        iSetColor(255, 220, 100);
        iText(250, 115, "NOTHING BIT!", GLUT_BITMAP_HELVETICA_18);
        iSetColor(200, 200, 200);
        iText(250, 90, "Left click to cast again!", GLUT_BITMAP_HELVETICA_12);
    }

    // 6. "REELED IN TOO QUICKLY" Popup
    if (fishingState == FISH_STATE_TOO_EARLY)
    {
        iSetColor(20, 15, 25);
        iFilledRectangle(160, 60, 480, 95);
        iSetColor(220, 60, 60);
        iRectangle(160, 60, 480, 95);

        iSetColor(255, 80, 80);
        iText(180, 125, "REELED IN TOO QUICKLY!", GLUT_BITMAP_HELVETICA_18);
        iSetColor(255, 220, 220);
        iText(180, 100, "You reeled in too quickly before the fish could even bite.", GLUT_BITMAP_HELVETICA_12);
    }

    // 7. "FISH CAUGHT" Popup & Special Turtle Release Banner
    if (fishingState == FISH_STATE_CAUGHT && lastCaughtFishType >= 0)
    {
        iShowBMP(FISH_POPUP_X, FISH_POPUP_Y, fishImages[lastCaughtFishType]);

        // Turtle Release Popup Banner
        if (lastCaughtFishType == 8)
        {
            iSetColor(20, 30, 20);
            iFilledRectangle(180, 100, 440, 40);
            iSetColor(60, 180, 80);
            iRectangle(180, 100, 440, 40);

            iSetColor(220, 255, 220);
            iText(210, 115, "The turtle was released back in the pond.", GLUT_BITMAP_HELVETICA_12);
        }
    }

    // 8. MARKET OVERLAY (Only displays sellable fish 0 to 5)
    if (isFishMarketOpen)
    {
        iSetColor(15, 25, 35);
        iFilledRectangle(180, 80, 440, 420);
        iSetColor(40, 120, 200);
        iRectangle(180, 80, 440, 420);

        // Header Banner Placeholder
        iShowBMPAlternative2(280, 455, "assets/market.bmp", 0xFFFFFF);

        // Gold Icon & Current Balance
        iShowBMPAlternative2(200, 410, "assets/gold.bmp", 0xFFFFFF);
        char goldBuf[64];
        sprintf(goldBuf, "$%d", playerGold);
        iSetColor(240, 200, 80);
        iText(370, 417, goldBuf, GLUT_BITMAP_HELVETICA_18);

        // Sellable Fish List (Goonch, Perch, Catfish, Tilapia, Rui, Chitol)
        for (int i = 0; i < NUM_MARKET_FISH; i++)
        {
            int itemY = 360 - (i * 38);
            char lineStr[100];
            sprintf(lineStr, "%s: %d owned (%d Gold)", fishNames[i], fishCount[i], fishPrices[i]);

            iSetColor(255, 255, 255);
            iText(200, itemY + 5, lineStr, GLUT_BITMAP_HELVETICA_12);

            // Sell Button
            iSetColor(40, 180, 80);
            iFilledRectangle(520, itemY, 70, 24);
            iSetColor(255, 255, 255);
            iText(538, itemY + 7, "SELL", GLUT_BITMAP_HELVETICA_10);
        }

        // Close Button
        iSetColor(200, 50, 50);
        iFilledRectangle(360, 95, 80, 30);
        iSetColor(255, 255, 255);
        iText(380, 105, "CLOSE", GLUT_BITMAP_HELVETICA_12);
    }
}

// ============================================================
// CONTROLS & INPUT
// ============================================================

void handleLevel3Keyboard(unsigned char key)
{
    if (isFishMarketOpen)
    {
        if (key == 27) isFishMarketOpen = 0;
        return;
    }

    if (key == 27)
    {
        gameState = STATE_TOWN;
    }
}

void handleLevel3MouseClick(int mx, int my)
{
    // 1. Handle Market interactions
    if (isFishMarketOpen)
    {
        if (mx >= 360 && mx <= 440 && my >= 95 && my <= 125)
        {
            isFishMarketOpen = 0;
            return;
        }

        for (int i = 0; i < NUM_MARKET_FISH; i++)
        {
            int itemY = 360 - (i * 38);
            if (mx >= 520 && mx <= 590 && my >= itemY && my <= itemY + 24)
            {
                if (fishCount[i] > 0)
                {
                    fishCount[i]--;
                    playerGold += fishPrices[i];
                }
                return;
            }
        }
        return;
    }

    // 2. Ignore top bar area
    if (my >= 540)
    {
        return;
    }

    // 3. Fishing Logic Clicks
    if (fishingState == FISH_STATE_IDLE ||
        fishingState == FISH_STATE_NOTHING ||
        fishingState == FISH_STATE_TOO_EARLY ||
        fishingState == FISH_STATE_CAUGHT)
    {
        // Immediately cast line, bypassing display timer wait
        fishingState = FISH_STATE_WAITING;
        fishingWaitTimer = 25 + rand() % 25;
        castPopupTimer = 10;
        resultDisplayTimer = 0;
    }
    else if (fishingState == FISH_STATE_WAITING)
    {
        // Clicked before fish hooked
        fishingState = FISH_STATE_TOO_EARLY;
        resultDisplayTimer = 35;
        castPopupTimer = 0;
    }
    else if (fishingState == FISH_STATE_HOOKED)
    {
        fishingState = FISH_STATE_CAUGHT;
        resultDisplayTimer = 35;

        int type = getRandomFishType();

        lastCaughtFishType = type;
        strcpy(caughtFishName, fishNames[type]);
        caughtFishValue = fishPrices[type];

        // Only add to inventory if it's a sellable fish (Types 0 to 5)
        if (type < NUM_MARKET_FISH)
        {
            fishCount[type]++;
        }
    }
}

#endif // DRAWLEVEL3_H
