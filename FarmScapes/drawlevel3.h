#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include <stdio.h>
#include <stdbool.h>

// Screen & World Dimensions
#define L3_SCREEN_WIDTH 800
#define L3_SCREEN_HEIGHT 600

// Water Boundaries (Playable movement area for the boat)
#define WATER_MIN_X 10
#define WATER_MAX_X 710
#define WATER_MIN_Y 10
#define WATER_MAX_Y 450

// Wooden Pier Bounds (Extended down to 340 to cover support stilts)
#define PIER_X 50
#define PIER_Y 350
#define PIER_WIDTH 160
#define PIER_HEIGHT 170

// Boat Collision Box (Expanded height to include fisherman character & hat)
#define BOAT_WIDTH 75
#define BOAT_HEIGHT 45

// Transparent Key for BMP (0xFFFFFF = Pure White background transparency)
#define SPRITE_TRANSPARENT_KEY 0xFFFFFF

// Boat Struct
struct BoatPlayer {
    double x, y;
    double speed;
    bool facingRight; // Tracks orientation (true = Right, false = Left)
};

static BoatPlayer boat;

// Initialize Level 3 state
inline void initLevel3() {
    boat.x = PIER_X + PIER_WIDTH + 20; // Start safely to the right of the pier
    boat.y = 280;                       // Start safely below the pier stilts
    boat.speed = 5.0;
    boat.facingRight = true;
}

// Collision Check: Determines if moving to (x, y) overlaps with the pier
inline bool isCollidingWithPier(double x, double y) {
    double boatLeft = x;
    double boatRight = x + BOAT_WIDTH;
    double boatBottom = y;
    double boatTop = y + BOAT_HEIGHT;

    double pierLeft = PIER_X;
    double pierRight = PIER_X + PIER_WIDTH;
    double pierBottom = PIER_Y;
    double pierTop = PIER_Y + PIER_HEIGHT;

    // Overlap check on both axes
    if (boatRight > pierLeft && boatLeft < pierRight &&
        boatTop > pierBottom && boatBottom < pierTop) {
        return true; // Collision detected
    }
    return false;
}

// WASD Movement with Independent Axis Pier Collision Checking
inline void handleLevel3Keyboard(unsigned char key) {
    double nextX = boat.x;
    double nextY = boat.y;

    // Determine target coordinates based on key pressed
    if (key == 'w' || key == 'W') nextY += boat.speed;
    if (key == 's' || key == 'S') nextY -= boat.speed;

    if (key == 'a' || key == 'A') {
        nextX -= boat.speed;
        boat.facingRight = false; // Turn left
    }
    if (key == 'd' || key == 'D') {
        nextX += boat.speed;
        boat.facingRight = true;  // Turn right
    }

    // Apply X movement if within water boundary and NOT colliding with pier
    if (nextX >= WATER_MIN_X && nextX <= WATER_MAX_X && !isCollidingWithPier(nextX, boat.y)) {
        boat.x = nextX;
    }

    // Apply Y movement if within water boundary and NOT colliding with pier
    if (nextY >= WATER_MIN_Y && nextY <= WATER_MAX_Y && !isCollidingWithPier(boat.x, nextY)) {
        boat.y = nextY;
    }
}

// Render Level 3 environment, UI, and player boat
inline void drawLevel3() {
    // 1. Render Pre-baked BMP Background
    iShowBMP(0, 0, "assets/lvl3_bg.bmp");

    // 2. Render Directional Boat Sprite
    const char* boatSprite = boat.facingRight ? "assets/boat_right.bmp" : "assets/boat_left.bmp";
    iShowBMPAlternative2((int)boat.x, (int)boat.y, (char*)boatSprite, SPRITE_TRANSPARENT_KEY);

    // 3. Top Navigation Buttons (Town & Menu)
	// SAVE BUTTON
	iSetColor(240, 140, 30); // Orange background
	iFilledRectangle(
		425,
		552,
		110,
		34
		);

	iSetColor(255, 255, 255); // White border & rectangle matching level 3 style
	iRectangle(
		425,
		552,
		110,
		34
		);

	iSetColor(0, 0, 0); // Black text
	iText(
		465,
		563,
		(char*)"SAVE",
		GLUT_BITMAP_HELVETICA_12
		);
    iSetColor(50, 50, 50);
    iFilledRectangle(545, 552, 110, 34);
    iFilledRectangle(670, 552, 110, 34);

    iSetColor(255, 255, 255);
    iRectangle(545, 552, 110, 34);
    iRectangle(670, 552, 110, 34);
    iText(578, 563, "TOWN", GLUT_BITMAP_HELVETICA_12);
    iText(703, 563, "MENU", GLUT_BITMAP_HELVETICA_12);

    // 4. Navigation Hint
    iSetColor(240, 240, 240);
    iText(15, 15, "Use WASD to move the boat | Click TOWN or MENU at top right to exit", GLUT_BITMAP_HELVETICA_12);
}

#endif // DRAWLEVEL3_H
