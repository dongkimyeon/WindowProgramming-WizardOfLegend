#pragma once
#include "fmod.hpp"
#include "CommonInclude.h"

class Sound
{
public:
	void Initialize();

	void LoadSound(const std::wstring* filePath);
	void mPlaySound();
	void mStopSound();
	void mPauseSound();
	void mResumeSound();

	void mSetVolume(float volume);
	float mGetVolume() const;
	void mSetPitch(float pitch);
	float mGetPitch() const;
	void mSetLoop(bool loop);
	bool mIsLooping() const;

private:
	FMOD::System* mSystem;
	FMOD::Channel* mChannel;
	FMOD::Sound mSound;
	float mVolume;
	float mPitch;
	bool mLooping;
	bool mIsPlaying;
	bool mIsPaused;
};