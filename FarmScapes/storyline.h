#ifndef STORYLINE_H
#define STORYLINE_H

#include "iGraphics.h"
#include "bitmap_loader.h"
#include <string.h>

#ifndef STATE_STORYLINE
#define STATE_STORYLINE 12
#endif

#define SCENE_DURATION 90 

static int storyImg1, storyImg2, storyImg3, storyImg4, storyImg5, storyImg6, storyImg7;

typedef enum {
	STORY_SCENE_WORK_HIGH = 0,
	STORY_SCENE_WORK_MID,
	STORY_SCENE_WORK_BURNOUT,
	STORY_SCENE_REFLECTION,
	STORY_SCENE_METRO_CITY,
	STORY_SCENE_METRO_RURAL,
	STORY_SCENE_TRACTOR_ARRIVE,
	STORY_SCENE_COUNT
} StoryScene;

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
static int storyTicks = 0;
static int textCharIndex = 0;
static char currentTypeText[256] = "";

static void updateTypewriterText(const char* fullText) {
	if (!fullText) return;
	int len = (int)strlen(fullText);
	if (textCharIndex < len) {
		textCharIndex++;
	}
	strncpy(currentTypeText, fullText, textCharIndex);
	currentTypeText[textCharIndex] = '\0';
}

static void changeStoryScene(StoryScene newScene) {
	currentStoryScene = newScene;
	textCharIndex = 0;
	currentTypeText[0] = '\0';
	storyTicks = (int)newScene * SCENE_DURATION;
}

static void finishTypewriterText() {
	if (currentStoryScene < STORY_SCENE_COUNT) {
		const char* fullText = STORY_DIALOGUES[currentStoryScene];
		textCharIndex = (int)strlen(fullText);
		strcpy(currentTypeText, fullText);
	}
}

void initStorylineAssets() {
	storyImg1 = iLoadImage("assets/story_work_100.bmp");
	storyImg2 = iLoadImage("assets/story_work_50.bmp");
	storyImg3 = iLoadImage("assets/story_work_10.bmp");
	storyImg4 = iLoadImage("assets/story_reflection.bmp");
	storyImg5 = iLoadImage("assets/story_metro_city.bmp");
	storyImg6 = iLoadImage("assets/story_metro_rural.bmp");
	storyImg7 = iLoadImage("assets/story_tractor.bmp");

	storyTicks = 0;
	currentStoryScene = STORY_SCENE_WORK_HIGH;
}

void startStoryline() {
	changeStoryScene(STORY_SCENE_WORK_HIGH);
}

void updateStoryline() {
	if (gameState != STATE_STORYLINE) return;

	storyTicks++;

	int targetSceneIndex = storyTicks / SCENE_DURATION;

	// End of cutscene
	if (targetSceneIndex >= STORY_SCENE_COUNT) {
		gameState = STATE_LOADING;
		return;
	}

	// Auto-advance scene when timer threshold reached
	if (targetSceneIndex != (int)currentStoryScene) {
		changeStoryScene((StoryScene)targetSceneIndex);
	}

	if (currentStoryScene < STORY_SCENE_COUNT) {
		updateTypewriterText(STORY_DIALOGUES[currentStoryScene]);
	}
}

void drawStoryline() {
	iSetColor(255, 255, 255);

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

	// Textbox Overlay
	iSetColor(0, 0, 0);
	iFilledRectangle(40, 30, SCREEN_WIDTH - 80, 60);

	iSetColor(255, 255, 255);
	iRectangle(40, 30, SCREEN_WIDTH - 80, 60);
	iText(60, 52, currentTypeText, GLUT_BITMAP_HELVETICA_18);

}

void handleStorylineKeyboard(unsigned char key) {
	

	// 2. Advance Keys: Space, Enter, or any letter
	const char* fullText = STORY_DIALOGUES[currentStoryScene];
	if (textCharIndex < (int)strlen(fullText)) {
		// Instantly reveal text if still typing
		finishTypewriterText();
	}
	else {
		// Advance to next scene or trigger loading screen
		if (currentStoryScene + 1 < STORY_SCENE_COUNT) {
			changeStoryScene((StoryScene)(currentStoryScene + 1));
		}
		else {
			gameState = STATE_LOADING;
		}
	}
}

#endif // STORYLINE_H