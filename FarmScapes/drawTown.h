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

// ============================================================
// 1. COLLISION LOGIC PRECISELY MATCHING DIRT PATHS & INTERACTION ZONES
// ============================================================
int canWalk(int newX, int newY) {
	// Player footprint center estimation (48x48 sprite)
	int px = newX + 24;
	int py = newY + 8;

	// --- MAIN HORIZONTAL DIRT ROAD (WEST TO EAST) ---
	if (px >= 10 && px <= 790 && py >= 270 && py <= 330) return 1;

	// --- SECTION 1: NADIRA'S HERBARY & CROPLANDS (TOP LEFT) ---
	// Vertical Branch from Main Road to Herbary
	if (px >= 140 && px <= 240 && py >= 320 && py <= 460) return 1;

	// Horizontal path across crops & front door of Herbary
	if (px >= 80 && px <= 260 && py >= 340 && py <= 400) return 1;

	// --- SECTION 2: RAGIB'S RANCH (BOTTOM MIDDLE) ---
	// Vertical Branch going down into Ranch (Extended so player can reach Y = 160)
	if (px >= 320 && px <= 460 && py >= 150 && py <= 280) return 1;

	// --- SECTION 3: ANIKA'S FISHING & POND (TOP RIGHT) ---
	// Vertical Branch & area around Boathouse/Pier
	if (px >= 550 && px <= 730 && py >= 320 && py <= 450) return 1;

	return 0; // Block movement off-road
}

// ============================================================
// 2. TOWN RENDERING
// ============================================================
void drawTown() {
	// Render 800x600 scaled background
	if (currentSeason == 0) iShowBMP(0, 0, "assets/town_summer_bg.bmp");
	else if (currentSeason == 1) iShowBMP(0, 0, "assets/town_rainy_bg.bmp");
	else if (currentSeason == 2) iShowBMP(0, 0, "assets/town_winter_bg.bmp");

	// Render 48x48 player (ignoring pure black background key 0)
	iShowBMP2(playerX, playerY, "assets/farmman1.bmp", 0);

	// --- LEVEL LABELS MOVED HIGHER ABOVE BUILDINGS ---

	// Level 1: Nadira's Herbary (Top-Left)
	iSetColor(0, 0, 0);
	iFilledRectangle(125, 535, 80, 20);
	iSetColor(255, 215, 0); // Gold border
	iRectangle(125, 535, 80, 20);
	iText(133, 540, "LEVEL 1", GLUT_BITMAP_HELVETICA_12);

	// Level 2: Ragib's Ranch (Bottom-Middle)
	iSetColor(0, 0, 0);
	iFilledRectangle(360, 235, 80, 20);
	iSetColor(255, 215, 0);
	iRectangle(360, 235, 80, 20);
	iText(368, 240, "LEVEL 2", GLUT_BITMAP_HELVETICA_12);

	// Level 3: Anika's Fishing (Top-Right)
	iSetColor(0, 0, 0);
	iFilledRectangle(610, 535, 80, 20);
	iSetColor(255, 215, 0);
	iRectangle(610, 535, 80, 20);
	iText(618, 540, "LEVEL 3", GLUT_BITMAP_HELVETICA_12);

	// --- BOTTOM RIGHT: RETURN TO MENU BUTTON ---
	iSetColor(180, 50, 50);
	iFilledRectangle(670, 20, 110, 40);

	iSetColor(255, 255, 255);
	iRectangle(670, 20, 110, 40);
	iText(705, 33, "MENU", GLUT_BITMAP_HELVETICA_12);

	// --- INTERACTION PROMPTS ("PRESS E") ---
	if (!showDialogue) {
		// Nadira (Top-Left)
		if (playerX >= 120 && playerX <= 240 && playerY >= 340 && playerY <= 450) {
			iSetColor(255, 255, 0);
			iText(playerX - 20, playerY + 52, "[E] Talk to Nadira", GLUT_BITMAP_HELVETICA_12);
		}
		// Ragib (Bottom-Middle)
		else if (playerX >= 320 && playerX <= 460 && playerY >= 150 && playerY <= 260) {
			iSetColor(255, 255, 0);
			iText(playerX - 20, playerY + 52, "[E] Talk to Ragib", GLUT_BITMAP_HELVETICA_12);
		}
		// Anika (Top-Right)
		else if (playerX >= 550 && playerX <= 720 && playerY >= 320 && playerY <= 440) {
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
		iText(560, 42, "[Press E to Continue]", GLUT_BITMAP_HELVETICA_10);
	}
}

#endif // DRAW_TOWN_H