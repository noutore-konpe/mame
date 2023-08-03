#pragma once
#include "BaseScene.h"

#include "../Resource/sprite.h"
#include "../Resource/GltfModel.h"
#include "../Graphics/Model.h"
#include "../Graphics/Effect.h"

#include <memory>

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

public:
    

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
};

