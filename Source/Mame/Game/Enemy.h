#pragma once

#include "../../Taki174/OperatorXMFloat3.h"
#include "Character.h"

class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {}

    virtual void Initialize();                                          // 初期化
    virtual void Finalize() = 0;                                        // 終了化
    virtual void Begin()    = 0;                                           // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime);                      // 更新処理
    virtual void End()      = 0;                                             // 毎フレーム一番最後に呼ばれる
    //virtual void Render(const float& elapsedTime, const float& scale);  // 描画処理
    virtual void Render(const float& scale, ID3D11PixelShader* psShader);  // 描画処理
    virtual void DrawDebug() = 0;                                       // デバッグ描画

    //virtual void UpdateConstants() = 0;

public:
    const DirectX::XMFLOAT3& GetVelocity() const { return velocity_; }
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ = velocity; }
    void AddVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ += velocity; }

protected:
    DirectX::XMFLOAT3   velocity_       = {};
    int                 dropExpCount_   = 5;    // ドロップする経験値の数

};

