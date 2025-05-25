#pragma once
#include "CommonInclude.h"
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib, "winmm.lib")

class Sound
{
public:
	Sound();
	~Sound();

	int Load(const std::wstring soundName);
	void Play(bool _bLoop = false);
	void PlayToBGM(bool _bLoop = false);
	void Stop(bool _bReset = false);
	void SetVolume(float _fVolume);
	void SetPosition(float _fPosition);
	int getVolume() { return volume; }

private:
	LPDIRECTSOUNDBUFFER _soundBuffer;
	DSBUFFERDESC _bufferInfo;
	int volume;

	bool LoadWaveSound(const std::string _strPath);
	int GetDecibel(float _fVolume);
};
