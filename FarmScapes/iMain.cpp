#define _CRT_SECURE_NO_WARNINGS

#include "iGraphics.h"
#include "bitmap_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

// --- SCREEN & RENDER DIMENSIONS ---
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Fixed Player Sprite Size
#define PLAYER_WIDTH 48
#define PLAYER_HEIGHT 48

// Game States
#define STATE_MENU 0
#define STATE_LEVEL_1 1
#define STATE_SETTINGS 2
#define STATE_CREDITS 3
#define STATE_LOADING 4
#define STATE_TOWN 5
#define STATE_LEVEL_2 6

int gameState = STATE_MENU;
int loadingTimer = 0;
int musicOn = 1;
int eKeyPressedLastFrame = 0;

// --- TOWN & SEASON VARIABLES ---
int playerX = 400, playerY = 300, playerSpeed = 8;
int currentSeason = 0, seasonTimer = 40; // 0=Summer, 1=Rainy, 2=Winter
int showDialogue = 0;
char dialogueText[200] = "";
char npcName[50] = "";
int level2Unlocked = 0, level3Unlocked = 0;

#include "toggleMusic.h"
#include "menu.h"
#include "settings.h"
#include "credits.h"
#include "loading.h"
#include "updatecropgrowth.h"
#include "drawlevel1.h"
#include "animalgrowth.h"  // Included first so MAX_ANIMALS_PER_TYPE is defined
#include "drawlevel2.h"
#include "drawTown.h"

int playerGold = 0;

// --- ANIMAL STATE VARIABLES FOR LEVEL 2 ---
int henCount = 0, cowCount = 0, sheepCount = 0;
struct Animal hens[MAX_ANIMALS_PER_TYPE];
struct Animal cows[MAX_ANIMALS_PER_TYPE];
struct Animal sheep[MAX_ANIMALS_PER_TYPE];

int countFeed = 5;
int countEgg = 0, countMilk = 0, countWool = 0;
int feedBuyPrice = 5;
int eggSellPrice = 15, milkSellPrice = 30, woolSellPrice = 45;
int henBuyPrice = 30, cowBuyPrice = 100, sheepBuyPrice = 70;
int selectedRanchTool = 1; // 1 = Feed, 2 = Collect
int isRanchMarketOpen = 0;

// --- RANCH TIMER VARIABLES ---
int ranchTimer = 0;          // 20-second countdown timer
int isRanchTimerActive = 0;  // Flag to check if timer is running

// CROP PRICES (MAX PROFIT = $5 EACH)
int riceBuyPrice = 5, riceSellPrice = 10;
int tomatoBuyPrice = 15, tomatoSellPrice = 20;
int berryBuyPrice = 25, berrySellPrice = 30;

// STARTING SEEDS
int seedRice = 9;
int seedTomato = 0;
int seedBerry = 0;

// INVENTORY
int cropRiceCount = 0;
int cropTomatoCount = 0;
int cropBerryCount = 0;

int isMarketOpen = 0;
int massPlowUnlocked = 0;
int showCapWarning = 0;

int selectedTool = 0;
int batchTimer = 0;
int batchActive = 0;
int hasRottenCrop = 0;

Tile farmGrid[GRID_ROWS][GRID_COLS];

// --- BOUNDARY CHECK HELPER ---
int isWithinBounds(int x, int y) {
	if (x < 0 || x > SCREEN_WIDTH - PLAYER_WIDTH) return 0;
	if (y < 0 || y > SCREEN_HEIGHT - PLAYER_HEIGHT) return 0;
	return 1;
}

void updatePlayer() {}

