#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ============================================================
// LEVEL 3 DEFINITIONS & CONSTANTS
// ============================================================

#define FISH_STATE_IDLE 0
#define FISH_STATE_WAITING 1
#define FISH_STATE_CAUGHT 2
#define FISH_STATE_NOTHING 3

#define NUM_FISH_TYPES 6

// 1/9th Center Screen Proportions (800x600 screen)
// Width = 800 / 3 = 266px, Height = 600 / 3 = 200px
#define FISH_POPUP_W 266
#define FISH_POPUP_H 200
#define FISH_POPUP_X 267 // Center X: (800 - 266) / 2
#define FISH_POPUP_Y 200 // Center Y: (600 - 200) / 2

// ============================================================
// LEVEL 3 GLOBAL VARIABLES
// ============================================================

int fishingState = FISH_STATE_IDLE;
int fishingWaitTimer = 0;
int resultDisplayTimer = 0;

int lastCaughtFishType = -1; // 0 to 5 corresponding to Fish 1 through Fish 6
char caughtFishName[50] = "";
int caughtFishValue = 0;

// Inventory & Placeholder Prices for Fish 1 - Fish 6 (Index 0 to 5)
int fishCount[NUM_FISH_TYPES] = { 0, 0, 0, 0, 0, 0 };
int fishPrices[NUM_FISH_TYPES] = { 10, 20, 30, 40, 50, 60 };

char fishNames[NUM_FISH_TYPES][20] = {
    "Fish 1",
    "Fish 2",
    "Fish 3",
    "Fish 4",
    "Fish 5",
    "Fish 6"
};

char fishImages[NUM_FISH_TYPES][100] = {
    "assets/fish_1.bmp",
    "assets/fish_2.bmp",
    "assets/fish_3.bmp",
    "assets/fish_4.bmp",
    "assets/fish_5.bmp",
    "assets/fish_6.bmp"
};

int isFishMarketOpen = 0;


// ============================================================
// INITIALIZATION
// ============================================================

void initLevel3()
{
    fishingState = FISH_STATE_IDLE;
    fishingWaitTimer = 0;
    resultDisplayTimer = 0;
    isFishMarketOpen = 0;
    srand((unsigned int)time(NULL));
}


// ============================================================
// LOGIC UPDATES
// ============================================================

void updateLevel3Logic()
{
    // Handle fishing wait timer
    if (fishingState == FISH_STATE_WAITING)
    {
        if (fishingWaitTimer > 0)
        {
            fishingWaitTimer--;
        }
        else
        {
            // 30% chance nothing bit, 70% chance catching a fish
            int chance = rand() % 100;

            if (chance < 30)
            {
                fishingState = FISH_STATE_NOTHING;
                resultDisplayTimer = 25; // Displays text for ~2.5s
            }
            else
            {
                fishingState = FISH_STATE_CAUGHT;
                resultDisplayTimer = 30; // Displays pop-up for ~3.0s

                // Select fish by index (0 through 5)
                int type = rand() % NUM_FISH_TYPES;

                lastCaughtFishType = type;
                strcpy(caughtFishName, fishNames[type]);
                caughtFishValue = fishPrices[type];
                fishCount[type]++;
            }
        }
    }

    // Timer countdown to reset back to idle
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
    // 1. Water Background
    iSetColor(28, 107, 160);
    iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Decorative Water Accents
    iSetColor(45, 130, 185);
    iFilledRectangle(50, 100, 120, 10);
    iFilledRectangle(550, 420, 180, 10);
    iFilledRectangle(200, 480, 150, 10);
    iFilledRectangle(620, 150, 100, 10);

    // Fishing Pier / Dock
    iSetColor(120, 70, 30);
    iFilledRectangle(320, 0, 160, 150);
    iSetColor(90, 50, 20);
    iRectangle(320, 0, 160, 150);

    // Fishing Rod Line (Visible when line is cast)
    if (fishingState == FISH_STATE_WAITING)
    {
        iSetColor(255, 255, 255);
        iLine(400, 140, 400, 280);
        iSetColor(220, 50, 50);
        iFilledCircle(400, 280, 6); // Bobber
    }

    // 2. Navigation Top Bar
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
    iText(20, 20, "SPACE: Cast Fishing Line", GLUT_BITMAP_HELVETICA_12);

    // 3. "NOTHING BIT" DISPLAY (PURE TEXT - NO IMAGE)
    if (fishingState == FISH_STATE_NOTHING)
    {
        iSetColor(15, 25, 35);
        iFilledRectangle(220, 260, 360, 80);
        iSetColor(200, 180, 60);
        iRectangle(220, 260, 360, 80);

        iSetColor(255, 220, 100);
        iText(250, 305, "NOTHING BIT!", GLUT_BITMAP_HELVETICA_18);
        iSetColor(200, 200, 200);
        iText(250, 280, "The fish swam away. Cast your line again!", GLUT_BITMAP_HELVETICA_12);
    }

    // 4. "FISH CAUGHT" POPUP (CENTERED AT 1/9TH SCREEN AREA: 266x200)
    if (fishingState == FISH_STATE_CAUGHT && lastCaughtFishType >= 0)
    {
        // Card frame behind popup
        iSetColor(10, 20, 30);
        iFilledRectangle(FISH_POPUP_X - 20, FISH_POPUP_Y - 55, FISH_POPUP_W + 40, FISH_POPUP_H + 90);

        iSetColor(240, 190, 40);
        iRectangle(FISH_POPUP_X - 20, FISH_POPUP_Y - 55, FISH_POPUP_W + 40, FISH_POPUP_H + 90);
        iRectangle(FISH_POPUP_X - 18, FISH_POPUP_Y - 53, FISH_POPUP_W + 36, FISH_POPUP_H + 86);

        // Header Title
        iSetColor(255, 255, 255);
        iText(FISH_POPUP_X + 50, FISH_POPUP_Y + FISH_POPUP_H + 15, "FISH CAUGHT!", GLUT_BITMAP_HELVETICA_18);

        // Fish BMP Image rendered cleanly at center 1/9th size (266x200)
        iShowBMP2(FISH_POPUP_X, FISH_POPUP_Y, fishImages[lastCaughtFishType], 0);

        // Fish details text
        char caughtDetails[100];
        sprintf(caughtDetails, "%s  (+%d Gold)", caughtFishName, caughtFishValue);
        iSetColor(100, 255, 120);
        iText(FISH_POPUP_X + 45, FISH_POPUP_Y - 35, caughtDetails, GLUT_BITMAP_HELVETICA_18);
    }

    // 5. MARKET OVERLAY (FISH 1 THROUGH FISH 6)
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

    // Spacebar to Cast Line
    if (key == ' ')
    {
        if (fishingState == FISH_STATE_IDLE)
        {
            fishingState = FISH_STATE_WAITING;
            fishingWaitTimer = 20 + rand() % 20;
        }
    }
}

void handleLevel3MouseClick(int mx, int my)
{
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
    }
}

#endif // DRAWLEVEL3_H
