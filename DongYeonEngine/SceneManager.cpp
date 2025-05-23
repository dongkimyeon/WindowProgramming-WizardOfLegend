#include "SceneManager.h"
#include "Input.h"
#include "Time.h"

std::map<std::wstring, Scene*> SceneManager::mScene = {};
Scene* SceneManager::mActiveScene = nullptr;
CImage SceneManager::mMouseCursorImage;
Player* SceneManager::mSharedPlayer = nullptr; 
float SceneManager::playTime = 0.0f;
bool SceneManager::mIsGameStart = false;

void SceneManager::Initialize()
{
	mMouseCursorImage.Load(L"resources/MouseCursor.png");
	mSharedPlayer = new Player(); // 플레이어 객체 초기화
	playTime = 0.0f; // 플레이 타임 초기화

	MapManager::GetInstance()->Initialize();

}

void SceneManager::Update()
{
	mActiveScene->Update();
	if (Input::GetKeyDown(eKeyCode::ESC))
	{
		PostQuitMessage(0);	
	}
	if (mIsGameStart)
	{
		playTime += Time::DeltaTime();
	}

}

void SceneManager::LateUpdate()
{
	mActiveScene->LateUpdate();
}

void SceneManager::Render(HDC hdc)
{
	mActiveScene->Render(hdc);
	int cursorImageWidth = mMouseCursorImage.GetWidth();
	int cursorImageHeight = mMouseCursorImage.GetHeight();
	mMouseCursorImage.Draw(hdc, Input::GetMousePosition().x - 40, Input::GetMousePosition().y - 40, cursorImageWidth + 50, cursorImageHeight + 50);
	
	Time::Render(hdc);
	// 플레이타임 분:초 형식으로 렌더링
	if (mIsGameStart)
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		HFONT hFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
		HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

		int minutes = static_cast<int>(playTime) / 60;
		int seconds = static_cast<int>(playTime) % 60;
		wchar_t playTimeText[32];
		swprintf_s(playTimeText, L"Play Time: %02d:%02d", minutes, seconds);
		TextOut(hdc, 1000, 0, playTimeText, wcslen(playTimeText));

		SelectObject(hdc, hOldFont);
		DeleteObject(hFont);
	}


}