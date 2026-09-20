#ifndef STORYLINE_H
#define STORYLINE_H

#include "iGraphics.h"
#include "bitmap_loader.h"
#include <string.h>

// ============================================================
// STORYLINE DEFINITIONS & CONSTANTS
// ============================================================

#ifndef STATE_STORYLINE
#define STATE_STORYLINE 12
#endif

// Image asset IDs (marked static to avoid multi-definition linker errors)
static int storyImg1, storyImg2, storyImg3, storyImg4, storyImg5, storyImg6, storyImg7;

// Internal Cinematic FSM States
typedef enum {
	STORY_SCENE_WORK_HIGH = 0, // Asset 1
	STORY_SCENE_WORK_MID,      // Asset 2
	STORY_SCENE_WORK_BURNOUT,  // Asset 3
	STORY_SCENE_REFLECTION,    // Asset 4
	STORY_SCENE_METRO_CITY,    // Asset 5
	STORY_SCENE_METRO_RURAL,   // Asset 6
	STORY_SCENE_TRACTOR_ARRIVE,// Asset 7
	STORY_SCENE_COUNT          // Total number of scenes
} StoryScene;

// Dialogues corresponding to each scene
static const char* STORY_DIALOGUES[] = {
	"Arham spent years in the city as a software engineer...",
	"His days were a cycle of code, coffee, and deadlines.",
	"Work. Time. Life. Everything had to be optimized.",
	"Until he realized... he was completely exhausted.",
	"So he packed his bag and left.",
	"Returning to the old family farm he had almost forgotten.",
	"Maybe life didn't need to be optimized. Maybe it needed to be lived."
};

static StoryScene currentStoryScene = STORY_SCENE_WORK_HIGH;

// Frame counters & controls
static int storyTicks = 0;             // Incremented frame count for timing
static int textCharIndex = 0;          // Index for typewriter effect
static bool storyInitialized = false;

// Dynamic text container
static char currentTypeText[256] = "";

// Helper to update text typewriter animation
static void updateTypewriterText(const char* fullText) {
	if (!fullText) return;
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
	// Load 7 bitmap images
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
// LOGIC & TIMING TICK
// ============================================================

void updateStoryline() {
	if (gameState != STATE_STORYLINE) return;

	storyTicks++;

	if (storyTicks < 90) {
		if (currentStoryScene != STORY_SCENE_WORK_HIGH) changeStoryScene(STORY_SCENE_WORK_HIGH);
	}
	else if (storyTicks < 180) {
		if (currentStoryScene != STORY_SCENE_WORK_MID) changeStoryScene(STORY_SCENE_WORK_MID);
	}
	else if (storyTicks < 270) {
		if (currentStoryScene != STORY_SCENE_WORK_BURNOUT) changeStoryScene(STORY_SCENE_WORK_BURNOUT);
	}
	else if (storyTicks < 360) {
		if (currentStoryScene != STORY_SCENE_REFLECTION) changeStoryScene(STORY_SCENE_REFLECTION);
	}
	else if (storyTicks < 450) {
		if (currentStoryScene != STORY_SCENE_METRO_CITY) changeStoryScene(STORY_SCENE_METRO_CITY);
	}
	else if (storyTicks < 540) {
		if (currentStoryScene != STORY_SCENE_METRO_RURAL) changeStoryScene(STORY_SCENE_METRO_RURAL);
	}
	else if (storyTicks < 660) {
		if (currentStoryScene != STORY_SCENE_TRACTOR_ARRIVE) changeStoryScene(STORY_SCENE_TRACTOR_ARRIVE);
	}
	else {
		// Cutscene finished naturally -> Go to Loading Screen
		gameState = STATE_LOADING;
		return;
	}

	if (currentStoryScene < STORY_SCENE_COUNT) {
		updateTypewriterText(STORY_DIALOGUES[currentStoryScene]);
	}
}

// ============================================================
// RENDERING
// ============================================================

void drawStoryline() {
	// Force draw color to solid white so BMP textures render true-to-color
	iSetColor(255, 255, 255);

	// 1. Render Current Scene Asset
	switch (currentStoryScene) {
	case STORY_SCENE_WORK_HIGH:     iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg1); break;
	case STORY_SCENE_WORK_MID:      iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg2); break;
	case STORY_SCENE_WORK_BURNOUT:  iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg3); break;
	case STORY_SCENE_REFLECTION:    iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg4); break;
	case STORY_SCENE_METRO_CITY:    iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg5); break;
	case STORY_SCENE_METRO_RURAL:   iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg6); break;
	case STORY_SCENE_TRACTOR_ARRIVE:iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, storyImg7); break;
	default: break;
	}

	// 2. Render Text Box Overlay & Subtitles
	iSetColor(0, 0, 0);
	iFilledRectangle(40, 30, SCREEN_WIDTH - 80, 60);

	iSetColor(255, 255, 255);
	iRectangle(40, 30, SCREEN_WIDTH - 80, 60);
	iText(60, 52, currentTypeText, GLUT_BITMAP_HELVETICA_18);

	// 3. Render Skip Prompt
	iSetColor(180, 180, 180);
	iText(SCREEN_WIDTH - 170, SCREEN_HEIGHT - 30, "Press [SPACE] to Skip", GLUT_BITMAP_HELVETICA_12);
}

// ============================================================
// INPUT HANDLER
// ============================================================

void handleStorylineKeyboard(unsigned char key) {
	if (key == ' ' || key == '\r') {
		// Player pressed Space/Enter to skip -> Go to Loading Screen
		gameState = STATE_LOADING;
	}
}

#endif // STORYLINE_H