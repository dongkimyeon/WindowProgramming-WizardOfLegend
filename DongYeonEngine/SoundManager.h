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

    static SoundManager* GetInstance() {
        static SoundManager instance;
        return &instance;
    }

    void mPlaySound(const std::string& SoundName, bool loop);

private:
    SoundManager() : mSystem(nullptr), mBGMChannel(nullptr), mSEGroup(nullptr), mBGMGroup(nullptr), mBGM(nullptr) {
        for (int i = 0; i < 15; i++) mSE[i] = nullptr;
    }
    ~SoundManager() { Release(); }

    FMOD::System* mSystem;
    FMOD::Channel* mBGMChannel;
    FMOD::ChannelGroup* mSEGroup;
    FMOD::ChannelGroup* mBGMGroup;
    FMOD::Sound* mSE[15];
    FMOD::Sound* mBGM;
    std::map<std::string, FMOD::Sound*> mSoundMap;
    std::vector<FMOD::Channel*> mSEChannels; // SE용 동적 채널 관리
};