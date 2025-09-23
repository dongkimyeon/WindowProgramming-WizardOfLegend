#include "RankingScene.h"
#include "Input.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include <fstream>

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
    LoadUserRecord();
}

void RankingScene::Update()
{
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

    // 제목용 폰트 설정 (EXO 2, 크기 50, 굵은체)
    HFONT hTitleFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldTitleFont = (HFONT)SelectObject(hdc, hTitleFont);

    // 텍스트 색상 및 배경 설정
    SetTextColor(hdc, RGB(255, 255, 255)); // 금색 텍스트
    SetBkMode(hdc, TRANSPARENT);

    // 제목 표시
    wchar_t titleText[] = L"Ranking Board";
    SIZE titleSize;
    GetTextExtentPoint32(hdc, titleText, wcslen(titleText), &titleSize);
    int xTitle = (width - titleSize.cx) / 2; // 중앙 정렬
    TextOut(hdc, xTitle, 75, titleText, wcslen(titleText));

    // 제목 폰트 해제
    SelectObject(hdc, hOldTitleFont);
    DeleteObject(hTitleFont);

    // 랭킹용 폰트 설정 (EXO 2, 크기 30, 굵은체)
    HFONT hRankFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldRankFont = (HFONT)SelectObject(hdc, hRankFont);

    // 랭킹 표시 (상위 10개)
    int startY = 200; // 시작 Y 위치
    SIZE textSize;

    for (int i = 0; i < 10; i++)
    {
        int yPos = startY + (i * 50); // 각 항목 세로 간격 50px

        // 트로피 이미지 표시 (1~3위만)
        if (i == 0 && timeRecord[i] < 9999.0f) // 1위
            goldTrophyImage.Draw(hdc, 435, yPos, 40, 40);
        else if (i == 1 && timeRecord[i] < 9999.0f) // 2위
            silverTrophyImage.Draw(hdc, 435, yPos, 40, 40);
        else if (i == 2 && timeRecord[i] < 9999.0f) // 3위
            bronzeTrophyImage.Draw(hdc, 435, yPos, 40, 40);

        // 랭킹 텍스트 생성
        wchar_t rankText[64];
        if (timeRecord[i] < 9999.0f)
        {
            int rankMinutes = static_cast<int>(timeRecord[i]) / 60;
            int rankSeconds = static_cast<int>(timeRecord[i]) % 60;
            swprintf_s(rankText, L"%d. %s: %02d:%02d", i + 1, userID[i].c_str(), rankMinutes, rankSeconds);
        }
        else
        {
            swprintf_s(rankText, L"%d. ---: --:--", i + 1);
        }

        // 텍스트 중앙 정렬
        GetTextExtentPoint32(hdc, rankText, wcslen(rankText), &textSize);
        int xRank = (width - textSize.cx) / 2; // 중앙 정렬
        TextOut(hdc, xRank, yPos, rankText, wcslen(rankText));
    }

    // 랭킹 폰트 해제
    SelectObject(hdc, hOldRankFont);
    DeleteObject(hRankFont);
}
void RankingScene::LoadUserRecord()
{
    // load UserID and TimeRecord
    FILE* fp = nullptr;
    errno_t err = _wfopen_s(&fp, L"resources/TimeRecords/TimeRecords.txt", L"r");

    if (err != 0 || fp == nullptr)
    {
        // 파일이 없는 경우 초기화
        wprintf(L"File not found or error opening, creating new record file (error: %d)\n", err);
        for (int i = 0; i < 10; i++)
        {
            userID[i] = L"";
            timeRecord[i] = 9999.0f;
        }
        return;
    }

    // 파일에서 기존 기록 읽기
    int recordCount = 0;
    wchar_t lineBuffer[256];

    // 라인 단위로 읽기
    while (recordCount < 10 && fgetws(lineBuffer, sizeof(lineBuffer) / sizeof(wchar_t), fp))
    {
        // 줄 끝의 개행문자 제거
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

        // 문자열 파싱 (스페이스나 탭으로 구분)
        wchar_t* idStart = lineBuffer;
        wchar_t* timeStart = wcschr(lineBuffer, L' ');
        if (timeStart == nullptr)
            timeStart = wcschr(lineBuffer, L'\t'); // 탭도 확인

        if (timeStart != nullptr)
        {
            *timeStart = L'\0'; // ID와 시간 분리
            timeStart++;

            // ID 끝의 공백 제거
            wchar_t* idEnd = wcsrchr(idStart, L' ');
            if (idEnd != nullptr && idEnd < timeStart - 1)
            {
                *idEnd = L'\0';
                idStart = idEnd + 1;
            }

            // 시간 문자열을 float으로 변환
            wchar_t* endPtr;
            float timeValue = wcstod(timeStart, &endPtr);

            if (endPtr != timeStart && *endPtr == L'\0') // 성공적으로 변환된 경우
            {
                // ID 길이 제한
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
            if (timeRecord[i] > timeRecord[j] && timeRecord[j] < 9999.0f && !userID[j].empty())
            {
                std::swap(timeRecord[i], timeRecord[j]);
                std::swap(userID[i], userID[j]);
            }
        }
    }

    wprintf(L"Successfully loaded %d records from file\n", recordCount);
}