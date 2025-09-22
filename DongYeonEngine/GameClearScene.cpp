#include "GameClearScene.h"
#include "SceneManager.h"
#include "MapManager.h"
#include "SoundManager.h"
#include "Time.h"
#include "Input.h"
#include <fstream>
#include <cmath> // sin 함수 사용을 위해 추가


extern const UINT width;
extern const UINT height;

// 생성자
GameClearScene::GameClearScene()
    : mAlpha(255.0f)
    , mAlphaDir(-1)
    , mSineTime(0.0f) // 사인파 시간 초기화
{
    // TimeRecord 배열 초기화
    for (int i = 0; i < 10; i++)
    {
        userID[i] = L"";
        timeRecord[i] = 9999.0f;
    }
}

// 소멸자
GameClearScene::~GameClearScene()
{
}

void GameClearScene::Initialize()
{
    mBackGroundImage.Load(L"resources/GameClearImage.png");

    // 초기화 시 기존 기록 로드
    LoadUserRecord();
}

void GameClearScene::Update()
{
    if (Input::GetKeyDown(eKeyCode::SPACE))
    {
        // 현재 플레이어 기록을 랭킹에 반영
        SaveCurrentRecord();

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

    // "GameClear" 텍스트를 중앙 상단에 그리기
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
    int yTop = 50; // 상단에서 50px 아래
    TextOut(hdc, xCenter, yTop, gameClearText, wcslen(gameClearText));

    SelectObject(hdc, hOldTitleFont);
    DeleteObject(hTitleFont);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));

    HFONT hFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    // 플레이타임 가져오기
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

    // 오른쪽 정렬을 위한 텍스트 크기 계산
    int rightMargin = width - 50; // 오른쪽 여백 50px
    int y = 250;
    SIZE textSize;

    // Play Time 텍스트
    GetTextExtentPoint32(hdc, playTimeText, wcslen(playTimeText), &textSize);
    int xPlayTime = rightMargin - textSize.cx;
    TextOut(hdc, xPlayTime, y, playTimeText, wcslen(playTimeText));

    // Kill Count 텍스트
    GetTextExtentPoint32(hdc, killCountText, wcslen(killCountText), &textSize);
    int xKillCount = rightMargin - textSize.cx;
    TextOut(hdc, xKillCount, y + 70, killCountText, wcslen(killCountText));

    // 랭킹 표시 (상위 5개)
    HFONT hRankFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
    HFONT hOldRankFont = (HFONT)SelectObject(hdc, hRankFont);

    SetTextColor(hdc, RGB(255, 255, 100)); // 금색으로 랭킹 강조
    wchar_t rankText[64];
    int rankY = 400;

    for (int i = 0; i < 5 && timeRecord[i] < 9999.0f; i++)
    {
        int rankMinutes = static_cast<int>(timeRecord[i]) / 60;
        int rankSeconds = static_cast<int>(timeRecord[i]) % 60;
        swprintf_s(rankText, L"%d. %s: %02d:%02d", i + 1, userID[i].c_str(), rankMinutes, rankSeconds);

        GetTextExtentPoint32(hdc, rankText, wcslen(rankText), &textSize);
        int xRank = (width - textSize.cx) / 2; // 중앙 정렬
        TextOut(hdc, xRank, rankY + (i * 40), rankText, wcslen(rankText));
    }

    SelectObject(hdc, hOldRankFont);
    DeleteObject(hRankFont);

    // GDI+로 "Press Space to Title"만 반투명 처리
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
    // load UserID and TimeRecord
    FILE* fp = nullptr;
    _wfopen_s(&fp, L"resources/TimeRecords/TimeRecords.txt", L"r");

    if (fp == nullptr)
    {
        // 파일이 없는 경우 초기화
        wprintf(L"File not found, creating new record file\n");
        for (int i = 0; i < 10; i++)
        {
            userID[i] = L"";
            timeRecord[i] = 9999.0f;
        }
        return;
    }

    // 파일에서 기존 기록 읽기
    int recordCount = 0;
    for (int i = 0; i < 10 && recordCount < 10; i++)
    {
        wchar_t idBuffer[50];
        float timeBuffer;

        if (fwscanf_s(fp, L"%49s %f", idBuffer, &timeBuffer) == 2)
        {
            userID[i] = idBuffer;
            timeRecord[i] = timeBuffer;
            recordCount++;
        }
        else
        {
            // 더 이상 읽을 데이터가 없으면 나머지는 기본값
            userID[i] = L"";
            timeRecord[i] = 9999.0f;
        }
    }

    fclose(fp);

    // 읽은 기록이 10개 미만인 경우 나머지 초기화
    for (int i = recordCount; i < 10; i++)
    {
        userID[i] = L"";
        timeRecord[i] = 9999.0f;
    }

    // 기존 기록 정렬 (오름차순 - 가장 빠른 기록이 앞에)
    for (int i = 0; i < 10; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (timeRecord[i] > timeRecord[j] && timeRecord[j] < 9999.0f)
            {
                std::swap(timeRecord[i], timeRecord[j]);
                std::swap(userID[i], userID[j]);
            }
        }
    }

    wprintf(L"Loaded %d records from file\n", recordCount);
}

