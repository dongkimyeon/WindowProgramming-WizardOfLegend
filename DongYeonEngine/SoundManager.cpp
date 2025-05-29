#include "SoundManager.h"

#define MAX_SE_SOUND 15
#define MAX_BGM_SOUND 5

void SoundManager::Initialize() {
    FMOD::System_Create(&mSystem);
    mSystem->init(32, FMOD_INIT_NORMAL, 0);

    mBGMChannel = nullptr;

    mSystem->createChannelGroup("SEGroup", &mSEGroup);
    mSystem->createChannelGroup("BGMGroup", &mBGMGroup);

    mSystem->createSound("resources/Audio/SE/ArcherRelease.wav", FMOD_DEFAULT, 0, &mSE[0]);
    mSoundMap["ArcherRelease"] = mSE[0];
    mSystem->createSound("resources/Audio/SE/ArcherReload.wav", FMOD_DEFAULT, 0, &mSE[1]);
    mSoundMap["ArcherReload"] = mSE[1];
    mSystem->createSound("resources/Audio/SE/ArcherShoot.wav", FMOD_DEFAULT, 0, &mSE[2]);
    mSoundMap["ArcherShoot"] = mSE[2];

    mSystem->createSound("resources/Audio/SE/SwordManAttack.wav", FMOD_DEFAULT, 0, &mSE[9]);
    mSoundMap["SwordManAttack"] = mSE[9];

    mSystem->createSound("resources/Audio/SE/PlayerFootstep.wav", FMOD_DEFAULT, 0, &mSE[8]);
    mSoundMap["PlayerFootstep"] = mSE[8];
    mSystem->createSound("resources/Audio/SE/Dash.wav", FMOD_DEFAULT, 0, &mSE[3]);
    mSoundMap["Dash"] = mSE[3];
    mSystem->createSound("resources/Audio/SE/Fireball.wav", FMOD_DEFAULT, 0, &mSE[4]);
    mSoundMap["Fireball"] = mSE[4];
    mSystem->createSound("resources/Audio/SE/FireBlastEnd.wav", FMOD_DEFAULT, 0, &mSE[5]);
    mSoundMap["FireBlastEnd"] = mSE[5];
    mSystem->createSound("resources/Audio/SE/FireBlastLOOP.wav", FMOD_DEFAULT, 0, &mSE[6]);
    mSoundMap["FireBlastLOOP"] = mSE[6];
    mSystem->createSound("resources/Audio/SE/FireBlastStart.wav", FMOD_DEFAULT, 0, &mSE[7]);
    mSoundMap["FireBlastStart"] = mSE[7];
    mSystem->createSound("resources/Audio/SE/StandardPlayerAttack.wav", FMOD_DEFAULT, 0, &mSE[10]);
    mSoundMap["PlayerAttack"] = mSE[10];


    mSystem->createSound("resources/Audio/BGM/Tutorial_Jazz.wav", FMOD_LOOP_NORMAL, 0, &mBGM);
    mSoundMap["Tutorial_Jazz"] = mBGM;

    // Set default volumes for channel groups
    if (mSEGroup) mSEGroup->setVolume(0.1f);
    if (mBGMGroup) mBGMGroup->setVolume(0.2f);
}

void SoundManager::mPlaySound(const std::string& SoundName, bool loop) {
    auto it = mSoundMap.find(SoundName);
    if (it != mSoundMap.end()) {
        FMOD::Sound* sound = it->second;
        FMOD::Channel* channel = nullptr;
        FMOD::ChannelGroup* group = (sound == mBGM) ? mBGMGroup : mSEGroup;

        // BGM 처리
        if (sound == mBGM) {
            mSystem->playSound(sound, mBGMGroup, !loop, &mBGMChannel);
        }
        // SE 처리
        else {
            FMOD_MODE mode = FMOD_DEFAULT;
            if (loop) mode |= FMOD_LOOP_NORMAL;
            sound->setMode(mode);
            mSystem->playSound(sound, mSEGroup, false, &channel); // 항상 새 채널 할당
            if (channel) {
                mSEChannels.push_back(channel);
            }
        }
    }
}

void SoundManager::Release() {
    for (int i = 0; i < MAX_SE_SOUND; i++) {
        if (mSE[i]) mSE[i]->release();
    }
    if (mBGM) mBGM->release();
    if (mSEGroup) mSEGroup->release();
    if (mBGMGroup) mBGMGroup->release();
    if (mSystem) {
        mSystem->release();
    }
    mSEChannels.clear();
}