#ifndef DRAWLEVEL3_H
#define DRAWLEVEL3_H

#include <stdio.h>
#include <stdbool.h>

// Screen & World Dimensions
#define L3_SCREEN_WIDTH 800
#define L3_SCREEN_HEIGHT 600

// Water Boundaries (Playable movement area for the boat)
#define WATER_MIN_X 20
#define WATER_MAX_X 720
#define WATER_MIN_Y 20
#define WATER_MAX_Y 440

// Wooden Pier Bounds
#define PIER_X 60
#define PIER_Y 400
#define PIER_WIDTH 140
#define PIER_HEIGHT 100

// Transparent Key for BMP (0xFFFFFF = Pure White background transparency)
#define SPRITE_TRANSPARENT_KEY 0xFFFFFF

// Boat Struct
struct BoatPlayer {
    double x, y;
    double speed;
    bool isAtPier;
    bool facingRight; // Tracks orientation (true = Right, false = Left)
};

static BoatPlayer boat;

// Initialize Level 3 state
inline void initLevel3() {
    boat.x = PIER_X + PIER_WIDTH + 10;
    boat.y = PIER_Y - 20;
    boat.speed = 5.0;
    boat.isAtPier = true;
    boat.facingRight = true;
}

// Checks if the boat is close enough to dock at the pier
inline void updatePierProximity() {
    if (boat.x <= (PIER_X + PIER_WIDTH + 25) &&
        boat.y >= (PIER_Y - 40) &&
        boat.y <= (PIER_Y + PIER_HEIGHT + 20)) {
        boat.isAtPier = true;
    } else {
        boat.isAtPier = false;
    }
}

// WASD Movement, Direction Flipping & Pier Interaction
inline bool handleLevel3Keyboard(unsigned char key) {
    double nextX = boat.x;
    double nextY = boat.y;

    // Movement & Orientation
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

    // Apply movement within strict water boundaries
    if (nextX >= WATER_MIN_X && nextX <= WATER_MAX_X) {
        boat.x = nextX;
    }
    if (nextY >= WATER_MIN_Y && nextY <= WATER_MAX_Y) {
        boat.y = nextY;
    }

    // Update pier proximity after position change
    updatePierProximity();

    // Return true when docking at pier and pressing 'E'
    if ((key == 'e' || key == 'E') && boat.isAtPier) {
        return true;
    }

    return false;
}

// Render Level 3 environment and player boat
inline void drawLevel3() {
    // 1. Water Background (Cyan/Blue)
    iSetColor(30, 144, 255);
    iFilledRectangle(0, 0, L3_SCREEN_WIDTH, L3_SCREEN_HEIGHT);

    // 2. Shore / Grass Top Edge
    iSetColor(34, 139, 34);
    iFilledRectangle(0, WATER_MAX_Y + 50, L3_SCREEN_WIDTH, L3_SCREEN_HEIGHT - (WATER_MAX_Y + 50));

    // 3. Wooden Pier
    iSetColor(139, 69, 19);
    iFilledRectangle(PIER_X, PIER_Y, PIER_WIDTH, PIER_HEIGHT);

    // Pier Posts
    iSetColor(90, 40, 10);
    for (int px = PIER_X + 10; px < PIER_X + PIER_WIDTH; px += 30) {
        iFilledRectangle(px, PIER_Y - 12, 10, 12);
    }

    // 4. Render Directional Boat Sprite
    // Flips asset based on direction and strips pure white (0xFFFFFF) background
    const char* boatSprite = boat.facingRight ? "assets/boat_right.bmp" : "assets/boat_left.bmp";
    iShowBMPAlternative2((int)boat.x, (int)boat.y, (char*)boatSprite, SPRITE_TRANSPARENT_KEY);

    // 5. Interaction UI Prompt
    if (boat.isAtPier) {
        iSetColor(255, 255, 255);
        iText(PIER_X, PIER_Y + PIER_HEIGHT + 15, "Docked at Pier: Press [E] to Return to Town", GLUT_BITMAP_HELVETICA_12);
    }

    // Navigation Hint
    iSetColor(240, 240, 240);
    iText(15, 15, "Use WASD to move the boat | Press E at pier to exit", GLUT_BITMAP_HELVETICA_12);
}

#endif // DRAWLEVEL3_H
