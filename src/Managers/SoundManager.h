#pragma once
#include "ofMain.h"
#include <unordered_map>

class SoundManager {
    static std::unordered_map<string, ofSoundPlayer> sounds;

    public:
        static void loadSong(const string& songName, string songPath);

        static void playSong(const string &name, bool loop);

        static void stopSong(const string &name);

        static void setVolume(const string &name, double volume);

};