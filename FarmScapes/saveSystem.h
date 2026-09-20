#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void deleteSaveSlot(int slotNumber);

// Save Game Function (Creates or overwrites a save slot file)
void saveGameProgress(int slot) {
	char filename[30];
	sprintf(filename, "save_slot%d.txt", slot);

	FILE *file = fopen(filename, "w");
	if (file != NULL) {
		fprintf(file, "COINS: %d\n", playerGold);
		fprintf(file, "LEVEL2: %d\n", level2Unlocked);
		fprintf(file, "LEVEL3: %d\n", level3Unlocked);
		fprintf(file, "SEED_RICE: %d\n", seedRice);
		fprintf(file, "SEED_TOMATO: %d\n", seedTomato);
		fprintf(file, "SEED_BERRY: %d\n", seedBerry);
		fprintf(file, "CROP_RICE: %d\n", cropRiceCount);
		fprintf(file, "CROP_TOMATO: %d\n", cropTomatoCount);
		fprintf(file, "CROP_BERRY: %d\n", cropBerryCount);
		fprintf(file, "FEED: %d\n", countFeed);
		fprintf(file, "EGG: %d\n", countEgg);
		fprintf(file, "MILK: %d\n", countMilk);
		fprintf(file, "WOOL: %d\n", countWool);
		fclose(file);
	}
}

void deleteSaveSlot(int slotNumber)
{
	char filename[50];
	sprintf(filename, "save_slot_%d.txt", slotNumber);

	// Delete the save file from disk
	remove(filename);
}

// Load Game Function (Reads progress from a save slot file)
void loadGameProgress(int slot) {
	char filename[30];
	sprintf(filename, "save_slot%d.txt", slot);

	FILE *file = fopen(filename, "r");
	if (file != NULL) {
		fscanf(file, "COINS: %d", &playerGold);
		fscanf(file, "LEVEL2: %d", &level2Unlocked);
		fscanf(file, "LEVEL3: %d", &level3Unlocked);
		fscanf(file, "SEED_RICE: %d", &seedRice);
		fscanf(file, "SEED_TOMATO: %d", &seedTomato);
		fscanf(file, "SEED_BERRY: %d", &seedBerry);
		fscanf(file, "CROP_RICE: %d", &cropRiceCount);
		fscanf(file, "CROP_TOMATO: %d", &cropTomatoCount);
		fscanf(file, "CROP_BERRY: %d", &cropBerryCount);
		fscanf(file, "FEED: %d", &countFeed);
		fscanf(file, "EGG: %d", &countEgg);
		fscanf(file, "MILK: %d", &countMilk);
		fscanf(file, "WOOL: %d", &countWool);
		fclose(file);
	}
}

// Delete Game Progress Function (Removes the save slot file)
void deleteGameProgress(int slot) {
	char filename[30];
	sprintf(filename, "save_slot%d.txt", slot);
	remove(filename);
}

// Check if Save Slot Exists (Returns 1 if file exists, 0 otherwise)
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

// Handle Slot Action Logic (New Game, Load Game, or Delete Game)
void handleSlotAction(int slot) {
	if (slotActionMode == 1) // NEW GAME
	{
		saveGameProgress(slot);
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
			// Fallback: if slot is empty when loading, create a new game
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