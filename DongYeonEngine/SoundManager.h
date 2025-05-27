#pragma once
#include "CommonInclude.h"
#include "FMOD.hpp"

class SoundManager {
public:
	void Initialize();

	static SoundManager* GetInstance() {
		static SoundManager instance;
		return &instance;
	}

	void play(const std::wstring& fileName, const std::wstring& TypeOfSound);


	void test() { mSystem->playSound(mBGM, 0, false, &mBGMChannel); }

private:
	FMOD::System* mSystem;
	FMOD::Channel* mSEChannel;
	FMOD::Channel* mBGMChannel;
	FMOD::Sound* mSE[15];
	FMOD::Sound* mBGM;
};