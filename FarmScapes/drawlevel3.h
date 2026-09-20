#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================
// EXTERNAL & FALLBACK DEFINITIONS (Fixes C2065 Errors)
// ============================================================

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 800
#endif

#ifndef SCREEN_HEIGHT
#define SCREEN_HEIGHT 600
#endif

// Link playerGold from iMain.cpp
extern int playerGold;

// ============================================================
// LEVEL 3 DEFINITIONS & CONSTANTS
// ============================================================

#define FISH_STATE_IDLE 0
#define FISH_STATE_WAITING 1
#define FISH_STATE_HOOKED 2
#define FISH_STATE_CAUGHT 3
#define FISH_STATE_NOTHING 4

#define NUM_FISH_TYPES 6

// 1/9th Center Screen Proportions (800x600 screen)
// Width = 800 / 3 = 266px, Height = 600 / 3 = 200px
#define FISH_POPUP_W 266
#define FISH_POPUP_H 200
#define FISH_POPUP_X 267 // Center X: (800 - 266) / 2
#define FISH_POPUP_Y 200 // Center Y: (600 - 200) / 2

// Repositioned red.bmp higher above the fisherman's head
#define FISHERMAN_HEAD_X 465
#define FISHERMAN_HEAD_Y 330

// ============================================================
// LEVEL 3 GLOBAL VARIABLES
// ============================================================

int fishingState = FISH_STATE_IDLE;
int fishingWaitTimer = 0;
int hookTimer = 0;
int resultDisplayTimer = 0;

int lastCaughtFishType = -1; // Index 0 to 5 for Fish 1 to Fish 6
char caughtFishName[50] = "";
int caughtFishValue = 0;

// Background & Icon image paths (Must be true 24-bit BMP files)
char bgIdleImage[100] = "assets/fisherman_idle.bmp";
char bgPullingImage[100] = "assets/fisherman_pulling.bmp";
char redMarkImage[100] = "assets/red2.bmp";

// Inventory & Placeholder Prices for Fish 1 - Fish 6 (Index 0 to 5)
int fishCount[NUM_FISH_TYPES] = { 0, 0, 0, 0, 0, 0 };
int fishPrices[NUM_FISH_TYPES] = { 10, 20, 30, 40, 50, 60 };

char fishNames[NUM_FISH_TYPES][20] = {
    "Goonch",
    "Perch",
    "Catfish",
    "Tilapia",
    "Rui",
    "Chitol"
};

char fishImages[NUM_FISH_TYPES][100] = {
    "assets/fish1.bmp",
    "assets/fish2.bmp",
    "assets/fish3.bmp",
    "assets/fish4.bmp",
    "assets/fish5.bmp",
    "assets/fish6.bmp"
};

int isFishMarketOpen = 0;


// ============================================================
// INITIALIZATION
// ============================================================

void initLevel3()
{
    fishingState = FISH_STATE_IDLE;
    fishingWaitTimer = 0;
    hookTimer = 0;
    resultDisplayTimer = 0;
    isFishMarketOpen = 0;
    srand((unsigned int)time(NULL));
}


// ============================================================
// LOGIC UPDATES
// ============================================================

