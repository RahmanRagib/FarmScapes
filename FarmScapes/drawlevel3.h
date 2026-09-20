#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================================================
// LEVEL 3 DEFINITIONS & CONSTANTS
// ============================================================

#define FISH_STATE_IDLE 0
#define FISH_STATE_WAITING 1
#define FISH_STATE_CAUGHT 2
#define FISH_STATE_NOTHING 3

// 1/9th Center Screen Proportions (800x600 resolution)
// Width = 800 / 3 = ~266px, Height = 600 / 3 = 200px
#define FISH_POPUP_W 266
#define FISH_POPUP_H 200
#define FISH_POPUP_X 267 // (800 - 266) / 2
#define FISH_POPUP_Y 200 // (600 - 200) / 2

// ============================================================
// LEVEL 3 GLOBAL VARIABLES
// ============================================================

int boatX = 360;
int boatY = 250;
int boatSpeed = 6;

int fishingState = FISH_STATE_IDLE;
int fishingWaitTimer = 0;
int resultDisplayTimer = 0;

int lastCaughtFishType = -1; // 0: Small Fish, 1: Medium Fish, 2: Rare Fish
char caughtFishName[50] = "";
int caughtFishValue = 0;

// Inventory
int countSmallFish = 0;
int countMediumFish = 0;
int countRareFish = 0;

// Market Prices
int smallFishPrice = 15;
int mediumFishPrice = 35;
int rareFishPrice = 75;

int isFishMarketOpen = 0;

// Fish asset file paths
char fishImages[3][100] = {
    "assets/fish_small.bmp",
    "assets/fish_medium.bmp",
    "assets/fish_rare.bmp"
};


// ============================================================
// INITIALIZATION
// ============================================================

