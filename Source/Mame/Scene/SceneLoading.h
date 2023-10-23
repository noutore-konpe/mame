#pragma once

#include "BaseScene.h"

#include <thread>
#include <memory>

#include "../Game/LoadingPlayer.h"
#include "../Resource/sprite.h"

class SceneLoading : public Mame::Scene::BaseScene
{
public:
    SceneLoading(BaseScene* nextScene);
    ~SceneLoading() override {}

    void CreateResource()override;                  // リソース生成
    void Initialize()   override;                   // 初期化
    void Finalize()     override;                   // 終了化
    void Begin()        override;                   // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime) override; // 更新処理
    void End()          override;                   // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime) override; // 描画処理

    void DrawDebug()    override;

private:
    // ローディングスレッド
    static void LoadingThread(SceneLoading* scene);

private:
    BaseScene*      nextScene   = nullptr;
    std::thread*    thread      = nullptr;

private:
    std::unique_ptr<LoadingPlayer> loadingPlayer = nullptr;
    std::unique_ptr<Sprite> titleLogo = nullptr;

    Microsoft::WRL::ComPtr<ID3D11Buffer> playerConstant;
    struct PlayerConstants
    {
        DirectX::XMFLOAT4 color;
    }playerConstants;

    bool isChangeScene = false;
    float easingTimer = 0.0f;

    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
};

