#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Save Game Function
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