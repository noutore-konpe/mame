#pragma once

#include <memory>
#include "../../Graphics/Model.h"
#include "../../Resource/GeometricPrimitive.h"
#include "../Collision.h"

class Terrain
{
public:
    // 地形の種類
    enum class Type
    {
        None    = -1,
        Normal,         // 普通の地形
        Move,           // 動く地形
        NoClimb,        // 登れない地形
    };

public:
    Terrain(const char* const filename);
    virtual ~Terrain() = default;

    virtual void Initialize();                           // 初期化
    virtual void Finalize()                       {}     // 終了化
    virtual void Update(const float& elapsedTime) {}     // 更新処理
    virtual void Render(const float& elapsedTime);       // 描画処理
    virtual void DrawDebug();                            // デバッグ描画

    virtual void OnRiding() {}                           // 乗られた時に呼ばれる

    void Destroy();                                      // 自分を消去する

public:
    void UpdateAABB();                                   // AABB更新処理

public:
    Transform* GetTransform() const { return model_->GetTransform(); }   // トランスフォーム取得

    // Imgui名前被り防止のためのやつ //
    const char* const GetName() const { return name.c_str(); }
    void SetName(const char* const n) { name = n; }

    const DirectX::XMFLOAT4& GetMaterialColor() const { return materialColor; }
    void SetMaterialColor(const DirectX::XMFLOAT4& color) { materialColor = color; }
public:
    std::unique_ptr<Model> model_;                         // モデル

    std::unique_ptr<GeometricPrimitive> geometricAABB_;   // 当たり判定描画用ジオメトリックプリミティブ
    Collision::AABB aabb_ = {};                           // 当たり判定構造体

    Terrain::Type terrainType_ = Terrain::Type::None;

public:
    static float    renderLengthXLimit_;     // 描画される限界距離
    static int      nameNum;

public:
    bool isCollisionPlayer_ = true;    //　プレイヤーと当たり判定をさせるか(デフォルトはtrue)
    bool isCollisionEnemy_  = true;    //　エネミーと当たり判定をさせるか(デフォルトはtrue)

protected:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shaders = {};

    DirectX::XMFLOAT4   debugMaterialColor  = { 1.0f, 0.0f, 0.0f, 0.4f }; // マテリアルカラー(debug用)
    DirectX::XMFLOAT4   materialColor       = { 1.0f, 1.0f, 1.0f, 1.0f };

    DirectX::XMFLOAT3   velocity_           = {};
    DirectX::XMFLOAT3   originPosition_     = {};

private:
    // Imgui名前被り防止のためのやつ //    
    std::string name = {};   // Imgui用

    DirectX::XMFLOAT3 defaultMin_ = {};
    DirectX::XMFLOAT3 defaultMax_ = {};

};