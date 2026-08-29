#ifndef ANIMALGROWTH_H
#define ANIMALGROWTH_H

#include <stdio.h>

#ifndef STATE_LEVEL_2
#define STATE_LEVEL_2 6
#endif

#define ANIMAL_HEN   0
#define ANIMAL_COW   1
#define ANIMAL_SHEEP 2

#define MAX_ANIMALS_PER_TYPE 4

struct Animal {
	int x, y;
	int type;            // 0 = Hen, 1 = Cow, 2 = Sheep
	int fedState;        // 0 = Hungry, 1 = Fed
	int produceTimer;    // Timer ticks while fed
	int hasProduce;      // 1 if egg/milk/wool is ready for harvest
	int isAlive;         // 1 if active in pen
};

// --- ANIMAL & FARM MAN EXTERNS ---
extern int gameState;
extern struct Animal hens[MAX_ANIMALS_PER_TYPE];
extern struct Animal cows[MAX_ANIMALS_PER_TYPE];
extern struct Animal sheep[MAX_ANIMALS_PER_TYPE];
extern int henCount, cowCount, sheepCount;

extern int farmmanX, farmmanY;

// --- ANIMAL GROWTH UPDATE (Called every 1 sec via iSetTimer) ---
inline void updateAnimalGrowth() {
	if (gameState != STATE_LEVEL_2) return;

	// 1. Hens Produce Eggs (Timer: 3 seconds)
	for (int i = 0; i < henCount; i++) {
		if (hens[i].isAlive && hens[i].fedState == 1 && !hens[i].hasProduce) {
			hens[i].produceTimer++;
			if (hens[i].produceTimer >= 3) {
				hens[i].hasProduce = 1;
				hens[i].fedState = 0;
				hens[i].produceTimer = 0;
			}
		}
	}

	// 2. Cows Produce Milk (Timer: 6 seconds)
	for (int i = 0; i < cowCount; i++) {
		if (cows[i].isAlive && cows[i].fedState == 1 && !cows[i].hasProduce) {
			cows[i].produceTimer++;
			if (cows[i].produceTimer >= 6) {
				cows[i].hasProduce = 1;
				cows[i].fedState = 0;
				cows[i].produceTimer = 0;
			}
		}
	}

	// 3. Sheep Produce Wool (Timer: 8 seconds)
	for (int i = 0; i < sheepCount; i++) {
		if (sheep[i].isAlive && sheep[i].fedState == 1 && !sheep[i].hasProduce) {
			sheep[i].produceTimer++;
			if (sheep[i].produceTimer >= 8) {
				sheep[i].hasProduce = 1;
				sheep[i].fedState = 0;
				sheep[i].produceTimer = 0;
			}
		}
	}
}

#endif // ANIMALGROWTH_H
