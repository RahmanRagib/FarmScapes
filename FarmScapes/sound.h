#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#pragma comment(lib, "winmm.lib")

// Global state variable to prevent music from restarting on every iGraphics frame render loop
static int currentMusicTrack = -1;

// Base function to play any MP3 track
inline void playMusicTrack(const char* filePath, int trackId) {
    if (currentMusicTrack == trackId) {
        return; // Audio is already playing, do nothing
    }

    // Stop and close any audio currently active under the alias 'bgm'
    mciSendStringA("close bgm", NULL, 0, NULL);

    // Build the MCI command string (handles folder paths with spaces safely)
    char command[512];
    sprintf_s(command, sizeof(command), "open \"%s\" type mpegvideo alias bgm", filePath);

    // Open and play the MP3 on continuous repeat
    mciSendStringA(command, NULL, 0, NULL);
    mciSendStringA("play bgm repeat", NULL, 0, NULL);

    currentMusicTrack = trackId;
}

// Convenience wrapper for Default Background Music
inline void playDefaultBGM() {
    playMusicTrack("Audios/background.mp3", 1);
}

// Convenience wrapper for Marketplace Music
inline void playMarketBGM() {
    playMusicTrack("Audios/market.mp3", 99);
}

// Stop audio playback completely
inline void stopMusic() {
    mciSendStringA("close bgm", NULL, 0, NULL);
    currentMusicTrack = -1;
}

#endif // SOUND_H