void iDraw() {
	iClear();

	if (gameState == STATE_MENU) drawMenu();
	else if (gameState == STATE_LOADING) drawLoading();
	else if (gameState == STATE_TOWN) drawTown();
	else if (gameState == STATE_LEVEL_1) drawLevel1();
	else if (gameState == STATE_LEVEL_2) drawLevel2();
	else if (gameState == STATE_SETTINGS) drawSettings();
	else if (gameState == STATE_CREDITS) drawCredits();
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

		// 1. MENU STATE
		if (gameState == STATE_MENU) {
			if (mx >= 290 && mx <= 510 && my >= 410 && my <= 480) {
				gameState = STATE_LOADING;
				loadingTimer = 0;
			}
			else if (mx >= 290 && mx <= 510 && my >= 320 && my <= 390) gameState = STATE_SETTINGS;
			else if (mx >= 290 && mx <= 510 && my >= 230 && my <= 300) gameState = STATE_CREDITS;
			else if (mx >= 290 && mx <= 510 && my >= 140 && my <= 210) {
				mciSendString("close bgmusic", NULL, 0, NULL);
				exit(0);
			}
		}
		// 2. SETTINGS / CREDITS STATE
		else if (gameState == STATE_SETTINGS) {
			if (mx >= 290 && mx <= 510 && my >= 340 && my <= 410) toggleMusic();
			else if (mx >= 290 && mx <= 510 && my >= 220 && my <= 290) gameState = STATE_MENU;
		}
		else if (gameState == STATE_CREDITS) {
			if (mx >= 290 && mx <= 510 && my >= 140 && my <= 210) gameState = STATE_MENU;
		}
		// 3. TOWN STATE
		else if (gameState == STATE_TOWN) {
			if (mx >= 670 && mx <= 780 && my >= 20 && my <= 60) {
				gameState = STATE_MENU;
				return;
			}
		}
		// 4. LEVEL 1 STATE
		else if (gameState == STATE_LEVEL_1) {
			if (showCapWarning) {
				showCapWarning = 0;
			}

			// Market Button
			if (mx >= 420 && mx <= 520 && my >= 552 && my <= 586) {
				isMarketOpen = !isMarketOpen;
				return;
			}

			// Explore Town Button
			if (mx >= 535 && mx <= 655 && my >= 552 && my <= 586) {
				gameState = STATE_TOWN;
				return;
			}

			// Menu Button
			if (mx >= 670 && mx <= 780 && my >= 552 && my <= 586) {
				gameState = STATE_MENU;
				return;
			}

			// Market Overlay
			if (isMarketOpen) {
				if (mx >= 600 && mx <= 680 && my >= 80 && my <= 110) {
					isMarketOpen = 0;
					return;
				}

				// Sell Crops
				if (mx >= 330 && mx <= 395 && my >= 370 && my <= 392 && cropRiceCount > 0) {
					cropRiceCount--; playerGold += riceSellPrice;
				}
				else if (mx >= 330 && mx <= 395 && my >= 330 && my <= 352 && cropTomatoCount > 0) {
					cropTomatoCount--; playerGold += tomatoSellPrice;
				}
				else if (mx >= 330 && mx <= 395 && my >= 290 && my <= 312 && cropBerryCount > 0) {
					cropBerryCount--; playerGold += berrySellPrice;
				}

				// Buy Seeds
				if (mx >= 600 && mx <= 665 && my >= 370 && my <= 392 && playerGold >= riceBuyPrice) {
					playerGold -= riceBuyPrice; seedRice++;
				}
				else if (mx >= 600 && mx <= 665 && my >= 330 && my <= 352 && playerGold >= tomatoBuyPrice) {
					playerGold -= tomatoBuyPrice; seedTomato++;
				}
				else if (mx >= 600 && mx <= 665 && my >= 290 && my <= 312 && playerGold >= berryBuyPrice) {
					playerGold -= berryBuyPrice; seedBerry++;
				}

				// Mass-Plow Upgrade
				if (!massPlowUnlocked && mx >= 380 && mx <= 510 && my >= 188 && my <= 214) {
					if (playerGold >= 1500) {
						playerGold -= 1500;
						massPlowUnlocked = 1;
					}
				}
				return;
			}

			// Mass-Plow Button
			if (massPlowUnlocked && mx >= 90 && mx <= 160 && my >= 28 && my <= 72) {
				for (int r = 0; r < GRID_ROWS; r++) {
					for (int c = 0; c < GRID_COLS; c++) {
						if (farmGrid[r][c].state == CROP_EMPTY || farmGrid[r][c].state == CROP_ROTTEN) {
							farmGrid[r][c].state = CROP_PLOWED;
							farmGrid[r][c].growTimer = 0;
						}
					}
				}
				return;
			}

			// Toolbar Selection
			if (my >= 20 && my <= 80) {
				if (mx >= 160 && mx <= 270) selectedTool = 1;
				if (mx >= 280 && mx <= 390) selectedTool = 2;
				if (mx >= 400 && mx <= 510) selectedTool = 3;
				if (mx >= 520 && mx <= 640) selectedTool = 4;
			}

			// Tile Interactions
			for (int r = 0; r < GRID_ROWS; r++) {
				for (int c = 0; c < GRID_COLS; c++) {
					Tile *t = &farmGrid[r][c];

					if (mx >= t->x && mx <= t->x + 80 && my >= t->y && my <= t->y + 80) {
						if (selectedTool == 1) {
							if (t->state == CROP_EMPTY || t->state == CROP_ROTTEN) {
								t->state = CROP_PLOWED;
								t->growTimer = 0;
							}
						}
						else if (selectedTool == 2 && t->state == CROP_PLOWED) {
							if (seedBerry > 0) {
								seedBerry--;
								t->cropType = 2;
								t->state = BERRY_TREE;
								t->growTimer = 0;
								if (!batchActive) {
									batchActive = 1;
									batchTimer = 20;
								}
							}
							else if (seedTomato > 0) {
								seedTomato--;
								t->cropType = 1;
								t->state = CROP_SEEDED;
								t->growTimer = 0;
								if (!batchActive) {
									batchActive = 1;
									batchTimer = 20;
								}
							}
							else if (seedRice > 0) {
								seedRice--;
								t->cropType = 0;
								t->state = CROP_SEEDED;
								t->growTimer = 0;
								if (!batchActive) {
									batchActive = 1;
									batchTimer = 20;
								}
							}
						}
						else if (selectedTool == 3) {
							if (t->state == CROP_SEEDED || t->state == BERRY_TREE) {
								t->state = CROP_WATERED;
								t->growTimer = 0;
							}
						}
						else if (selectedTool == 4) {
							if (t->state == CROP_READY) {
								if (cropRiceCount < MAX_INVENTORY_CAP) {
									t->state = CROP_EMPTY;
									cropRiceCount++;
								}
								else { showCapWarning = 1; }
							}
							else if (t->state == TOMATO_READY) {
								if (cropTomatoCount < MAX_INVENTORY_CAP) {
									t->state = CROP_EMPTY;
									cropTomatoCount++;
								}
								else { showCapWarning = 1; }
							}
							else if (t->state == BERRY_READY) {
								if (cropBerryCount < MAX_INVENTORY_CAP) {
									t->state = CROP_EMPTY;
									cropBerryCount++;
								}
								else { showCapWarning = 1; }
							}

							int activeCrops = 0;
							for (int r2 = 0; r2 < GRID_ROWS; r2++) {
								for (int c2 = 0; c2 < GRID_COLS; c2++) {
									int s = farmGrid[r2][c2].state;
									if (s == CROP_SEEDED || s == CROP_WATERED || s == CROP_READY ||
										s == TOMATO_READY || s == BERRY_TREE || s == BERRY_READY) {
										activeCrops++;
									}
								}
							}

							if (activeCrops == 0) {
								batchActive = 0;
								batchTimer = 0;
								hasRottenCrop = 0;
							}
						}
					}
				}
			}
		}
		// 5. LEVEL 2 STATE
		else if (gameState == STATE_LEVEL_2) {
			// Top HUD Navigation Buttons
			if (my >= 552 && my <= 586) {
				if (mx >= 430 && mx <= 530) { isRanchMarketOpen = !isRanchMarketOpen; return; }
				if (mx >= 545 && mx <= 655) { gameState = STATE_TOWN; return; }
				if (mx >= 670 && mx <= 780) { gameState = STATE_MENU; return; }
			}

			// Ranch Market Overlay
			if (isRanchMarketOpen) {
				if (mx >= 600 && mx <= 680 && my >= 90 && my <= 120) {
					isRanchMarketOpen = 0;
					return;
				}

				if (mx >= 320 && mx <= 385) {
					if (my >= 395 && my <= 417 && countEgg > 0)  { countEgg--;  playerGold += eggSellPrice; }
					if (my >= 345 && my <= 367 && countMilk > 0) { countMilk--; playerGold += milkSellPrice; }
					if (my >= 295 && my <= 317 && countWool > 0) { countWool--; playerGold += woolSellPrice; }
				}

				if (mx >= 610 && mx <= 675) {
					if (my >= 395 && my <= 417 && playerGold >= feedBuyPrice) {
						playerGold -= feedBuyPrice;
						countFeed++;
					}
					else if (my >= 345 && my <= 367 && playerGold >= henBuyPrice) {
						for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++) {
							if (!hens[i].isAlive) {
								playerGold -= henBuyPrice;
								hens[i].isAlive = 1;
								hens[i].x = 70 + (i % 2) * 55;
								hens[i].y = 150 + (i / 2) * 55;
								if (i + 1 > henCount) henCount = i + 1;
								break;
							}
						}
					}
					else if (my >= 295 && my <= 317 && playerGold >= cowBuyPrice) {
						for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++) {
							if (!cows[i].isAlive) {
								playerGold -= cowBuyPrice;
								cows[i].isAlive = 1;
								cows[i].x = 380 + (i % 2) * 60;
								cows[i].y = 220 + (i / 2) * 60;
								if (i + 1 > cowCount) cowCount = i + 1;
								break;
							}
						}
					}
					else if (my >= 245 && my <= 267 && playerGold >= sheepBuyPrice) {
						for (int i = 0; i < MAX_ANIMALS_PER_TYPE; i++) {
							if (!sheep[i].isAlive) {
								playerGold -= sheepBuyPrice;
								sheep[i].isAlive = 1;
								sheep[i].x = 580 + (i % 2) * 60;
								sheep[i].y = 180 + (i / 2) * 60;
								if (i + 1 > sheepCount) sheepCount = i + 1;
								break;
							}
						}
					}
				}
				return;
			}

			// Upper Right Toolbar Selection
			if (my >= 485 && my <= 521) {
				if (mx >= 543 && mx <= 655) { selectedRanchTool = 1; return; }
				if (mx >= 667 && mx <= 779) { selectedRanchTool = 2; return; }
			}

			// Animal Interactions (Gated by Pen Proximity)
			if (isNearHenPen()) {
				for (int i = 0; i < henCount; i++) {
					if (hens[i].isAlive && mx >= hens[i].x && mx <= hens[i].x + 48 && my >= hens[i].y && my <= hens[i].y + 48) {
						if (selectedRanchTool == 1 && countFeed > 0 && hens[i].fedState == 0) {
							countFeed--; hens[i].fedState = 1; ranchTimer = 20; isRanchTimerActive = 1;
						}
						else if (selectedRanchTool == 2 && hens[i].hasProduce) {
							hens[i].hasProduce = 0; countEgg++;
						}
					}
				}
			}
			else if (isNearCowPen()) {
				for (int i = 0; i < cowCount; i++) {
					if (cows[i].isAlive && mx >= cows[i].x && mx <= cows[i].x + 48 && my >= cows[i].y && my <= cows[i].y + 48) {
						if (selectedRanchTool == 1 && countFeed > 0 && cows[i].fedState == 0) {
							countFeed--; cows[i].fedState = 1; ranchTimer = 20; isRanchTimerActive = 1;
						}
						else if (selectedRanchTool == 2 && cows[i].hasProduce) {
							cows[i].hasProduce = 0; countMilk++;
						}
					}
				}
			}
			else if (isNearSheepPen()) {
				for (int i = 0; i < sheepCount; i++) {
					if (sheep[i].isAlive && mx >= sheep[i].x && mx <= sheep[i].x + 48 && my >= sheep[i].y && my <= sheep[i].y + 48) {
						if (selectedRanchTool == 1 && countFeed > 0 && sheep[i].fedState == 0) {
							countFeed--; sheep[i].fedState = 1; ranchTimer = 20; isRanchTimerActive = 1;
						}
						else if (selectedRanchTool == 2 && sheep[i].hasProduce) {
							sheep[i].hasProduce = 0; countWool++;
						}
					}
				}
			}
		}
	}
}
void iMouseMove(int mx, int my) {}
void iPassiveMouseMove(int mx, int my) {}

