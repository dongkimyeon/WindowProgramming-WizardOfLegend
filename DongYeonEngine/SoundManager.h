#pragma once
#include "CommonInclude.h"
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib, "winmm.lib")

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    static SoundManager* GetInstance()
    {
        static SoundManager instance;
        return &instance;
    }
    
    


private:
    LPDIRECTSOUNDBUFFER _soundBuffer;
    DSBUFFERDESC _bufferInfo;
	int volume; // º¼·ý °ª (0~0xFFFF)
};