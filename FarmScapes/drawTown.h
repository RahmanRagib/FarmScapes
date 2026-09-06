#ifndef DRAW_TOWN_H
#define DRAW_TOWN_H

#include "iGraphics.h"
#include "bitmap_loader.h"

extern int currentSeason;
extern int playerX;
extern int playerY;
extern int showDialogue;
extern char dialogueText[200];
extern char npcName[50];

// 1. COLLISION LOGIC FOR DIRT PATHS
int canWalk(int newX, int newY) {
	// Player footprint center estimation
	int px = newX + 24;
	int py = newY + 8;

	// --- MAIN VERTICAL SPINES ---
	// Central North-South Main Dirt Path
	if (px >= 360 && px <= 400 && py >= 90 && py <= 510) return 1;

	// --- HORIZONTAL PATH SEGMENTS ---
	// Top Path leading to Hut
	if (px >= 120 && px <= 360 && py >= 360 && py <= 390) return 1;

	// Upper Cross Path leading to Market & Nadira's Herbary (East)
	if (px >= 350 && px <= 580 && py >= 360 && py <= 400) return 1;

	// Path leading directly to Nadira's Herbary door (Level 1)
	if (px >= 500 && px <= 580 && py >= 330 && py <= 480) return 1;

	// Middle Cross Path leading to Ragib's Barn (East)
	if (px >= 360 && px <= 580 && py >= 250 && py <= 320) return 1;

	// Path leading directly to Ragib's Barn door (Level 2)
	if (px >= 480 && px <= 580 && py >= 280 && py <= 360) return 1;

	// Lower Path sweeping past Big Red/Purple Trees
	if (px >= 180 && px <= 360 && py >= 60 && py <= 100) return 1;

	// Bottom-Left Trail loop connecting bottom-left tree area
	if (px >= 180 && px <= 240 && py >= 100 && py <= 140) return 1;

	// Lower Right Path leading towards Anika's Boathouse
	if (px >= 360 && px <= 600 && py >= 60 && py <= 110) return 1;

	// Branch leading up/across to Anika's Boathouse door (Level 3)
	if (px >= 550 && px <= 690 && py >= 80 && py <= 220) return 1;

	return 0; // Block movement off-road
}

// 2. TOWN RENDERING
void drawTown() {
	// Render 800x600 scaled background
	if (currentSeason == 0) iShowBMP(0, 0, "assets/town_summer_bg.bmp");
	else if (currentSeason == 1) iShowBMP(0, 0, "assets/town_rainy_bg.bmp");
	else if (currentSeason == 2) iShowBMP(0, 0, "assets/town_winter_bg.bmp");

	// Render 48x48 player (ignoring pure black background key 0)
	iShowBMP2(playerX, playerY, "assets/farmman1.bmp", 0);

	// --- PERMANENT LEVEL LABELS ABOVE BUILDINGS ---
	iSetColor(255, 255, 255); // White label background box

	// Level 1: Nadira's Herbary
	iSetColor(0, 0, 0);
	iFilledRectangle(545, 485, 75, 20);
	iSetColor(255, 215, 0); // Gold text
	iRectangle(545, 485, 75, 20);
	iText(553, 490, "LEVEL 1", GLUT_BITMAP_HELVETICA_12);

	// Level 2: Ragib's Barn
	iSetColor(0, 0, 0);
	iFilledRectangle(505, 345, 75, 20);
	iSetColor(255, 215, 0);
	iRectangle(505, 345, 75, 20);
	iText(513, 350, "LEVEL 2", GLUT_BITMAP_HELVETICA_12);

	// Level 3: Anika's Boathouse
	iSetColor(0, 0, 0);
	iFilledRectangle(615, 200, 75, 20);
	iSetColor(255, 215, 0);
	iRectangle(615, 200, 75, 20);
	iText(625, 205, "LEVEL 3", GLUT_BITMAP_HELVETICA_12);

	// --- BOTTOM RIGHT: RETURN TO MENU BUTTON ---
	iSetColor(180, 50, 50);
	iFilledRectangle(670, 20, 110, 40);

	iSetColor(255, 255, 255);
	iRectangle(670, 20, 110, 40);
	iText(705, 33, "MENU", GLUT_BITMAP_HELVETICA_12);

	// --- INTERACTION PROMPTS ("PRESS E") ---
	if (!showDialogue) {
		// Doorway of Nadira's Herbary (Level 1)
		if (playerX >= 480 && playerX <= 570 && playerY >= 370 && playerY <= 460) {
			iSetColor(255, 255, 0);
			iText(playerX - 20, playerY + 52, "[E] Talk to Nadira", GLUT_BITMAP_HELVETICA_12);
		}
		// Doorway of Ragib's Barn (Level 2)
		else if (playerX >= 450 && playerX <= 550 && playerY >= 240 && playerY <= 330) {
			iSetColor(255, 255, 0);
			iText(playerX - 20, playerY + 52, "[E] Talk to Ragib", GLUT_BITMAP_HELVETICA_12);
		}
		// Doorway of Anika's Boathouse (Level 3)
		else if (playerX >= 530 && playerX <= 670 && playerY >= 80 && playerY <= 180) {
			iSetColor(255, 255, 0);
			iText(playerX - 20, playerY + 52, "[E] Talk to Anika", GLUT_BITMAP_HELVETICA_12);
		}
	}

	// --- DIALOGUE DISPLAY OVERLAY ---
	if (showDialogue) {
		// Box Background
		iSetColor(30, 30, 30);
		iFilledRectangle(80, 30, 640, 110);

		// Border
		iSetColor(255, 255, 255);
		iRectangle(82, 32, 636, 106);

		// NPC Portraits
		iSetColor(255, 255, 255);
		if (strcmp(npcName, "Nadira") == 0) iShowBMP2(95, 45, "assets/portrait_nadira.bmp", 0);
		else if (strcmp(npcName, "Ragib") == 0) iShowBMP2(95, 45, "assets/portrait_ragib.bmp", 0);
		else if (strcmp(npcName, "Anika") == 0) iShowBMP2(95, 45, "assets/portrait_anika.bmp", 0);

		// Header & Text
		iSetColor(255, 215, 0);
		iText(190, 110, npcName, GLUT_BITMAP_HELVETICA_18);

		iSetColor(255, 255, 255);
		iText(190, 75, dialogueText, GLUT_BITMAP_HELVETICA_12);

		iSetColor(180, 180, 180);
		iText(560, 42, "[Press E to Close]", GLUT_BITMAP_HELVETICA_10);
	}
}

#endif // DRAW_TOWN_H