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
        None,      // ���̵� ����
        FadeOut,   // ���̵�ƿ� ��
        FadeIn,    // ���̵��� ��
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
    static void StartFadeOut(const std::wstring& name); // ���̵�ƿ� ����
    static void StartFadeIn();                         // ���̵��� ����
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
    // ���̵� ���� ����
    static eFadeState mFadeState;
    static float mFadeAlpha;         // ���İ� (0.0f ~ 1.0f)
    static float mFadeTimer;         // ���̵� ���� �ð�
    static const float mFadeDuration; // ���̵� ���� �ð� (��)
    static std::wstring mTargetScene; // ��ȯ�� ��� �� �̸�


};