void updateLevel3Logic()
{
    // 1. Waiting for fish to bite
    if (fishingState == FISH_STATE_WAITING)
    {
        if (fishingWaitTimer > 0)
        {
            fishingWaitTimer--;
        }
        else
        {
            // 30% chance nothing bit, 70% chance fish hooked
            int chance = rand() % 100;

            if (chance < 30)
            {
                fishingState = FISH_STATE_NOTHING;
                resultDisplayTimer = 25; // Displays "NOTHING BIT" text for ~2.5s
            }
            else
            {
                fishingState = FISH_STATE_HOOKED;
                hookTimer = 35; // Window to click and reel in
            }
        }
    }

    // 2. Fish is hooked on the line (Red exclamation mark active)
    if (fishingState == FISH_STATE_HOOKED)
    {
        if (hookTimer > 0)
        {
            hookTimer--;
        }
        else
        {
            // Missed hook window - fish got away
            fishingState = FISH_STATE_NOTHING;
            resultDisplayTimer = 25;
        }
    }

    // 3. Reset timers back to idle
    if (fishingState == FISH_STATE_CAUGHT || fishingState == FISH_STATE_NOTHING)
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
    // 1. Pixel Art Background Rendering
    if (fishingState == FISH_STATE_CAUGHT || fishingState == FISH_STATE_HOOKED)
    {
        iShowBMP(0, 0, bgPullingImage);
    }
    else
    {
        iShowBMP(0, 0, bgIdleImage);
    }

    // 2. Navigation Top Bar Overlay
    iSetColor(20, 30, 45);
    iFilledRectangle(0, 550, SCREEN_WIDTH, 50);

    iSetColor(255, 255, 255);
    iText(20, 565, "LEVEL 3: FISHERY", GLUT_BITMAP_HELVETICA_18);

    char goldText[50];
    sprintf(goldText, "Gold: %d", playerGold);
    iText(220, 565, goldText, GLUT_BITMAP_HELVETICA_18);

    // Navigation Buttons
    iSetColor(40, 120, 200);
    iFilledRectangle(430, 555, 100, 34);
    iSetColor(255, 255, 255);
    iText(455, 565, "MARKET", GLUT_BITMAP_HELVETICA_12);

    iSetColor(180, 100, 40);
    iFilledRectangle(545, 555, 110, 34);
    iSetColor(255, 255, 255);
    iText(580, 565, "TOWN", GLUT_BITMAP_HELVETICA_12);

    iSetColor(180, 40, 40);
    iFilledRectangle(670, 555, 110, 34);
    iSetColor(255, 255, 255);
    iText(705, 565, "MENU", GLUT_BITMAP_HELVETICA_12);

    // Control Prompt
    iSetColor(255, 255, 255);
    iText(20, 20, "LEFT CLICK: Cast Line / Reel In Fish", GLUT_BITMAP_HELVETICA_12);

    // 3. RED EXCLAMATION MARK ABOVE FISHERMAN'S HEAD WHEN HOOKED
    if (fishingState == FISH_STATE_HOOKED)
    {
        // Renders red.bmp at higher Y position above head
        // If red.bmp has a white background, use iShowBMP2(..., 0xFFFFFF)
        // Otherwise, iShowBMP renders the full BMP crisp and solid
        iShowBMP(FISHERMAN_HEAD_X, FISHERMAN_HEAD_Y, redMarkImage);
    }

    // 4. "NOTHING BIT" DISPLAY (PURE TEXT - NO IMAGE)
    if (fishingState == FISH_STATE_NOTHING)
    {
        iSetColor(15, 25, 35);
        iFilledRectangle(220, 260, 360, 80);
        iSetColor(200, 180, 60);
        iRectangle(220, 260, 360, 80);

        iSetColor(255, 220, 100);
        iText(250, 305, "NOTHING BIT!", GLUT_BITMAP_HELVETICA_18);
        iSetColor(200, 200, 200);
        iText(250, 280, "The fish got away. Left click to cast again!", GLUT_BITMAP_HELVETICA_12);
    }

    // 5. "FISH CAUGHT" POPUP (CLEAN FULL IMAGE - FIXED BLURRY/FADED ISSUE)
    if (fishingState == FISH_STATE_CAUGHT && lastCaughtFishType >= 0)
    {
        // Using iShowBMP ensures black text/borders inside fish BMP are NOT erased as transparent
        iShowBMP(FISH_POPUP_X, FISH_POPUP_Y, fishImages[lastCaughtFishType]);
    }

    // 6. MARKET OVERLAY
    if (isFishMarketOpen)
    {
        iSetColor(15, 25, 35);
        iFilledRectangle(180, 80, 440, 420);
        iSetColor(40, 120, 200);
        iRectangle(180, 80, 440, 420);

        iSetColor(255, 255, 255);
        iText(330, 465, "FISH MARKET", GLUT_BITMAP_HELVETICA_18);

        // Render rows for Fish 1 to Fish 6
        for (int i = 0; i < NUM_FISH_TYPES; i++)
        {
            int itemY = 410 - (i * 45);
            char lineStr[100];
            sprintf(lineStr, "%s: %d owned (%d Gold)", fishNames[i], fishCount[i], fishPrices[i]);

            iSetColor(255, 255, 255);
            iText(200, itemY + 6, lineStr, GLUT_BITMAP_HELVETICA_12);

            // Sell Button
            iSetColor(40, 180, 80);
            iFilledRectangle(520, itemY, 70, 26);
            iSetColor(255, 255, 255);
            iText(538, itemY + 7, "SELL", GLUT_BITMAP_HELVETICA_12);
        }

        // Close Button
        iSetColor(200, 50, 50);
        iFilledRectangle(360, 100, 80, 30);
        iSetColor(255, 255, 255);
        iText(380, 110, "CLOSE", GLUT_BITMAP_HELVETICA_12);
    }
}


// ============================================================
// CONTROLS & INPUT
// ============================================================

void handleLevel3Keyboard(unsigned char key)
{
    if (isFishMarketOpen)
    {
        if (key == 27) isFishMarketOpen = 0; // ESC closes market
        return;
    }
}

void handleLevel3MouseClick(int mx, int my)
{
    // 1. If Fish Market is open, handle UI clicks
    if (isFishMarketOpen)
    {
        // Close market button
        if (mx >= 360 && mx <= 440 && my >= 100 && my <= 130)
        {
            isFishMarketOpen = 0;
            return;
        }

        // Check clicks on sell buttons for Fish 1 - Fish 6
        for (int i = 0; i < NUM_FISH_TYPES; i++)
        {
            int itemY = 410 - (i * 45);
            if (mx >= 520 && mx <= 590 && my >= itemY && my <= itemY + 26)
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

    // 2. Navigation Top Bar Clicks
    if (my >= 550)
    {
        // Market Button
        if (mx >= 430 && mx <= 530)
        {
            isFishMarketOpen = 1;
            return;
        }
        return;
    }

    // 3. Fishing Sequence Control
    if (fishingState == FISH_STATE_IDLE)
    {
        // First Left Click: Cast fishing line
        fishingState = FISH_STATE_WAITING;
        fishingWaitTimer = 25 + rand() % 25;
    }
    else if (fishingState == FISH_STATE_HOOKED)
    {
        // Second Left Click: Reel in hooked fish!
        fishingState = FISH_STATE_CAUGHT;
        resultDisplayTimer = 30; // Displays popup image for ~3.0s

        // Select fish by index (0 through 5)
        int type = rand() % NUM_FISH_TYPES;

        lastCaughtFishType = type;
        strcpy(caughtFishName, fishNames[type]);
        caughtFishValue = fishPrices[type];
        fishCount[type]++;
    }
}

#endif // DRAWLEVEL3_H