void GameClearScene::SaveCurrentRecord()
{
    std::wstring CurrentUserID = SceneManager::GetUserID();
    float playTime = SceneManager::GetPlayTime();

    if (CurrentUserID.empty() || playTime >= 9999.0f)
    {
        wprintf(L"No valid user ID or play time\n");
        return;
    }

    wprintf(L"Saving record for user: %s, time: %.2f\n", CurrentUserID.c_str(), playTime);

    // 현재 기록이 이미 랭킹에 있는지 확인
    bool existingRecord = false;
    int existingIndex = -1;
    for (int i = 0; i < 10; i++)
    {
        if (userID[i] == CurrentUserID && timeRecord[i] < 9999.0f)
        {
            existingRecord = true;
            existingIndex = i;
            break;
        }
    }

    if (existingRecord)
    {
        // 기존 기록이 더 빠른 경우 유지, 현재 기록이 더 빠른 경우 업데이트
        if (playTime < timeRecord[existingIndex])
        {
            timeRecord[existingIndex] = playTime;
            wprintf(L"Updated existing record for %s\n", CurrentUserID.c_str());
        }
        else
        {
            wprintf(L"Current time is slower than existing record for %s\n", CurrentUserID.c_str());
        }
    }
    else
    {
        // 새로운 기록 추가
        bool inserted = false;
        for (int i = 0; i < 10; i++)
        {
            if (playTime < timeRecord[i] || timeRecord[i] >= 9999.0f)
            {
                // 현재 기록을 삽입하고 기존 기록들을 뒤로 밀기
                for (int j = 9; j > i; j--)
                {
                    timeRecord[j] = timeRecord[j - 1];
                    userID[j] = userID[j - 1];
                }
                timeRecord[i] = playTime;
                userID[i] = CurrentUserID;
                inserted = true;
                wprintf(L"New record inserted at rank %d for %s\n", i + 1, CurrentUserID.c_str());
                break;
            }
        }

        if (!inserted && playTime < 9999.0f)
        {
            // 10위권에 들어갈 수 있는 경우
            if (timeRecord[9] >= 9999.0f || playTime < timeRecord[9])
            {
                timeRecord[9] = playTime;
                userID[9] = CurrentUserID;
                wprintf(L"New record added at rank 10 for %s\n", CurrentUserID.c_str());
            }
        }
    }

    // 랭킹 재정렬 (안전장치)
    for (int i = 0; i < 10; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (timeRecord[i] > timeRecord[j] && timeRecord[j] < 9999.0f)
            {
                std::swap(timeRecord[i], timeRecord[j]);
                std::swap(userID[i], userID[j]);
            }
        }
    }

    // 파일에 저장
    FILE* fp = nullptr;
    _wfopen_s(&fp, L"resources/TimeRecords/TimeRecords.txt", L"w");
    if (fp == nullptr)
    {
        wprintf(L"Failed to open file for writing\n");
        return;
    }

    int savedCount = 0;
    for (int i = 0; i < 10; i++)
    {
        if (timeRecord[i] < 9999.0f)
        {
            fwprintf(fp, L"%s %.2f\n", userID[i].c_str(), timeRecord[i]);
            savedCount++;
        }
        else
        {
            break;
        }
    }

    fclose(fp);
    wprintf(L"Saved %d records to file successfully\n", savedCount);
}