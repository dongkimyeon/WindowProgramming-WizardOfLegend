#pragma once
#include "Scene.h"
#include "Player.h"
#include "MapManager.h"
#include "Snow.h"

class SceneManager
{
public:
    enum class eFadeState
    {
        None,      // 페이드 없음
        FadeOut,   // 페이드아웃 중
        FadeIn,    // 페이드인 중
    };

    template <typename T>
    static Scene* CreateScene(const std::wstring& name)
    {
        T* scene = new T();
        scene->SetName(name);
        scene->Initialize();
        mScene.insert(std::make_pair(name, scene));
        return scene;   
    }

    static Scene* LoadScene(const std::wstring& name);
    static void StartFadeOut(const std::wstring& name); // 페이드아웃 시작
    static void StartFadeIn();                         // 페이드인 시작
    static void Initialize();
    static void Update();
    static void LateUpdate();
    static void Render(HDC hdc);
    static Player* GetSharedPlayer() { return mSharedPlayer; }
    static Scene* GetActiveScene() { return mActiveScene; }
    static void SetmIsGameStart(bool isGameStart) { mIsGameStart = isGameStart; }
    static float GetPlayTime() { return playTime; }
    static void ResetPlayTime() { playTime = 0; }

private:
    static Player* mSharedPlayer;
    static std::map<std::wstring, Scene*> mScene;
    static Scene* mActiveScene;
    static CImage mMouseCursorImage;
    static float playTime;
    static bool mIsGameStart;
    static bool mESCstate;
    static Snow mSnows[30];
    // 페이드 관련 변수
    static eFadeState mFadeState;
    static float mFadeAlpha;         // 알파값 (0.0f ~ 1.0f)
    static float mFadeTimer;         // 페이드 진행 시간
    static const float mFadeDuration; // 페이드 지속 시간 (초)
    static std::wstring mTargetScene; // 전환할 대상 씬 이름


};