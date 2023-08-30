#pragma once
#include "BaseScene.h"

#include <memory>

#include "../Resource/sprite.h"
#include "../Resource/GltfModel.h"
#include "../Graphics/Model.h"
#include "../Graphics/Effect.h"

#include "../Game/EnemySlime.h"

#include "../Graphics/Bloom.h"

#include "../Game/Stage.h"

#include "../Graphics/SkyBox.h"

#include "../Graphics/ShadowMap.h"

#define GLTF_MODEL 0
#define MODEL 0
#define SPRITE 1
#define BLOOM 0
#define SKYBOX 1

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
    enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC, LINEAR_BORDER_BLACK, LINEAR_BORDER_WHITE };
    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[5];

    enum class DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[4];

    enum class BLEND_STATE { NONE, ALPHA, ADD, MULTIPLY };
    Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[4];

    enum class RASTER_STATE { SOLID, WIREFRAME, CULL_NONE, WIREFRAME_CULL_NONE };
    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[4];

    void SetStates();

#if BLOOM
    std::unique_ptr<FrameBuffer> framebuffers[8];
    std::unique_ptr<FullscreenQuad> bit_block_transfer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders[8];

    // BLOOM
    std::unique_ptr<Bloom> bloomer;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
#endif // BLOOM

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

    // stage
    std::unique_ptr<Stage> stage;

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

};