// --- TIMER FUNCTION TO DECREMENT RANCH TIMER EVERY SECOND ---
void updateRanchTimer() {
	if (isRanchTimerActive && ranchTimer > 0) {
		ranchTimer--;
		if (ranchTimer == 0) {
			isRanchTimerActive = 0;
		}
	}
}

// --- CONTINUOUS GAME LOOP ---
void fixedUpdate() {
	// 1. WASD & Arrow Keys Movement for Town and Level 1
	if ((gameState == STATE_TOWN && !showDialogue) || gameState == STATE_LEVEL_1) {
		if (isKeyPressed('w') || isKeyPressed('W') || isSpecialKeyPressed(GLUT_KEY_UP)) {
			if (canWalk(playerX, playerY + playerSpeed) && isWithinBounds(playerX, playerY + playerSpeed)) playerY += playerSpeed;
		}
		if (isKeyPressed('s') || isKeyPressed('S') || isSpecialKeyPressed(GLUT_KEY_DOWN)) {
			if (canWalk(playerX, playerY - playerSpeed) && isWithinBounds(playerX, playerY - playerSpeed)) playerY -= playerSpeed;
		}
		if (isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT)) {
			if (canWalk(playerX - playerSpeed, playerY) && isWithinBounds(playerX - playerSpeed, playerY)) playerX -= playerSpeed;
		}
		if (isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
			if (canWalk(playerX + playerSpeed, playerY) && isWithinBounds(playerX + playerSpeed, playerY)) playerX += playerSpeed;
		}
	}

	// 2. Level 2 Farm Man Movement (Sideways Only via Left/Right Arrow Keys)
	if (gameState == STATE_LEVEL_2 && !isRanchMarketOpen) {
		int step = 8;
		if (isSpecialKeyPressed(GLUT_KEY_LEFT)) moveFarmMan(-step, 0);
		if (isSpecialKeyPressed(GLUT_KEY_RIGHT)) moveFarmMan(step, 0);
	}

	// 3. DIALOGUE TOGGLE & LEVEL TRANSITION
	if (gameState == STATE_TOWN) {
		int eIsDown = isKeyPressed('e') || isKeyPressed('E');
		if (eIsDown && !eKeyPressedLastFrame) {
			if (showDialogue) {
				// Close dialogue window
				showDialogue = 0;

				// Enter level ONLY if conditions are met
				if (strcmp(npcName, "Nadira") == 0) {
					gameState = STATE_LEVEL_1;
				}
				else if (strcmp(npcName, "Ragib") == 0 && level2Unlocked) {
					gameState = STATE_LEVEL_2;
				}
				else if (strcmp(npcName, "Anika") == 0 && level3Unlocked) {
					// gameState = STATE_LEVEL_3;
				}
				// If level is locked, showDialogue set to 0 closes dialogue box and stays in STATE_TOWN
			}
			else {
				// Open dialogue prompt based on proximity
				if (playerX >= 480 && playerX <= 700 && playerY >= 320 && playerY <= 460) {
					strcpy(npcName, "Nadira");
					strcpy(dialogueText, "Welcome to the Farm! Press E again to enter Level 1.");
					showDialogue = 1;
				}
				else if (playerX >= 450 && playerX <= 680 && playerY >= 210 && playerY <= 310) {
					strcpy(npcName, "Ragib");
					if (playerGold >= 100) {
						level2Unlocked = 1;
						strcpy(dialogueText, "You have 100 gold! Press E again to enter Level 2.");
					}
					else {
						strcpy(dialogueText, "Welcome to the Ranch! Earn 100 gold in Level 1 first.");
					}
					showDialogue = 1;
				}
				else if (playerX >= 450 && playerX <= 680 && playerY >= 100 && playerY <= 200) {
					strcpy(npcName, "Anika");
					strcpy(dialogueText, level3Unlocked ? "Entering Fishery..." : "Welcome to the Fishery! Clear Level 2 first.");
					showDialogue = 1;
				}
			}
		}
		eKeyPressedLastFrame = eIsDown;
	}
}

