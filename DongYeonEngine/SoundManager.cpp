#include "SoundManager.h"
#include <filesystem>
#include <iostream>

SoundManager::SoundManager() {
}

SoundManager::~SoundManager() {
    PlaySound(NULL, NULL, 0);
}

void SoundManager::Initialize() {
    SetVolume(0xFFFF);
    waveOutSetVolume(NULL, MAKELONG(0xFFFF, 0xFFFF));
}

void SoundManager::Update() {
}

void SoundManager::mPlaySound(const std::wstring& soundFile) {
    

   PlaySound(soundFile.c_str(), NULL, SND_ASYNC | SND_FILENAME);
   
}

void SoundManager::mStopSound(const std::wstring& soundFile) {
    PlaySound(NULL, NULL, 0);
}

void SoundManager::SetVolume(DWORD volume) {
    // 볼륨 범위: 0 (음소거) ~ 0xFFFF (최대)
    waveOutSetVolume(NULL, MAKELONG(volume, volume));
}

DWORD SoundManager::GetVolume() {
    DWORD volume;
    waveOutGetVolume(NULL, &volume);
    return LOWORD(volume); // 좌측 채널 볼륨 반환
}