#pragma once

#include "../Graphics/Model.h"
#include "Character.h"

// 前方宣言
class ProjectileManager;

class Projectile
{
public:
    Projectile(ProjectileManager* manager);
    virtual ~Projectile() {}

    virtual void Initialize();                      // 初期化
    virtual void Finalize();                        // 終了化
    virtual void Begin();                           // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime);  // 更新処理
    virtual void End();                             // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float& scale, ID3D11PixelShader* psShader = nullptr);  // 描画処理
    virtual void DrawDebug();                       // ImGui用

public:
    void Destroy(); // 破棄

public: // 取得・設定　関連
    Transform* GetTransform() { return model_->GetTransform(); }

    const Character* GetParent() const { return parent_; }
    void SetParent(Character* parent) { parent_ = parent; }

    const float GetRadius() const { return radius_; }   // 半径取得
    void SetRadius(float r) { radius_ = r; }            // 半径設定

    // --- ImGui用 --- //
    const char* GetName() const { return name_.c_str(); }
    void SetName(std::string n) { name_ = n; }

public:
    std::unique_ptr<Model> model_ = nullptr;

private:
    ProjectileManager* manager_ = nullptr;

    Character* parent_ = nullptr;  // 自分を生成したキャラクターのポインタ

    float radius_ = 0.5f;

    // --- ImGui用 --- //
    std::string name_ = {};

};

