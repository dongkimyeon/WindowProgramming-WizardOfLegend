#include "UI.h"
#include "SceneManager.h"	
#include "SoundManager.h"
#include "Boss.h"
#include "Time.h"
#include "Input.h"
ATL::CImage UI::UI_HPBAR;
ATL::CImage UI::UI_MPBAR;
ATL::CImage UI::UI_PLAYERBAR;
ATL::CImage UI::UI_SKILLBAR;
ATL::CImage UI::UI_BOSSBAR;
ATL::CImage UI::UI_BOSS_HPBAR;

UI::UI()
{
    UI_HPBAR.Load(L"resources/Ui/UI_HPBAR.png");
    UI_MPBAR.Load(L"resources/Ui/UI_MPBAR.png");
    UI_PLAYERBAR.Load(L"resources/Ui/UI_PLAYERBAR.png");
    UI_SKILLBAR.Load(L"resources/Ui/UI_SKILLBAR.png");
    UI_BOSSBAR.Load(L"resources/Ui/UI_BOSS_BAR.png");
    UI_BOSS_HPBAR.Load(L"resources/Ui/UI_BOSS_HPBAR.png");
}

UI::~UI()
{
    // 자원 해제
    UI_HPBAR.Destroy();
    UI_MPBAR.Destroy();
    UI_PLAYERBAR.Destroy();
    UI_SKILLBAR.Destroy();
    UI_BOSSBAR.Destroy();
    UI_BOSS_HPBAR.Destroy();
}

void UI::Initialize()
{
    UI_HPBAR.Load(L"resources/Ui/UI_HPBAR.png");
    UI_MPBAR.Load(L"resources/Ui/UI_MPBAR.png");
    UI_PLAYERBAR.Load(L"resources/Ui/UI_PLAYERBAR.png");
    UI_SKILLBAR.Load(L"resources/Ui/UI_SKILLBAR.png");
    UI_BOSSBAR.Load(L"resources/Ui/UI_BOSS_BAR.png");
    UI_BOSS_HPBAR.Load(L"resources/Ui/UI_BOSS_HPBAR.png");
}

void UI::Update()
{

}

