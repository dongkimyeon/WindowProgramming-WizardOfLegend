#pragma once
#include "Scene.h" 


class RankingScene : public Scene
{
public:
    RankingScene();
    ~RankingScene();

    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;
	void LoadUserRecord();

private:
	CImage goldTrophyImage;
	CImage silverTrophyImage;
	CImage bronzeTrophyImage;
	CImage mBackGroundImage;

	std::wstring userID[10];
	float timeRecord[10];

};

