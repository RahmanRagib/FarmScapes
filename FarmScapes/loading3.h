#ifndef LOADING3_H
#define LOADING3_H

#include "iGraphics.h"

inline void drawLevel3Loading() {
    // 1. Deep Water Blue Background (800 x 600 px)
    iSetColor(0, 105, 148);
    iFilledRectangle(0, 0, 800, 600);

    // 2. Level 3 Loading Title Banner Image (480 x 80 px)
    iSetColor(255, 255, 255);
    iShowBMPAlternative2(160, 480, (char*)"assets/lc_title_3.bmp", 0xFFFFFF);

    // 3. Level 3 How to Play Instruction Box Image (600 x 360 px)
    iShowBMPAlternative2(100, 90, (char*)"assets/instrc_3.bmp", 0xFFFFFF);

    // 4. Click Anywhere to Start Prompt Image (400 x 50 px)
    iShowBMPAlternative2(200, 25, (char*)"assets/start.bmp", 0xFFFFFF);
}

#endif // LOADING3_H
