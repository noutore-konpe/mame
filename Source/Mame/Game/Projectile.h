#pragma once

#include "../Graphics/Model.h"

// 前方宣言
class ProjectileManager;

class Projectile
{
public:
    Projectile(ProjectileManager* manager);
    virtual ~Projectile() {}

    virtual void Initialize();                                          // 初期化
    virtual void Finalize();                                            // 終了化
    virtual void Begin();                                               // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime);                      // 更新処理
    virtual void End();                                                 // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float& elapsedTime, const float& scale);  // 描画処理
    virtual void DrawDebug();                                           // ImGui用

public:
    void Destroy(); // 破棄

public: // 取得・設定　関連
    Transform* GetTransform() { return model->GetTransform(); }

    float GetRadius()const { return radius; }   // 半径取得
    void SetRadius(float r) { radius = r; }     // 半径設定

public:
    std::unique_ptr<Model> model = nullptr;

private:
    ProjectileManager* manager = nullptr;

    float radius = 0.5f;

public: // --- ImGui用 --- //
    const char* GetName() const { return name.c_str(); }
    void SetName(std::string n) { name = n; }
    static int nameNum;

private: // --- ImGui用 --- //
    std::string name = {};
};

