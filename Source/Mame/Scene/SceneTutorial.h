#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"

#include "../Graphics/Bloom.h"
#include "../Graphics/ShadowMap.h"
#include "../Graphics/Particle.h"

#include "../Game/EnemySlime.h"
#include "../Game/EnemyAura.h"
#include "../Game/EnemyGolem.h"

#include "../Game/Stage.h"

class SceneTutorial : public Mame::Scene::BaseScene
{
public:// 定数


public:// 基底クラスにある関数
    SceneTutorial() {}
    ~SceneTutorial() override {}

    void CreateResource() override;                 // リソース生成
    void Initialize()     override;                 // 初期化
    void Finalize()       override;                 // 終了化
    void Begin()          override;                 // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()            override;                 // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理
    void DrawDebug()      override;

public:// 関数

private:// 変数
    std::unique_ptr<Stage> stageBase;
    std::unique_ptr<Stage> stageWall;

private:// グラフィック関係
    std::unique_ptr<FrameBuffer> framebuffers[2];
    std::unique_ptr<FullscreenQuad> bitBlockTransfer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
    // SHADOW
    struct Shadow
    {
        const uint32_t shadowMapWidth = 2048;
        const uint32_t shadowMapHeight = 2048;
        std::unique_ptr<ShadowMap> shadowMap;
        DirectX::XMFLOAT4 lightViewFocus{ 0,0,0,1 };
        float lightViewDistance = 10.0f;
        float lightViewSize = 12.0f;
        float lightViewNearZ = 2.0f;
        float lightViewFarZ = 18.0f;
    } shadow;
    Microsoft::WRL::ComPtr<ID3D11Buffer> shadowConstantBuffer;

    // BLOOM
    std::unique_ptr<Bloom> bloomer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS;

    // FOG
    Microsoft::WRL::ComPtr<ID3D11PixelShader> fogPS;

public:
    static DirectX::XMFLOAT3 stageCenter;       // ステージの中心
    static constexpr float stageRadius = 16.7f; // ステージ用の半径
};

