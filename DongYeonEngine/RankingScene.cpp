#include "RankingScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include <fstream>
#include <cmath>

extern const UINT width;
extern const UINT height;

RankingScene::RankingScene()
{
}

RankingScene::~RankingScene()
{
}

void RankingScene::Initialize()
{
    goldTrophyImage.Load(L"resources/Trophy/gold.png");
    silverTrophyImage.Load(L"resources/Trophy/silver.png");
    bronzeTrophyImage.Load(L"resources/Trophy/bronze.png");
    mBackGroundImage.Load(L"resources/RankingBackground.png");
}

void RankingScene::Update()
{
    LoadUserRecord();

    if (Input::GetKeyDown(eKeyCode::SPACE))
    {
        SceneManager::LoadScene(L"TitleScene");
        SoundManager::GetInstance()->mPlaySound("TitleScreen", true);
        SceneManager::ResetUserID();
    }
}

void RankingScene::LateUpdate()
{
}

void RankingScene::Render(HDC hdc)
{
    mBackGroundImage.Draw(hdc, 0, 0, width, height);

    // 제목용 폰트 설정 (8BIT WONDER, 크기 50, 굵은체)
    HFONT hTitleFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldTitleFont = (HFONT)SelectObject(hdc, hTitleFont);

    // 텍스트 색상 및 배경 설정
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    // 제목 표시
    wchar_t titleText[] = L"Ranking Board";
    SIZE titleSize;
    GetTextExtentPoint32(hdc, titleText, wcslen(titleText), &titleSize);
    int xTitle = (width - titleSize.cx) / 2;
    TextOut(hdc, xTitle, 75, titleText, wcslen(titleText));

    // 제목 폰트 해제
    SelectObject(hdc, hOldTitleFont);
    DeleteObject(hTitleFont);

    // 랭킹용 폰트 설정 (8BIT WONDER, 크기 30, 굵은체)
    HFONT hRankFont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldRankFont = (HFONT)SelectObject(hdc, hRankFont);

    // 랭킹 표시 (상위 10개)
    int startY = 200;
    SIZE textSize;

    for (int i = 0; i < 10; i++)
    {
        int yPos = startY + (i * 50);

      
        // 랭킹 텍스트 생성
        wchar_t rankText[64];
        if (timeRecord[i] < 9999.0f)
        {
            int minutes = static_cast<int>(timeRecord[i]) / 60;
            float seconds = timeRecord[i] - (minutes * 60);
            swprintf_s(rankText, L"%d. %s: %02d:%05.2f", i + 1, userID[i].c_str(), minutes, seconds);
        }
        else
        {
            swprintf_s(rankText, L"%d. ---: --:--.--", i + 1);
        }

        // 텍스트 중앙 정렬
        GetTextExtentPoint32(hdc, rankText, wcslen(rankText), &textSize);
        int xRank = (width - textSize.cx) / 2;
        TextOut(hdc, xRank, yPos, rankText, wcslen(rankText));

        // 트로피 이미지 표시 (1~3위만)
        if (i == 0 && timeRecord[i] < 9999.0f)
            goldTrophyImage.Draw(hdc, xRank - 50, yPos, 40, 40);
        else if (i == 1 && timeRecord[i] < 9999.0f)
            silverTrophyImage.Draw(hdc, xRank - 50, yPos, 40, 40);
        else if (i == 2 && timeRecord[i] < 9999.0f)
            bronzeTrophyImage.Draw(hdc, xRank - 50, yPos, 40, 40);

    }

    // 랭킹 폰트 해제
    SelectObject(hdc, hOldRankFont);
    DeleteObject(hRankFont);
}

void RankingScene::LoadUserRecord()
{
    FILE* fp = nullptr;
    errno_t err = _wfopen_s(&fp, L"resources/TimeRecords/TimeRecords.txt", L"r");

    if (err != 0 || fp == nullptr)
    {
        wprintf(L"File not found or error opening, creating new record file (error: %d)\n", err);
        for (int i = 0; i < 10; i++)
        {
            userID[i] = L"";
            timeRecord[i] = 9999.0f;
        }
        return;
    }

    int recordCount = 0;
    wchar_t lineBuffer[256];

    while (recordCount < 10 && fgetws(lineBuffer, sizeof(lineBuffer) / sizeof(wchar_t), fp))
    {
        size_t len = wcslen(lineBuffer);
        if (len > 0 && (lineBuffer[len - 1] == L'\n' || lineBuffer[len - 1] == L'\r'))
        {
            lineBuffer[len - 1] = L'\0';
            len--;
        }
        if (len > 0 && lineBuffer[len - 1] == L'\r')
        {
            lineBuffer[len - 1] = L'\0';
        }

        wchar_t* idStart = lineBuffer;
        wchar_t* timeStart = wcschr(lineBuffer, L' ');
        if (timeStart == nullptr)
            timeStart = wcschr(lineBuffer, L'\t');

        if (timeStart != nullptr)
        {
            *timeStart = L'\0';
            timeStart++;

            wchar_t* idEnd = wcsrchr(idStart, L' ');
            if (idEnd != nullptr && idEnd < timeStart - 1)
            {
                *idEnd = L'\0';
                idStart = idEnd + 1;
            }

            wchar_t* endPtr;
            float timeValue = wcstod(timeStart, &endPtr);

            if (endPtr != timeStart && *endPtr == L'\0')
            {
                timeValue = roundf(timeValue * 100.0f) / 100.0f;
                size_t idLen = wcslen(idStart);
                if (idLen > 0 && idLen < 50)
                {
                    userID[recordCount] = idStart;
                    timeRecord[recordCount] = timeValue;
                    recordCount++;
                    wprintf(L"Loaded record: %s %.2f\n", idStart, timeValue);
                }
            }
        }
    }

    fclose(fp);

    for (int i = recordCount; i < 10; i++)
    {
        userID[i] = L"";
        timeRecord[i] = 9999.0f;
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = i + 1; j < 10; j++)
        {
            if (timeRecord[i] > timeRecord[j] && timeRecord[j] < 9999.0f && !userID[j].empty())
            {
                std::swap(timeRecord[i], timeRecord[j]);
                std::swap(userID[i], userID[j]);
            }
        }
    }

    wprintf(L"Successfully loaded %d records from file\n", recordCount);
}