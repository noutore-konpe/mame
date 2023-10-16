#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"
#include "../Graphics/ShadowMap.h"
#include "../Graphics/Bloom.h"
#include "../Graphics/Particle.h"

#include "../Game/Stage.h"

class SceneTitle : public Mame::Scene::BaseScene
{
public:
    SceneTitle() {}
    ~SceneTitle()       override {}

    void CreateResource()override;                  // リソース生成
    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;

private:
    std::unique_ptr<Stage> stageBase;
    std::unique_ptr<Stage> stageWall;

    std::unique_ptr<Sprite> backSprite;
    std::unique_ptr<Sprite> emmaSprite;
    std::unique_ptr<Sprite> pressSprite;

    // 変数
    bool isAlphaDown = false;
    float pressTimer = 0.0f;

private:
    std::unique_ptr<FrameBuffer> framebuffers[3];
    std::unique_ptr<FullscreenQuad> bitBlockTransfer;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> finalPassPS;


    // BLOOM
    std::unique_ptr<Bloom> bloomer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> bloomPS;

    // FOG
    Microsoft::WRL::ComPtr<ID3D11PixelShader> fogPS;

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
};

