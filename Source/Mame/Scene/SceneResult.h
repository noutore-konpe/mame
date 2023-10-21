#pragma once
#include "BaseScene.h"

#include <memory>

#include "../FrameBuffer.h"
#include "../FullscreenQuad.h"

#include "../Resource/sprite.h"

#include "../Game/EnemyGolemResult.h"
#include "../Game/PlayerResult.h"

class SceneResult : public Mame::Scene::BaseScene
{
private:// 定数
    enum class STATE
    {
        Initialize,
        LifeTime,
        Wave,
        Lv,
        Icon,
        Enemy,
        EnemyKill,
        Max,
    };

    float lifeTimePos[4] =
    {
        300,
        350,
        450,
        500,
    };

    static const int iconMax = 15;

    // アイコン画像の位置
    DirectX::XMFLOAT2 iconPos[iconMax] =
    {
        {  670, 150 }, 
        {  780, 150 }, 
        {  890, 150 }, 
        { 1000, 150 }, 
        { 1110, 150 },  // 5
        {  670, 300 },
        {  780, 300 },
        {  890, 300 },
        { 1000, 300 },
        { 1110, 300 },  // 10
        {  670, 450 },
        {  780, 450 },
        {  890, 450 },
        { 1000, 450 },
        { 1110, 450 },  // 15
    };

    DirectX::XMFLOAT2 skillXPos[iconMax] =
    {
        {  620, 220 },
        {  730, 220 },
        {  840, 220 },
        {  950, 220 },
        { 1060, 220 }, // 5
        {  620, 370 },
        {  730, 370 },
        {  840, 370 },
        {  950, 370 },
        { 1060, 370 }, // 10
        {  620, 520 },
        {  730, 520 },
        {  840, 520 },
        {  950, 520 },
        { 1060, 520 }, // 15
    };

    DirectX::XMFLOAT2 skillNumPos[iconMax] =
    {
        {  650, 205 },
        {  760, 205 },
        {  870, 205 },
        {  980, 205 },
        { 1090, 205 }, // 5
        {  650, 355 },
        {  760, 355 },
        {  870, 355 },
        {  980, 355 },
        { 1090, 355 }, // 10
        {  650, 505 },
        {  760, 505 },
        {  870, 505 },
        {  980, 505 },
        { 1090, 505 }, // 15
    };

public:
    SceneResult() {}
    ~SceneResult()       override {}

    void CreateResource()override;                  // リソース生成
    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;

private:
    void UpdateLifeTimeNum(const float& elapsedTime);
    void UpdateWave(const float& elapsedTime);
    void UpdateLv(const float& elapsedTime);
    void UpdateIcon(const float& elapsedTime);
    void UpdateEnemyKillNumAndx(const float& elapsedTime);

    void RenderLifeTime();
    void RenderWave();
    void RenderLv();

    void RenderSkill();
    void RenderSkillX();
    void RenderSkillNum();
    void RenderEnemyKillX();
    void RenderEnemyKillNum();
    /// <summary>
    /// 四桁まで対応する数字の描画
    /// </summary>
    /// <param name="who">誰のカウントか</param>
    /// <param name="firstPosX">1桁目</param>
    /// <param name="secondPosX">2桁目</param>
    /// <param name="thirdPosX">3桁目</param>
    /// <param name="fourthPosX">4桁目</param>
    void RenderNum(const int who, const float firstPosX, const float secondPosX, const float thirdPosX, const float fourthPosX);

    void RenderEnemyModel();

private:
    std::unique_ptr<EnemyGolemResult> enemyGolem = nullptr;
    std::unique_ptr<PlayerResult> player = nullptr;

    std::unique_ptr<Sprite> backSprite = nullptr;
    std::unique_ptr<Sprite> emmaSprite = nullptr;
       
    
    std::unique_ptr<Sprite> lifeTimeSprite = nullptr;
    std::unique_ptr<Sprite> waveSprite = nullptr;
    std::unique_ptr<Sprite> lvSprite = nullptr;

    std::unique_ptr<Sprite> xSprite = nullptr;
    std::unique_ptr<Sprite> numSprite = nullptr;

    std::unique_ptr<Sprite> chonchonSprite = nullptr;

    struct EnemyResult
    {
        DirectX::XMFLOAT3 position = {};
        DirectX::XMFLOAT3 scale = {};
        DirectX::XMFLOAT3 rotate = {};
        DirectX::XMFLOAT4 color = {};
    }enemyResult[3];


    struct SlideStruct
    {
        float addPosX = 0.0f;
        float easingTimer = 0.0f;
        float alpha = 0.0f;
    };

    SlideStruct lifeTimer;      // lifeTimerの文字
    SlideStruct lifeTimerNum;   // lifeTimerの数字
    SlideStruct wave;       // waveの文字
    SlideStruct waveNum;    // waveの数字
    SlideStruct skillX;
    SlideStruct skillNum;
    SlideStruct KillX;
    SlideStruct killNum;

    bool isSlide = false;
    bool isLifeTimer = false;
    bool isWave = false;
    bool isLv = false;
    bool isIconUpdateEnd = false;

    struct IconStruct
    {
        float easingTimer = 0.0f;   // easing用タイマー
        float scale = 0.0f;
        bool isDisplay = false;     // 表示するか
    }iconStruct[iconMax];

    int resultState = 0;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> finalPassPS;
    Microsoft::WRL::ComPtr<ID3D11Buffer> resultConstantBuffer;
    struct ResultConstants
    {
        DirectX::XMFLOAT4 color{ 0.3f, 0.3f, 0.3f ,1.0f };
    }resultConstants;
};

