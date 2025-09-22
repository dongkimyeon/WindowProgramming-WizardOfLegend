#include "GameClearScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "SoundManager.h"
#include "Time.h"
#include "Input.h"
#include <fstream>
#include <cmath> // sin �Լ� ����� ���� �߰�

extern const UINT width;
extern const UINT height;

// ������
GameClearScene::GameClearScene()
	: mAlpha(255.0f)
	, mAlphaDir(-1)
	, mSineTime(0.0f) // ������ �ð� �ʱ�ȭ
{
}

// �Ҹ���
GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{
	mBackGroundImage.Load(L"resources/GameClearImage.png");
}

void GameClearScene::Update()
{
	if (Input::GetKeyDown(eKeyCode::SPACE))
	{
		// UserID and TimeRecord save
		FILE* fp = nullptr;
		_wfopen_s(&fp, L"resources/TimeRecords/TimeRecords.txt", L"w");
		if (fp == nullptr)
		{
			wprintf(L"File open error\n");
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				fwprintf(fp, L"%s %.2f\n", userID[i].c_str(), timeRecord[i]);
			}
		}


		SceneManager::LoadScene(L"TitleScene");
		SoundManager::GetInstance()->mPlaySound("TitleScreen", true);
		SceneManager::GetSharedPlayer()->SetHp(300);
		SceneManager::GetSharedPlayer()->Setrevive();
		SceneManager::ResetPlayTime();
		SceneManager::SetmIsGameStart(false);
	}


	mAlpha += mAlphaDir;
	if (mAlpha >= 255 || mAlpha <= 50)
		mAlphaDir *= -1;


	mSineTime += Time::DeltaTime();
}

void GameClearScene::LateUpdate()
{
	Scene::LateUpdate();
}