void iKeyboard(unsigned char key) {
	if (gameState == STATE_LEVEL_2) {
		if (isRanchMarketOpen) {
			if (key == 27) isRanchMarketOpen = 0; // ESC to close market
		}
		else {
			if (key == ' ' || key == '\r') {
				if (selectedRanchTool == 1) {
					feedAnimalsByFarmMan();
				}
				else if (selectedRanchTool == 2) {
					collectProduceByFarmMan();
				}
			}
		}
	}
}

// --- 3. ANIMATION TIMER CALLBACK ---
void iAnim() {
	fixedUpdate(); // Ensures continuous frame movement executes
}

void updateSeasonTimer() {
	if (seasonTimer > 0) seasonTimer--;
	else {
		currentSeason = (currentSeason + 1) % 3;
		seasonTimer = 40;
	}
}

void updateLoading() {
	if (gameState == STATE_LOADING) {
		loadingTimer += 2;
		if (loadingTimer >= 100) {
			gameState = STATE_LEVEL_1;
			loadingTimer = 0;
		}
	}
}



int main() {
	initFarmGrid();
	initLevel2();
	initAudio();

	iSetTimer(1000, updateCropGrowth);
	iSetTimer(1000, updateAnimalGrowth);
	iSetTimer(1000, updateSeasonTimer);
	iSetTimer(1000, updateRanchTimer); // 20s countdown ticker per second
	iSetTimer(50, updateLoading);
	iSetTimer(20, iAnim);

	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "FarmScapes - 2D Farming Simulator");
	iStart();
	return 0;
}
