#pragma once
#include "CommonInclude.h"
#include "FMOD.hpp"
#include <map>
#include <string>
#include <vector>

class SoundManager {
public:
    void Initialize();
    void Release();
    void Update(); // Added for channel cleanup

    static SoundManager* GetInstance() {
        static SoundManager instance;
        return &instance;
    }

    void mPlaySound(const std::string& SoundName, bool loop);

private:
    SoundManager() : mSystem(nullptr), mBGMChannel(nullptr), mSEGroup(nullptr), mBGMGroup(nullptr) {
        for (int i = 0; i < 30; i++) mSE[i] = nullptr;
        for (int i = 0; i < 10; i++) mBGM[i] = nullptr;
    }
    ~SoundManager() { Release(); }

    FMOD::System* mSystem;
    FMOD::Channel* mBGMChannel;
    FMOD::ChannelGroup* mSEGroup;
    FMOD::ChannelGroup* mBGMGroup;
    FMOD::Sound* mSE[30];
    FMOD::Sound* mBGM[10];
    std::map<std::string, FMOD::Sound*> mSoundMap;
    std::vector<FMOD::Channel*> mSEChannels;
};