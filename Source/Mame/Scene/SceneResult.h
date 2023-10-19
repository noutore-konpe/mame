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
    std::unique_ptr<EnemyGolemResult> enemyGolem = nullptr;
    std::unique_ptr<PlayerResult> player = nullptr;

    std::unique_ptr<Sprite> whiteSprite = nullptr;

    struct EnemyResult
    {
        DirectX::XMFLOAT3 position = {};
        DirectX::XMFLOAT3 scale = {};
        DirectX::XMFLOAT3 rotate = {};
        DirectX::XMFLOAT4 color = {};
    }enemyResult[3];

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> finalPassPS;
    Microsoft::WRL::ComPtr<ID3D11Buffer> resultConstantBuffer;
    struct ResultConstants
    {
        DirectX::XMFLOAT4 color{ 0.3f, 0.3f, 0.3f ,1.0f };
    }resultConstants;


};

