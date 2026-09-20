#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ============================================================
// EXTERNAL GLOBAL VARIABLES
// ============================================================

extern int gameState;
extern int slotActionMode;
extern int loadingTimer;

extern int playerGold;
extern int level2Unlocked;
extern int level3Unlocked;

// Seeds
extern int seedRice;
extern int seedTomato;
extern int seedBerry;

// Crops
extern int cropRiceCount;
extern int cropTomatoCount;
extern int cropBerryCount;

// Ranch & Animals
extern int countFeed;
extern int countEgg;
extern int countMilk;
extern int countWool;
extern int henCount;
extern int cowCount;
extern int sheepCount;

// States needed from iMain
#ifndef STATE_LEVEL_1
#define STATE_LEVEL_1 1
#endif

#ifndef STATE_LOADING
#define STATE_LOADING 4
#endif

#ifndef STATE_STORYLINE
#define STATE_STORYLINE 12
#endif

// Forward declarations
void startStoryline();

// ============================================================
// SAVE / LOAD SYSTEM
// ============================================================

// Save Game Function (Stores everything into the text file)
void saveGameProgress(int slot) {
	char filename[50];
	sprintf(filename, "save_slot_%d.txt", slot);

	FILE *file = fopen(filename, "w");
	if (file != NULL) {
		// 1. Core State & Progression
		fprintf(file, "GAME_STATE: %d\n", gameState);
		fprintf(file, "COINS: %d\n", playerGold);
		fprintf(file, "LEVEL2: %d\n", level2Unlocked);
		fprintf(file, "LEVEL3: %d\n", level3Unlocked);

		// 2. Seeds Inventory
		fprintf(file, "SEED_RICE: %d\n", seedRice);
		fprintf(file, "SEED_TOMATO: %d\n", seedTomato);
		fprintf(file, "SEED_BERRY: %d\n", seedBerry);

		// 3. Harvested Crops Inventory
		fprintf(file, "CROP_RICE: %d\n", cropRiceCount);
		fprintf(file, "CROP_TOMATO: %d\n", cropTomatoCount);
		fprintf(file, "CROP_BERRY: %d\n", cropBerryCount);

		// 4. Ranch Items & Animal Counts
		fprintf(file, "FEED: %d\n", countFeed);
		fprintf(file, "EGG: %d\n", countEgg);
		fprintf(file, "MILK: %d\n", countMilk);
		fprintf(file, "WOOL: %d\n", countWool);
		fprintf(file, "HEN_COUNT: %d\n", henCount);
		fprintf(file, "COW_COUNT: %d\n", cowCount);
		fprintf(file, "SHEEP_COUNT: %d\n", sheepCount);

		fclose(file);
	}
}

// Load Game Function (Reads everything back from the text file)
void loadGameProgress(int slot) {
	char filename[50];
	sprintf(filename, "save_slot_%d.txt", slot);

	FILE *file = fopen(filename, "r");
	if (file != NULL) {
		// Read values in exact matching format
		fscanf(file, "GAME_STATE: %d\n", &gameState);
		fscanf(file, "COINS: %d\n", &playerGold);
		fscanf(file, "LEVEL2: %d\n", &level2Unlocked);
		fscanf(file, "LEVEL3: %d\n", &level3Unlocked);

		fscanf(file, "SEED_RICE: %d\n", &seedRice);
		fscanf(file, "SEED_TOMATO: %d\n", &seedTomato);
		fscanf(file, "SEED_BERRY: %d\n", &seedBerry);

		fscanf(file, "CROP_RICE: %d\n", &cropRiceCount);
		fscanf(file, "CROP_TOMATO: %d\n", &cropTomatoCount);
		fscanf(file, "CROP_BERRY: %d\n", &cropBerryCount);

		fscanf(file, "FEED: %d\n", &countFeed);
		fscanf(file, "EGG: %d\n", &countEgg);
		fscanf(file, "MILK: %d\n", &countMilk);
		fscanf(file, "WOOL: %d\n", &countWool);
		fscanf(file, "HEN_COUNT: %d\n", &henCount);
		fscanf(file, "COW_COUNT: %d\n", &cowCount);
		fscanf(file, "SHEEP_COUNT: %d\n", &sheepCount);

		fclose(file);
	}
}

// Check if a save slot file exists
bool checkIfSlotExists(int slotNumber)
{
	char filename[50];
	sprintf(filename, "save_slot_%d.txt", slotNumber);
	FILE *fp = fopen(filename, "r");
	if (fp)
	{
		fclose(fp);
		return true;
	}
	return false;
}

// Get player score/gold saved in a slot
int getSlotScore(int slotNumber)
{
	char filename[50];
	sprintf(filename, "save_slot_%d.txt", slotNumber);
	FILE *fp = fopen(filename, "r");
	int state = 0, gold = 0;
	if (fp)
	{
		fscanf(fp, "GAME_STATE: %d\n", &state);
		fscanf(fp, "COINS: %d\n", &gold);
		fclose(fp);
	}
	return gold;
}

// Delete a save slot
void deleteSaveSlot(int slotNumber)
{
	char filename[50];
	sprintf(filename, "save_slot_%d.txt", slotNumber);
	remove(filename);
}

// Alias to maintain compatibility
void deleteGameProgress(int slotNumber)
{
	deleteSaveSlot(slotNumber);
}

// Handle Slot Action Logic (New Game, Load Game, or Delete Game)
void handleSlotAction(int slot) {
	if (slotActionMode == 1) // NEW GAME - Reset everything to fresh start
	{
		playerGold = 0;
		seedRice = 9;       // Fresh starting rice seeds
		seedTomato = 0;
		seedBerry = 0;
		cropRiceCount = 0;
		cropTomatoCount = 0;
		cropBerryCount = 0;
		countFeed = 5;      // Fresh starting ranch feed
		countEgg = 0;
		countMilk = 0;
		countWool = 0;
		henCount = 0;
		cowCount = 0;
		sheepCount = 0;
		level2Unlocked = 0; // Lock levels for a true fresh game
		level3Unlocked = 0;

		saveGameProgress(slot); // Save the fresh default values
		startStoryline();       // Trigger storyline cutscene
		gameState = STATE_STORYLINE;
	}
	else if (slotActionMode == 2) // LOAD GAME
	{
		if (checkIfSlotExists(slot))
		{
			loadGameProgress(slot);
			gameState = STATE_LOADING;
			loadingTimer = 0;
		}
		else
		{
			// Fallback if slot is empty: treat it as a new game
			playerGold = 0;
			seedRice = 9;
			seedTomato = 0;
			seedBerry = 0;
			cropRiceCount = 0;
			cropTomatoCount = 0;
			cropBerryCount = 0;
			countFeed = 5;
			countEgg = 0;
			countMilk = 0;
			countWool = 0;
			henCount = 0;
			cowCount = 0;
			sheepCount = 0;
			level2Unlocked = 0;
			level3Unlocked = 0;

			saveGameProgress(slot);
			startStoryline();
			gameState = STATE_STORYLINE;
		}
	}
	else if (slotActionMode == 3) // DELETE GAME
	{
		deleteSaveSlot(slot);
	}
}

#endif // SAVESYSTEM_H