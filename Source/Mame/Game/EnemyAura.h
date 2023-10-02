#pragma once

#include "Enemy.h"

class EnemyAura : public Enemy
{
public:
    EnemyAura();
    ~EnemyAura() override;

    void Initialize()                                           override; // 初期化
    void Finalize()                                             override; // 終了化
    void Begin()                                                override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)                       override; // 更新処理
    void End()                                                  override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // 描画処理
    void DrawDebug()                                            override; // デバッグ描画

    void UpdateConstants() override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> emissiveTextureUVScroll;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;

};

