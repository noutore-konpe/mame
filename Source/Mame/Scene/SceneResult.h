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
public:// 定数
    enum class Ability
    {
        Absorption,     // 吸収
        AttackSpeed,    // 攻撃スピード
        Defense,        // 防御力
        Power,          // 攻撃力
        Book,           // 本
        Homing,         // ホーミング
        Poison,         // 毒
        BlackHole,      // ブラックホール
        HpUp,           // Hp上昇
        MoveSpeed,      // 移動速度
        Max,            // 最大
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
    void UpdateEnemyKillNumAndx(const float& elapsedTime);

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
    
    std::unique_ptr<Sprite> lifeTimeSprite = nullptr;
    std::unique_ptr<Sprite> waveSprite = nullptr;
    std::unique_ptr<Sprite> lvSprite = nullptr;

    std::unique_ptr<Sprite> xSprite = nullptr;
    std::unique_ptr<Sprite> numSprite = nullptr;

    std::unique_ptr<Sprite> iconSprite[static_cast<UINT>(Ability::Max)];

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
    };

    SlideStruct KillX;
    SlideStruct killNum;
    bool isSlide = false;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> finalPassPS;
    Microsoft::WRL::ComPtr<ID3D11Buffer> resultConstantBuffer;
    struct ResultConstants
    {
        DirectX::XMFLOAT4 color{ 0.3f, 0.3f, 0.3f ,1.0f };
    }resultConstants;
};

