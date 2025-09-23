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

    // ����� ��Ʈ ���� (EXO 2, ũ�� 50, ����ü)
    HFONT hTitleFont = CreateFont(50, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldTitleFont = (HFONT)SelectObject(hdc, hTitleFont);

    // �ؽ�Ʈ ���� �� ��� ����
    SetTextColor(hdc, RGB(255, 255, 255)); // �ݻ� �ؽ�Ʈ
    SetBkMode(hdc, TRANSPARENT);

    // ���� ǥ��
    wchar_t titleText[] = L"Ranking Board";
    SIZE titleSize;
    GetTextExtentPoint32(hdc, titleText, wcslen(titleText), &titleSize);
    int xTitle = (width - titleSize.cx) / 2; // �߾� ����
    TextOut(hdc, xTitle, 75, titleText, wcslen(titleText));

    // ���� ��Ʈ ����
    SelectObject(hdc, hOldTitleFont);
    DeleteObject(hTitleFont);

    // ��ŷ�� ��Ʈ ���� (EXO 2, ũ�� 30, ����ü)
    HFONT hRankFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    HFONT hOldRankFont = (HFONT)SelectObject(hdc, hRankFont);

    // ��ŷ ǥ�� (���� 10��)
    int startY = 200; // ���� Y ��ġ
    SIZE textSize;

    for (int i = 0; i < 10; i++)
    {
        int yPos = startY + (i * 50); // �� �׸� ���� ���� 50px

        // Ʈ���� �̹��� ǥ�� (1~3����)
        if (i == 0 && timeRecord[i] < 9999.0f) // 1��
            goldTrophyImage.Draw(hdc, 435, yPos, 40, 40);
        else if (i == 1 && timeRecord[i] < 9999.0f) // 2��
            silverTrophyImage.Draw(hdc, 435, yPos, 40, 40);
        else if (i == 2 && timeRecord[i] < 9999.0f) // 3��
            bronzeTrophyImage.Draw(hdc, 435, yPos, 40, 40);

        // ��ŷ �ؽ�Ʈ ����
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

        // �ؽ�Ʈ �߾� ����
        GetTextExtentPoint32(hdc, rankText, wcslen(rankText), &textSize);
        int xRank = (width - textSize.cx) / 2; // �߾� ����
        TextOut(hdc, xRank, yPos, rankText, wcslen(rankText));
    }

    // ��ŷ ��Ʈ ����
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
        // ������ ���� ��� �ʱ�ȭ
        wprintf(L"File not found or error opening, creating new record file (error: %d)\n", err);
        for (int i = 0; i < 10; i++)
        {
            userID[i] = L"";
            timeRecord[i] = 9999.0f;
        }
        return;
    }

    // ���Ͽ��� ���� ��� �б�
    int recordCount = 0;
    wchar_t lineBuffer[256];

    // ���� ������ �б�
    while (recordCount < 10 && fgetws(lineBuffer, sizeof(lineBuffer) / sizeof(wchar_t), fp))
    {
        // �� ���� ���๮�� ����
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

        // ���ڿ� �Ľ� (�����̽��� ������ ����)
        wchar_t* idStart = lineBuffer;
        wchar_t* timeStart = wcschr(lineBuffer, L' ');
        if (timeStart == nullptr)
            timeStart = wcschr(lineBuffer, L'\t'); // �ǵ� Ȯ��

        if (timeStart != nullptr)
        {
            *timeStart = L'\0'; // ID�� �ð� �и�
            timeStart++;

            // ID ���� ���� ����
            wchar_t* idEnd = wcsrchr(idStart, L' ');
            if (idEnd != nullptr && idEnd < timeStart - 1)
            {
                *idEnd = L'\0';
                idStart = idEnd + 1;
            }

            // �ð� ���ڿ��� float���� ��ȯ
            wchar_t* endPtr;
            float timeValue = wcstod(timeStart, &endPtr);

            if (endPtr != timeStart && *endPtr == L'\0') // ���������� ��ȯ�� ���
            {
                // ID ���� ����
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

    // ���� ����� 10�� �̸��� ��� ������ �ʱ�ȭ
    for (int i = recordCount; i < 10; i++)
    {
        userID[i] = L"";
        timeRecord[i] = 9999.0f;
    }

    // ���� ��� ���� (�������� - ���� ���� ����� �տ�)
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