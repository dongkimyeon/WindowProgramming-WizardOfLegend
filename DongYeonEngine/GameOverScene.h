#pragma once
#include "Scene.h"

class GameOverScene : public Scene
{
public:
    GameOverScene();
    ~GameOverScene();

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

private:

    CImage mBackGroundImage;
    CImage mLogoImage;

};