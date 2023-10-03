#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"

#include "../Graphics/Effect.h"
#include "../Graphics/Bloom.h"
#include "../Graphics/SkyBox.h"
#include "../Graphics/ShadowMap.h"
#include "../Graphics/Particle.h"

#include "../Game/Stage.h"

class SceneGame : public Mame::Scene::BaseScene
{
public: // 基底クラスにある関数
    SceneGame() {}
    ~SceneGame()override {}

    void CreateResource()override;                  // リソース生成
    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;

public: // 関数

public: // 設定・取得

private: // 変数等々
    // stage
    std::unique_ptr<Stage> stageBase;
    std::unique_ptr<Stage> stageWall;

    // enemy
    //std::unique_ptr<EnemyAura> enemyAura;

    // effect
    std::unique_ptr<Effect> effect[4];

    bool isDebugCamera = false;

private: // GPU用定数・変数
    std::unique_ptr<FrameBuffer> framebuffers[3];
    std::unique_ptr<FullscreenQuad> bitBlockTransfer;

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

    Microsoft::WRL::ComPtr<ID3D11PixelShader> colorPS;

    // BLOOM
    std::unique_ptr<Bloom> bloomer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS;

    // FOG
    Microsoft::WRL::ComPtr<ID3D11PixelShader> fogPS;

    // PARTICLE
    std::unique_ptr<Particles> particles;
    bool integrateParticles = true;

    // SKYBOX
    std::shared_ptr<Sprite> skyBoxSprite;
    std::unique_ptr<SkyBox> skyBox;

    // EMISSIVE
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;

    // PostEffect
    Microsoft::WRL::ComPtr<ID3D11PixelShader> postEffectPS;

    // PS Shader
    Microsoft::WRL::ComPtr<ID3D11PixelShader> sagePS; // 透明のシェーダー

};

