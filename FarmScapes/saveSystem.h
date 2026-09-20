#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare external global variables defined in your main file so the header can access them
extern int gameState;
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

// Save Game Function (Stores everything into the text file)
void saveGameProgress(int slot) {
	char filename[30];
	sprintf(filename, "save_slot%d.txt", slot);

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
	char filename[30];
	sprintf(filename, "save_slot%d.txt", slot);

	FILE *file = fopen(filename, "r");
	if (file != NULL) {
		// Read values in the exact same matching format
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

// Delete Game Progress Function
void deleteGameProgress(int slot) {
	char filename[30];
	sprintf(filename, "save_slot%d.txt", slot);
	remove(filename);
}

// Check if Save Slot Exists
int checkIfSlotExists(int slot) {
	char filename[30];
	sprintf(filename, "save_slot%d.txt", slot);
	FILE *file = fopen(filename, "r");
	if (file != NULL) {
		fclose(file);
		return 1;
	}
	return 0;
}

// Helper function to read the gold score for the menu display text
int getSlotScore(int slot) {
	char filename[30];
	sprintf(filename, "save_slot%d.txt", slot);

	FILE *file = fopen(filename, "r");
	if (file == NULL) return 0;

	char line[50];
	int score = 0;
	while (fgets(line, sizeof(line), file)) {
		if (sscanf(line, "COINS: %d", &score) == 1) {
			fclose(file);
			return score;
		}
	}
	fclose(file);
	return 0;
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
		gameState = STATE_LEVEL_1; // Start fresh at Level 1

		saveGameProgress(slot); // Save the fresh default values
		gameState = STATE_LOADING;
		loadingTimer = 0;
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
			gameState = STATE_LEVEL_1;

			saveGameProgress(slot);
			gameState = STATE_LOADING;
			loadingTimer = 0;
		}
	}
	else if (slotActionMode == 3) // DELETE GAME
	{
		deleteGameProgress(slot);
	}
}
#endif