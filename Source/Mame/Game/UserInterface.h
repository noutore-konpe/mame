#pragma once
#include "../Resource/sprite.h"
#include <memory>

class UserInterface
{
public:
    enum class STATE
    {
        Idle,       // 待機
        CreateWhite,// 白背景
        FadeIn,     // フェードイン
        Display,    // 表示
        FadeOut,    // フェードアウト
        DeleteWhite,// 白背景
    };

private:
    UserInterface() {}
    ~UserInterface() {}

public:
    static UserInterface& Instance()
    {
        static UserInterface instance;
        return instance;
    }

    void Initialize();
    void Update(float elapsedTime);
    void Render();
    void BloomRender();//ブルーム効果付きの描画
    void DrawDebug();

public:// 設定・取得
    bool GetLockOnInitialize() { return isLockOnInitialize; }
    void SetLockOnInitialize() { isLockOnInitialize = true; }

    void SetWaveSlideSprite() { isWaveSlideSprite = true; }

private:
    void UpdateLockOnSprite(const float& elapsedTime);
    void UpdateWaveSprite(const float& elapsedTime);
    void UpdateHpExpGauge();

    void RenderWaveSlide();
    void RenderLv();
    void RenderWave();

private:
    std::unique_ptr<Sprite> lockOnSprite;
    std::unique_ptr<Sprite> lockOnMaruSprite;

    std::unique_ptr<Sprite> hpSprite;           // hp
    std::unique_ptr<Sprite> keikenchiSprite;    // 経験値ゲージ
    std::unique_ptr<Sprite> maruSprite;         // 丸
    std::unique_ptr<Sprite> backGageSprite[2];  // ゲージ背景

    std::unique_ptr<Sprite> lvSprite;   // Lv
    std::unique_ptr<Sprite> waveSprite; // Wave
    std::unique_ptr<Sprite> numSprite;  // num
    std::unique_ptr<Sprite> chonchonSprite; // :

    std::unique_ptr<Sprite> whiteSprite;        // 白画像
    std::unique_ptr<Sprite> waveSlideSprite;    // 流れていく画像(Wave)
    std::unique_ptr<Sprite> numSlideSprite;     // 流れていく画像(数字)
    DirectX::XMFLOAT2 numPosition[4] = {};    // 

    bool isLockOnInitialize = false;
    float lockOnTimer = 0.0f;

    bool isWaveSlideSprite = false; // 画面真ん中に出す処理
    int slideState = 0;             // スライド用
    float easingTimer = 0.0f;       // easing用

    float hpSpriteSizeX;//サイズの初期値を保持しときたいだけ
    float expSpriteSizeX;
};

