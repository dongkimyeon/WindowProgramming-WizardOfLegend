#pragma once
#include "SoundManager.h"

#define MAX_SE_SOUND 15
#define MAX_BGM_SOUND 5

void SoundManager::Initialize()	// 초기화
{
	FMOD::System_Create(&mSystem);
	mSystem->init(20, FMOD_INIT_NORMAL, 0);
	mSEChannel->setVolume(0.5f); // SE 볼륨 설정
	mBGMChannel->setVolume(0.5f); // BGM 볼륨 설정

	mSystem->createSound("resources/Audio/SE/ArcherRelease.wav", FMOD_DEFAULT, 0, &mSE[0]);	//archer
	mSystem->createSound("resources/Audio/SE/ArcherReload.wav", FMOD_DEFAULT, 0, &mSE[1]);
	mSystem->createSound("resources/Audio/SE/ArcherShoot.wav", FMOD_DEFAULT, 0, &mSE[2]);

	mSystem->createSound("resources/Audio/SE/SwordManAttack.wav", FMOD_DEFAULT, 0, &mSE[9]); //swordman
	
	mSystem->createSound("resources/Audio/SE/PlayerFootstep.wav", FMOD_DEFAULT, 0, &mSE[8]); //player
	mSystem->createSound("resources/Audio/SE/Dash.wav", FMOD_DEFAULT, 0, &mSE[3]);
	mSystem->createSound("resources/Audio/SE/Fireball.wav", FMOD_DEFAULT, 0, &mSE[4]);
	mSystem->createSound("resources/Audio/SE/FireBlastEnd.wav", FMOD_DEFAULT, 0, &mSE[5]);
	mSystem->createSound("resources/Audio/SE/FireBlastLOOP.wav", FMOD_DEFAULT, 0, &mSE[6]);
	mSystem->createSound("resources/Audio/SE/FireBlastStart.wav", FMOD_DEFAULT, 0, &mSE[7]);






	mSystem->createSound("resources/Audio/BGM/Tutorial_Jazz.wav", FMOD_LOOP_NORMAL, 0, &mBGM); //BGM
}

void SoundManager::play(const std::wstring& fileName, const std::wstring& TypeOfSound) {

}