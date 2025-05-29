#pragma once
#include "Scene.h"

class GameClearScene : public Scene
{
public:
    GameClearScene();
    ~GameClearScene();

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

private:

    CImage mBackGroundImage;


    float mAlpha;         // 기존 알파값
    int mAlphaDir;        // 기존 알파 방향
    float mSineTime;      // 사인파용 시간 누적 변수
};