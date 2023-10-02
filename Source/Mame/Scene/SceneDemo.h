#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"
#include "../Resource/GltfModel.h"
#include "../Graphics/Model.h"
#include "../Graphics/Effect.h"

#include "../Game/EnemySlime.h"
#include "../Game/EnemyAura.h"

#include "../Game/Stage.h"

#include "../Graphics/Bloom.h"
#include "../Graphics/SkyBox.h"
#include "../Graphics/ShadowMap.h"
#include "../Graphics/Particle.h"
#include "../Graphics/AuraEffect.h"

#define GLTF_MODEL 0
#define MODEL 0
#define SPRITE 0
#define BLOOM 1
#define SKYBOX 1
#define FOG 1
#define PARTICLE 1

class SceneDemo : public Mame::Scene::BaseScene
{
public:
    SceneDemo() {}
    ~SceneDemo()override {}

    void CreateResource()override;                  // リソース生成
    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;

    static bool isDebugRender;
    
public:

    std::unique_ptr<FrameBuffer> framebuffers[8];
    std::unique_ptr<FullscreenQuad> bitBlockTransfer;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> baseColorPS;

#if BLOOM
    // BLOOM
    std::unique_ptr<Bloom> bloomer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS;
#endif // BLOOM

#if FOG
    Microsoft::WRL::ComPtr<ID3D11PixelShader> fogPS;
#endif// FOG

#if PARTICLE
    bool integrateParticles = true;
    std::unique_ptr<Particles> particles;
#endif// PARTICLE

    // ZELDA
    Microsoft::WRL::ComPtr<ID3D11PixelShader> auraEffectPS;
    std::unique_ptr<AuraEffect> auraEffect;
    std::vector<AuraEffect::Emitter> auraEmitters;

    // EMISSIVE : emissiveTextuerUVScroll
    Microsoft::WRL::ComPtr<ID3D11PixelShader> emissiveTextureUVScrollPS;

private:
    bool isDebugCamera = false;

private:
    // GltfModel test
    std::unique_ptr<GltfModel> gltfModels[8];
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceViews[8];

    // Model test
    std::unique_ptr<Model> model;

    // Sprite test
    std::unique_ptr<Sprite>sprite;

    // Effekt test
    std::unique_ptr<Effect> effect[4];

    // slime
    std::unique_ptr<EnemySlime> enemySlime[2];

    std::unique_ptr<EnemyAura> enemyAura;

    // stage
    std::unique_ptr<Stage> stageBase;
    std::unique_ptr<Stage> stageWall;

    // ZELDA
    Microsoft::WRL::ComPtr<ID3D11PixelShader> sagePS;

    // skybox
#if SKYBOX
    std::shared_ptr<Sprite> skyBoxSprite;
    std::unique_ptr<SkyBox> skyBox;
#endif // SKYBOX


    // SHADOW
    Microsoft::WRL::ComPtr<ID3D11Buffer> SceneConstantBuffer;

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
    }shadow;

    // EMISSIVE
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiceTexture;
};