#pragma once
#include "CommonInclude.h"
#include "Sound.h"

class SoundManager {
public:
	void Initialize();

	static SoundManager& GetInstance() {
		static SoundManager instance;
		return instance;
	}

	void play(const std::wstring fileName);


};