void initLevel3()
{
    boatX = 360;
    boatY = 250;
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
    // Handle waiting state before hook result
    if (fishingState == FISH_STATE_WAITING)
    {
        if (fishingWaitTimer > 0)
        {
            fishingWaitTimer--;
        }
        else
        {
            // Determine result: 30% chance nothing, 70% chance fish
            int chance = rand() % 100;

            if (chance < 30)
            {
                fishingState = FISH_STATE_NOTHING;
                resultDisplayTimer = 25; // Displays text for ~2.5s
            }
            else
            {
                fishingState = FISH_STATE_CAUGHT;
                resultDisplayTimer = 30; // Displays pop-up image for ~3.0s

                int fishRoll = rand() % 100;
                if (fishRoll < 50)
                {
                    lastCaughtFishType = 0;
                    strcpy(caughtFishName, "Common Tilapia");
                    caughtFishValue = smallFishPrice;
                    countSmallFish++;
                }
                else if (fishRoll < 85)
                {
                    lastCaughtFishType = 1;
                    strcpy(caughtFishName, "River Salmon");
                    caughtFishValue = mediumFishPrice;
                    countMediumFish++;
                }
                else
                {
                    lastCaughtFishType = 2;
                    strcpy(caughtFishName, "Golden Trout");
                    caughtFishValue = rareFishPrice;
                    countRareFish++;
                }
            }
        }
    }

    // Result display timers
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

    // Decorative water accents
    iSetColor(45, 130, 185);
    iFilledRectangle(50, 100, 120, 10);
    iFilledRectangle(550, 420, 180, 10);
    iFilledRectangle(200, 480, 150, 10);
    iFilledRectangle(620, 150, 100, 10);

    // 2. Navigation Top Bar
    iSetColor(20, 30, 45);
    iFilledRectangle(0, 550, SCREEN_WIDTH, 50);

    iSetColor(255, 255, 255);
    iText(20, 565, "LEVEL 3: FISHERY", GLUT_BITMAP_HELVETICA_18);

    char goldText[50];
    sprintf(goldText, "Gold: %d", playerGold);
    iText(220, 565, goldText, GLUT_BITMAP_HELVETICA_18);

    // Buttons
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

    // 3. Player Boat
    iSetColor(139, 69, 19);
    iFilledRectangle(boatX, boatY, 80, 35);
    iSetColor(100, 40, 10);
    iFilledRectangle(boatX + 10, boatY + 35, 60, 10);

    // Fishing Line / Rod indicator
    if (fishingState == FISH_STATE_WAITING)
    {
        iSetColor(255, 255, 255);
        iLine(boatX + 70, boatY + 45, boatX + 120, boatY - 30);
        iSetColor(220, 50, 50);
        iFilledCircle(boatX + 120, boatY - 30, 5); // Bobber
    }

    // Boat Instructions
    iSetColor(255, 255, 255);
    iText(20, 20, "WASD: Move Boat  |  SPACE: Cast Line", GLUT_BITMAP_HELVETICA_12);

    // 4. "NOTHING BIT" DISPLAY (NO IMAGE, JUST TEXT BOX)
    if (fishingState == FISH_STATE_NOTHING)
    {
        // Dark translucent dialogue card
        iSetColor(15, 25, 35);
        iFilledRectangle(220, 260, 360, 80);
        iSetColor(200, 180, 60);
        iRectangle(220, 260, 360, 80);

        iSetColor(255, 220, 100);
        iText(250, 305, "NOTHING BIT!", GLUT_BITMAP_HELVETICA_18);
        iSetColor(200, 200, 200);
        iText(250, 280, "The fish swam away. Cast your line again!", GLUT_BITMAP_HELVETICA_12);
    }

    // 5. "FISH CAUGHT" POPUP (CENTER 1/9TH SCREEN AREA)
    if (fishingState == FISH_STATE_CAUGHT && lastCaughtFishType >= 0)
    {
        // Outer Popup Modal Overlay
        iSetColor(0, 0, 0);
        // Dim outer background behind popup
        iSetColor(10, 20, 30);
        iFilledRectangle(FISH_POPUP_X - 20, FISH_POPUP_Y - 55, FISH_POPUP_W + 40, FISH_POPUP_H + 90);

        iSetColor(240, 190, 40);
        iRectangle(FISH_POPUP_X - 20, FISH_POPUP_Y - 55, FISH_POPUP_W + 40, FISH_POPUP_H + 90);
        iRectangle(FISH_POPUP_X - 18, FISH_POPUP_Y - 53, FISH_POPUP_W + 36, FISH_POPUP_H + 86);

        // Header Text
        iSetColor(255, 255, 255);
        iText(FISH_POPUP_X + 50, FISH_POPUP_Y + FISH_POPUP_H + 15, "FISH CAUGHT!", GLUT_BITMAP_HELVETICA_18);

        // Fish Image (Rendered at exactly 1/9th center of screen area: 266x200)
        iShowBMP2(FISH_POPUP_X, FISH_POPUP_Y, fishImages[lastCaughtFishType], 0);

        // Details Text
        char caughtDetails[100];
        sprintf(caughtDetails, "%s  (+%d Gold)", caughtFishName, caughtFishValue);
        iSetColor(100, 255, 120);
        iText(FISH_POPUP_X + 15, FISH_POPUP_Y - 35, caughtDetails, GLUT_BITMAP_HELVETICA_18);
    }

    // 6. MARKET OVERLAY
    if (isFishMarketOpen)
    {
        iSetColor(15, 25, 35);
        iFilledRectangle(200, 100, 400, 380);
        iSetColor(40, 120, 200);
        iRectangle(200, 100, 400, 380);

        iSetColor(255, 255, 255);
        iText(330, 440, "FISH MARKET", GLUT_BITMAP_HELVETICA_18);

        // Items & Selling
        char lineStr[100];

        sprintf(lineStr, "Common Tilapia: %d in stock (%d Gold each)", countSmallFish, smallFishPrice);
        iText(220, 380, lineStr, GLUT_BITMAP_HELVETICA_12);
        iSetColor(40, 180, 80);
        iFilledRectangle(510, 370, 65, 25);
        iSetColor(255, 255, 255);
        iText(525, 378, "SELL", GLUT_BITMAP_HELVETICA_12);

        sprintf(lineStr, "River Salmon:     %d in stock (%d Gold each)", countMediumFish, mediumFishPrice);
        iText(220, 320, lineStr, GLUT_BITMAP_HELVETICA_12);
        iSetColor(40, 180, 80);
        iFilledRectangle(510, 310, 65, 25);
        iSetColor(255, 255, 255);
        iText(525, 318, "SELL", GLUT_BITMAP_HELVETICA_12);

        sprintf(lineStr, "Golden Trout:    %d in stock (%d Gold each)", countRareFish, rareFishPrice);
        iText(220, 260, lineStr, GLUT_BITMAP_HELVETICA_12);
        iSetColor(40, 180, 80);
        iFilledRectangle(510, 250, 65, 25);
        iSetColor(255, 255, 255);
        iText(525, 258, "SELL", GLUT_BITMAP_HELVETICA_12);

        // Close Button
        iSetColor(200, 50, 50);
        iFilledRectangle(360, 130, 80, 30);
        iSetColor(255, 255, 255);
        iText(380, 140, "CLOSE", GLUT_BITMAP_HELVETICA_12);
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

    // Boat Movements
    if (key == 'w' || key == 'W') { if (boatY + boatSpeed < 480) boatY += boatSpeed; }
    if (key == 's' || key == 'S') { if (boatY - boatSpeed > 60)  boatY -= boatSpeed; }
    if (key == 'a' || key == 'A') { if (boatX - boatSpeed > 20)  boatX -= boatSpeed; }
    if (key == 'd' || key == 'D') { if (boatX + boatSpeed < 700) boatX += boatSpeed; }

    // Start Fishing
    if (key == ' ')
    {
        if (fishingState == FISH_STATE_IDLE)
        {
            fishingState = FISH_STATE_WAITING;
            fishingWaitTimer = 20 + rand() % 20; // Waiting countdown
        }
    }
}

void handleLevel3MouseClick(int mx, int my)
{
    if (isFishMarketOpen)
    {
        // Close market button
        if (mx >= 360 && mx <= 440 && my >= 130 && my <= 160)
        {
            isFishMarketOpen = 0;
            return;
        }

        // Sell Small Fish
        if (mx >= 510 && mx <= 575 && my >= 370 && my <= 395 && countSmallFish > 0)
        {
            countSmallFish--;
            playerGold += smallFishPrice;
        }
        // Sell Medium Fish
        else if (mx >= 510 && mx <= 575 && my >= 310 && my <= 335 && countMediumFish > 0)
        {
            countMediumFish--;
            playerGold += mediumFishPrice;
        }
        // Sell Rare Fish
        else if (mx >= 510 && mx <= 575 && my >= 250 && my <= 275 && countRareFish > 0)
        {
            countRareFish--;
            playerGold += rareFishPrice;
        }
    }
}

#endif // DRAWLEVEL3_H
