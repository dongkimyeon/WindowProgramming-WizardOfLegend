#pragma once
#include "Sound.h"


void Sound::Initialize() {
	System_Create(&mSystem);
	mSystem->init(32, FMOD_INIT_NORMAL, 0);
}

void Sound::LoadSound(const std::wstring* filePath) {
	
}