void GameClearScene::Render(HDC hdc)
{
	if (!mBackGroundImage.IsNull())
	{
		mBackGroundImage.Draw(hdc, 0, 0, width, height);
	}

	HFONT devhFont = CreateFont(35, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
	HFONT devhOldFont = (HFONT)SelectObject(hdc, devhFont);

	const wchar_t* devText1 = L"DEVELOPER";
	const wchar_t* devText2 = L"DONGYEON";
	const wchar_t* devText3 = L"HYEONGMIN";
	int margin = 10;
	int lineHeight = 50;

	SIZE devSize1, devSize2, devSize3;
	GetTextExtentPoint32(hdc, devText1, wcslen(devText1), &devSize1);
	GetTextExtentPoint32(hdc, devText2, wcslen(devText2), &devSize2);
	GetTextExtentPoint32(hdc, devText3, wcslen(devText3), &devSize3);
	SetTextColor(hdc, RGB(231, 231, 231));
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, width - devSize1.cx - margin, height - lineHeight * 3 - margin, devText1, wcslen(devText1));
	TextOut(hdc, width - devSize2.cx - margin, height - lineHeight * 2 - margin, devText2, wcslen(devText2));
	TextOut(hdc, width - devSize3.cx - margin, height - lineHeight - margin, devText3, wcslen(devText3));

	SelectObject(hdc, devhOldFont);
	DeleteObject(devhFont);

	// "GameClear" �ؽ�Ʈ�� �߾� ��ܿ� �׸���
	HFONT hTitleFont = CreateFont(80, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
	HFONT hOldTitleFont = (HFONT)SelectObject(hdc, hTitleFont);
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);

	wchar_t gameClearText[] = L"GameClear";
	SIZE titleTextSize;
	GetTextExtentPoint32(hdc, gameClearText, wcslen(gameClearText), &titleTextSize);
	int xCenter = (width - titleTextSize.cx) / 2;
	int yTop = 50; // ��ܿ��� 50px �Ʒ�
	TextOut(hdc, xCenter, yTop, gameClearText, wcslen(gameClearText));

	SelectObject(hdc, hOldTitleFont);
	DeleteObject(hTitleFont);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	HFONT hFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

	// �÷���Ÿ�� ��������
	float playTime = SceneManager::GetPlayTime();
	int minutes = static_cast<int>(playTime) / 60;
	int seconds = static_cast<int>(playTime) % 60;
	wchar_t playTimeText[32];
	swprintf_s(playTimeText, L"Play Time: %02d:%02d", minutes, seconds);






	std::wstring deadStageName = SceneManager::GetSharedPlayer()->GetDeadStage();
	int killcount = SceneManager::GetSharedPlayer()->GetKillCount();

	wchar_t killCountText[32];
	swprintf_s(killCountText, L"Enemies Killed: %d", killcount);

	wchar_t stageText[64];
	swprintf_s(stageText, L"Stage Reached: %s", deadStageName.c_str());

	// ������ ������ ���� �ؽ�Ʈ ũ�� ���
	int rightMargin = width - 50; // ������ ���� 50px
	int y = 250;
	SIZE textSize;

	// Play Time �ؽ�Ʈ
	GetTextExtentPoint32(hdc, playTimeText, wcslen(playTimeText), &textSize);
	int xPlayTime = rightMargin - textSize.cx;
	TextOut(hdc, xPlayTime, y, playTimeText, wcslen(playTimeText));

	// Kill Count �ؽ�Ʈ
	GetTextExtentPoint32(hdc, killCountText, wcslen(killCountText), &textSize);
	int xKillCount = rightMargin - textSize.cx;
	TextOut(hdc, xKillCount, y + 70, killCountText, wcslen(killCountText));

	// GDI+�� "Press Space to Title"�� ������ ó��
	Gdiplus::Graphics graphics(hdc);
	graphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);
	Gdiplus::FontFamily fontFamily(L"EXO 2");
	Gdiplus::Font font(&fontFamily, 50, Gdiplus::FontStyleBold, Gdiplus::UnitPixel);
	float sineAlpha = 50.0f + (205.0f * (sin(mSineTime * 2.0f) + 1.0f) / 2.0f);
	Gdiplus::SolidBrush alphaBrush(Gdiplus::Color(static_cast<BYTE>(sineAlpha), 255, 255, 255));
	wchar_t pressSpaceText[] = L"Press Space to Title";
	graphics.DrawString(pressSpaceText, -1, &font, Gdiplus::PointF(50, height - 100), &alphaBrush);

	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);
}

void GameClearScene::LoadUserRecord()
{
	std::wstring CurrentUserID = SceneManager::GetUserID();
	float playTime = SceneManager::GetPlayTime();

	// load UserID and TimeRecord
	FILE* fp = nullptr;
	_wfopen_s(&fp, L"resources/TimeRecords/TimeRecords.txt", L"r+");

	if (fp == nullptr)
	{
		wprintf(L"File open error\n");
	}
	else
	{
		// ���Ͽ��� ���� ��� �б�

		for (int i = 0; i < 10; i++)
		{
			wchar_t idBuffer[50];
			float timeBuffer;
			if (fwscanf_s(fp, L"%49s %f", idBuffer, &timeBuffer) == 2)
			{
				userID[i] = idBuffer;
				timeRecord[i] = timeBuffer;

			}
			else
			{
				// �б� ���� �� �⺻�� ����
				userID[i] = L"dummy";
				timeRecord[i] = 9999.0f; // �ſ� ū ������ �ʱ�ȭ
			}
		}

		// ����

		for (int i = 0; i < 10; i++)
		{
			for (int j = i + 1; j < 10; j++)
			{
				if (timeRecord[i] > timeRecord[j])
				{
					std::swap(timeRecord[i], timeRecord[j]);
					std::swap(userID[i], userID[j]);
				}
			}
		}


		// �� ��� �߰� �Ǵ� ���� ��� ������Ʈ


		for (int i = 0; i < 10; i++)
		{

			if (playTime < timeRecord[i])
			{
				timeRecord[i] = playTime; // �� ���� ������� ������Ʈ
				userID[i] = CurrentUserID; // ���� ����� ID�� ������Ʈ
			}

		}


	}
	fclose(fp);
}