void UI::Render(HDC hdc)
{
    static float alpha = 0.0f;
    static float alphaTimer = 0.0f;
    const float fadeDuration = 2.5f;

    // 플레이어가 죽었는지 확인
    if (SceneManager::GetSharedPlayer()->GetIsDead())
    {
        // 알파값 증가
        alphaTimer += Time::DeltaTime();
        alpha = min(1.0f, alphaTimer / fadeDuration);

        // 게임 오버 텍스트 렌더링
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(255 * alpha, 0, 0)); // 알파값 적용
        HFONT hFont = CreateFont(120, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        // 화면 중앙에 텍스트 출력
        int screenWidth = 1280;
        int screenHeight = 720;
        const wchar_t* gameOverText = L"GAMEOVER";
        SIZE textSize;
        GetTextExtentPoint32(hdc, gameOverText, wcslen(gameOverText), &textSize);
        TextOut(hdc, (screenWidth - textSize.cx) / 2, (screenHeight - textSize.cy) / 2, gameOverText, wcslen(gameOverText));

        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);

        // 알파값이 1.0에 도달하면 씬 전환
        if (alpha >= 1.0f)
        {
            SceneManager::StartFadeIn();
            SceneManager::LoadScene(L"GameOverScene");
            SoundManager::GetInstance()->mPlaySound("GameOver", true);
        }
        return; // 더 이상 UI를 렌더링하지 않음
    }
    // Boss UI
    if (SceneManager::GetActiveScene()->GetName() == L"BossStage")
    {
        Boss* boss = Boss::GetInstance();
        if (boss)
        {
            int screenWidth = 1280;
            int bossBarX = (screenWidth - UI_BOSSBAR.GetWidth()) / 2;
            int bossBarY = 40; // Adjust vertical position as needed
            UI_BOSSBAR.Draw(hdc, bossBarX + 30, bossBarY);

            // Render boss name
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            HFONT hFont = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE, L"EXO 2");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            TextOut(hdc, bossBarX + 166, bossBarY - 30, L"서리 여왕 프리야", wcslen(L"서리 여왕 프리야"));
            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            float bossHpRatio = static_cast<float>(boss->GetHp()) / 800;
            int bossHpBarWidth = static_cast<int>(UI_BOSS_HPBAR.GetWidth() * bossHpRatio);
            if (bossHpBarWidth > 0) {
                int srcX = UI_BOSS_HPBAR.GetWidth() - bossHpBarWidth; // Start from right
                UI_BOSS_HPBAR.Draw(hdc, bossBarX + 80, bossBarY + 20, bossHpBarWidth, UI_BOSS_HPBAR.GetHeight(),
                    srcX, 0, bossHpBarWidth, UI_BOSS_HPBAR.GetHeight());
            }
        }
    }

    // 플레이어 UI
    {
        Player* player = SceneManager::GetSharedPlayer();

        UI_PLAYERBAR.Draw(hdc, 0, 0);
        // HP 바 렌더링 (오른쪽에서 왼쪽으로 줄어듦)
        float hpRatio = static_cast<float>(player->GetHp()) / 300;
        int hpBarWidth = static_cast<int>(UI_HPBAR.GetWidth() * hpRatio);
        if (hpBarWidth > 0) {
            int srcX = UI_HPBAR.GetWidth() - hpBarWidth;
            UI_HPBAR.Draw(hdc, 75, 13, hpBarWidth, UI_HPBAR.GetHeight(), srcX, 0, hpBarWidth, UI_HPBAR.GetHeight());
        }

        // MP 바 렌더링 (오른쪽에서 왼쪽으로 줄어듦)
        float mpRatio = static_cast<float>(player->GetMp()) / 100;
        int mpBarWidth = static_cast<int>(UI_MPBAR.GetWidth() * mpRatio);
        if (mpBarWidth > 0) {
            int srcX = UI_MPBAR.GetWidth() - mpBarWidth;
            UI_MPBAR.Draw(hdc, 75, 53, mpBarWidth, UI_MPBAR.GetHeight(), srcX, 0, mpBarWidth, UI_MPBAR.GetHeight());
        }

        int originalWidth = UI_SKILLBAR.GetWidth();
        int originalHeight = UI_SKILLBAR.GetHeight();
        int scaledWidth = static_cast<int>(originalWidth * 0.2f);
        int scaledHeight = static_cast<int>(originalHeight * 0.2f);
        int yPos = 720 - scaledHeight;
        UI_SKILLBAR.Draw(hdc, 0, yPos, scaledWidth, scaledHeight);

        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        HFONT hFont = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"PixelMplus10");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        // 플레이어 체력 텍스트 (현재체력/최대체력)
        wchar_t hpText[32];
        swprintf_s(hpText, L"%d / %d", player->GetHp(), 300);
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, 150, 16, hpText, wcslen(hpText));

        // 쿨타임 폰트 설정
        HFONT coolDownFont = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
        HFONT coolDownOldFont = (HFONT)SelectObject(hdc, coolDownFont);

        float fireBallCooldown = player->GetFireBallCooldown();
        float fireDragonCooldown = player->GetFireDragonCooldown();
        fireBallCooldown = (fireBallCooldown <= 0) ? 0 : fireBallCooldown;
        fireDragonCooldown = (fireDragonCooldown <= 0) ? 0 : fireDragonCooldown;
        wchar_t fireBallText[32];
        wchar_t fireDragonText[32];
        swprintf_s(fireBallText, L"%d", static_cast<int>(fireBallCooldown));
        swprintf_s(fireDragonText, L"%d", static_cast<int>(fireDragonCooldown));
        SetTextColor(hdc, RGB(0, 0, 0));
        // 쿨타임이 0이 아닐 때만 텍스트 출력
        if (fireBallCooldown > 0) {
            TextOut(hdc, 163, yPos + 80, fireBallText, wcslen(fireBallText));
        }
        if (fireDragonCooldown > 0) {
            TextOut(hdc, 233, yPos + 80, fireDragonText, wcslen(fireDragonText));
        }

        // 폰트 복원 및 삭제
        SelectObject(hdc, coolDownOldFont);
        DeleteObject(coolDownFont);
        SelectObject(hdc, hOldFont);
        DeleteObject(hFont);
    }
}