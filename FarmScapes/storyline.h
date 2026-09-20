#ifndef STORYLINE_H
#define STORYLINE_H

#include "iGraphics.h"
#include "bitmap_loader.h"

// ============================================================
// STORYLINE DEFINITIONS & CONSTANTS
// ============================================================

#define STATE_STORYLINE 12 // Ensure this value doesn't conflict with your other state IDs

// Image asset IDs
int storyImg1, storyImg2, storyImg3, storyImg4, storyImg5, storyImg6, storyImg7;

// Internal Cinematic FSM States
typedef enum {
	STORY_SCENE_WORK_HIGH,    // Asset 1
	STORY_SCENE_WORK_MID,     // Asset 2
	STORY_SCENE_WORK_BURNOUT, // Asset 3
	STORY_SCENE_REFLECTION,   // Asset 4
	STORY_SCENE_METRO_CITY,   // Asset 5
	STORY_SCENE_METRO_RURAL,  // Asset 6
	STORY_SCENE_TRACTOR_ARRIVE// Asset 7
} StoryScene;

static StoryScene currentStoryScene = STORY_SCENE_WORK_HIGH;

// Frame counters & controls
static int storyTicks = 0;             // Incremented frame count for timing
static int textCharIndex = 0;          // Index for typewriter effect
static bool storyInitialized = false;

// Dynamic text container
static char currentTypeText[256] = "";

// Helper to update text typewriter animation
static void updateTypewriterText(const char* fullText) {
	int len = (int)strlen(fullText);
	if (textCharIndex < len) {
		textCharIndex++;
	}
	strncpy(currentTypeText, fullText, textCharIndex);
	currentTypeText[textCharIndex] = '\0';
}

// Helper to reset typing progression whenever the scene changes
static void changeStoryScene(StoryScene newScene) {
	currentStoryScene = newScene;
	textCharIndex = 0;
	currentTypeText[0] = '\0';
}

// ============================================================
// INITIALIZATION
// ============================================================

void initStorylineAssets() {
	// Load your 7 bitmap images (Update paths to match your actual file locations)
	storyImg1 = iLoadImage("assets/story_work_100.bmp");
	storyImg2 = iLoadImage("assets/story_work_50.bmp");
	storyImg3 = iLoadImage("assets/story_work_10.bmp");
	storyImg4 = iLoadImage("assets/story_reflection.bmp");
	storyImg5 = iLoadImage("assets/story_metro_city.bmp");
	storyImg6 = iLoadImage("assets/story_metro_rural.bmp");
	storyImg7 = iLoadImage("assets/story_tractor.bmp");

	storyTicks = 0;
	currentStoryScene = STORY_SCENE_WORK_HIGH;
	storyInitialized = true;
}

// Call this function whenever starting or restarting the intro cutscene
void startStoryline() {
	storyTicks = 0;
	changeStoryScene(STORY_SCENE_WORK_HIGH);
}

// ============================================================
// LOGIC & TIMING TICK (Call inside your iSetTimer loop)
// ============================================================

void updateStoryline() {
	if (gameState != STATE_STORYLINE) return;

	storyTicks++;

	// Timing assumes tick runs ~30 FPS (1 tick ≈ 33ms)
	// Adjust tick thresholds if your timer speed differs
	if (storyTicks < 90) {
		// 0s to 3s
		if (currentStoryScene != STORY_SCENE_WORK_HIGH) changeStoryScene(STORY_SCENE_WORK_HIGH);
		updateTypewriterText("Arham spent years in the city as a software engineer...");
	}
	else if (storyTicks < 180) {
		// 3s to 6s
		if (currentStoryScene != STORY_SCENE_WORK_MID) changeStoryScene(STORY_SCENE_WORK_MID);
		updateTypewriterText("His days were a cycle of code, coffee, and deadlines.");
	}
	else if (storyTicks < 270) {
		// 6s to 9s
		if (currentStoryScene != STORY_SCENE_WORK_BURNOUT) changeStoryScene(STORY_SCENE_WORK_BURNOUT);
		updateTypewriterText("Work. Time. Life. Everything had to be optimized.");
	}
	else if (storyTicks < 360) {
		// 9s to 12s
		if (currentStoryScene != STORY_SCENE_REFLECTION) changeStoryScene(STORY_SCENE_REFLECTION);
		updateTypewriterText("Until he realized... he was completely exhausted.");
	}
	else if (storyTicks < 450) {
		// 12s to 15s
		if (currentStoryScene != STORY_SCENE_METRO_CITY) changeStoryScene(STORY_SCENE_METRO_CITY);
		updateTypewriterText("So he packed his bag and left.");
	}
	else if (storyTicks < 540) {
		// 15s to 18s
		if (currentStoryScene != STORY_SCENE_METRO_RURAL) changeStoryScene(STORY_SCENE_METRO_RURAL);
		updateTypewriterText("Returning to the old family farm he had almost forgotten.");
	}
	else if (storyTicks < 660) {
		// 18s to 22s
		if (currentStoryScene != STORY_SCENE_TRACTOR_ARRIVE) changeStoryScene(STORY_SCENE_TRACTOR_ARRIVE);
		updateTypewriterText("Maybe life didn't need to be optimized. Maybe it needed to be lived.");
	}
	else {
		// Cutscene finished -> Hand off to gameplay
		gameState = STATE_LEVEL_1; 
	}
}

// ============================================================
// RENDERING (Call inside iDraw when gameState == STATE_STORYLINE)
// ============================================================

void drawStoryline() {
	// 1. Render Current Scene Asset
	switch (currentStoryScene) {
	case STORY_SCENE_WORK_HIGH:
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg1);
		break;
	case STORY_SCENE_WORK_MID:
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg2);
		break;
	case STORY_SCENE_WORK_BURNOUT:
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg3);
		break;
	case STORY_SCENE_REFLECTION:
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg4);
		break;
	case STORY_SCENE_METRO_CITY:
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg5);
		break;
	case STORY_SCENE_METRO_RURAL:
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg6);
		break;
	case STORY_SCENE_TRACTOR_ARRIVE:
		iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg7);
		break;
	}

	// 2. Render Text Box Overlay (Subtitles at bottom)
	iSetColor(0, 0, 0);
	// Draw semi-transparent background banner for readability
	// (Note: replace with simple filled rectangle if iGraphics transparency isn't enabled)
	iFilledRectangle(40, 30, SCREEN_WIDTH - 80, 60);

	iSetColor(255, 255, 255);
	iRectangle(40, 30, SCREEN_WIDTH - 80, 60);

	// Draw Typewriter Text
	iText(60, 52, currentTypeText, GLUT_BITMAP_HELVETICA_18);

	// 3. Skip Prompt
	iSetColor(180, 180, 180);
	iText(SCREEN_WIDTH - 170, SCREEN_HEIGHT - 30, "Press [SPACE] to Skip", GLUT_BITMAP_HELVETICA_12);
}

// ============================================================
// INPUT HANDLER (Call inside iKeyboard)
// ============================================================

void handleStorylineKeyboard(unsigned char key) {
	// Allow player to skip intro by pressing Space or Enter
	if (key == ' ' || key == '\r') {
		gameState = STATE_TOWN;
	}
}

#endif // STORYLINE_H