#ifndef ANIMAL_MOVEMENT_H
#define ANIMAL_MOVEMENT_H

#include <cmath>
#include <cstdlib>

// --- SINGLE CENTRAL DEFINITION OF ANIMAL ---
struct Animal {
	float x, y;
	float targetX, targetY;
	int pauseTimer;
	int type;
	int fedState;
	int produceTimer;
	int hasProduce;
	int isAlive;
};

// --- PEN BOUNDARIES ---
#define HEN_PEN_MIN_X   60.0f
#define HEN_PEN_MAX_X  220.0f
#define HEN_PEN_MIN_Y  140.0f
#define HEN_PEN_MAX_Y  220.0f

#define COW_PEN_MIN_X  280.0f
#define COW_PEN_MAX_X  460.0f
#define COW_PEN_MIN_Y  200.0f
#define COW_PEN_MAX_Y  320.0f

#define SHEEP_PEN_MIN_X 540.0f
#define SHEEP_PEN_MAX_X 700.0f
#define SHEEP_PEN_MIN_Y 170.0f
#define SHEEP_PEN_MAX_Y 280.0f

// Pick a random destination within a pen
inline void setRandomTarget(Animal* a, float minX, float maxX, float minY, float maxY) {
	a->targetX = minX + (rand() % (int)(maxX - minX));
	a->targetY = minY + (rand() % (int)(maxY - minY));
}

// Move individual animal toward its target
inline void updateSingleAnimal(Animal* a, float minX, float maxX, float minY, float maxY, float speed) {
	if (!a->isAlive) return;

	if (a->pauseTimer > 0) {
		a->pauseTimer--;
		return;
	}

	float dx = a->targetX - a->x;
	float dy = a->targetY - a->y;
	float dist = (float)sqrt(dx * dx + dy * dy);

	if (dist < 3.0f || (a->targetX == 0 && a->targetY == 0)) {
		setRandomTarget(a, minX, maxX, minY, maxY);
		a->pauseTimer = 40 + (rand() % 80);
		return;
	}

	a->x += (dx / dist) * speed;
	a->y += (dy / dist) * speed;

	if (a->x < minX) a->x = minX;
	if (a->x > maxX) a->x = maxX;
	if (a->y < minY) a->y = minY;
	if (a->y > maxY) a->y = maxY;
}

// Update all active animals
inline void updateAnimalMovements(Animal hens[], int henCount, Animal cows[], int cowCount, Animal sheep[], int sheepCount) {
	float moveSpeed = 0.35f;

	for (int i = 0; i < henCount; i++) {
		updateSingleAnimal(&hens[i], HEN_PEN_MIN_X, HEN_PEN_MAX_X, HEN_PEN_MIN_Y, HEN_PEN_MAX_Y, moveSpeed);
	}
	for (int i = 0; i < cowCount; i++) {
		updateSingleAnimal(&cows[i], COW_PEN_MIN_X, COW_PEN_MAX_X, COW_PEN_MIN_Y, COW_PEN_MAX_Y, moveSpeed);
	}
	for (int i = 0; i < sheepCount; i++) {
		updateSingleAnimal(&sheep[i], SHEEP_PEN_MIN_X, SHEEP_PEN_MAX_X, SHEEP_PEN_MIN_Y, SHEEP_PEN_MAX_Y, moveSpeed);
	}
}

#endif
