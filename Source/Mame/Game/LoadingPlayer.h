#pragma once
#include "Character.h"

class LoadingPlayer : public Character
{
public:
    LoadingPlayer();
    ~LoadingPlayer() override {}

    void Initialize() override;                     // 初期化
    void Finalize();                                // 終了化

    void Update(const float elapsedTime) override; // 更新処理
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr) override; // 描画処理

    void DrawDebug() override;  // ImGui用

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> loadPS;

    std::unique_ptr<Model> swordModel;
};

