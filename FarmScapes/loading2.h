#ifndef LOADING2_H
#define LOADING2_H

inline void drawLevel2Loading() {
    // 1. Green Background (800 x 600 px)
    iSetColor(34, 139, 34);
    iFilledRectangle(0, 0, 800, 600);

    // 2. Level 2 Loading Title Banner Image (480 x 80 px)
    iSetColor(255, 255, 255);
    iShowBMPAlternative2(160, 480, (char*)"assets/lc_title_2.bmp", 0xFFFFFF);

    // 3. Level 2 How to Play Instruction Box Image (600 x 360 px)
    iShowBMPAlternative2(100, 90, (char*)"assets/instrc_2.bmp", 0xFFFFFF);

    // 4. Click Anywhere to Start Prompt Image (400 x 50 px)
    iShowBMPAlternative2(200, 25, (char*)"assets/start.bmp", 0xFFFFFF);
}

#endif // LOADING2_H
