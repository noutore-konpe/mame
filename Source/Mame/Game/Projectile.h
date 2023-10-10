#pragma once

#include "../Graphics/Model.h"
#include "Character.h"

class ProjectileManager;

class Projectile
{
public:
    Projectile(
        ProjectileManager* manager,
        const bool isPoison = false
    );
    virtual ~Projectile() {}

    virtual void Initialize();
    virtual void Finalize();
    virtual void Begin();
    virtual void Update(const float elapsedTime);
    virtual void End();
    virtual void Render(const float scale, ID3D11PixelShader* psShader = nullptr);
    virtual void DrawDebug();

public:
    void Destroy();

public:
    Transform* GetTransform() { return model_->GetTransform(); }

    const DirectX::XMFLOAT3& GetPosition() { return GetTransform()->GetPosition(); }
    void SetPosition(const DirectX::XMFLOAT3& pos) { GetTransform()->SetPosition(pos); }

    const Character* GetParent() const { return parent_; }
    void SetParent(Character* parent) { parent_ = parent; }

    const float GetRadius() const { return radius_; }
    void SetRadius(const float radius) { radius_ = radius; }

    const char* const GetName() const { return name_.c_str(); }
    void SetName(const std::string& name) { name_ = name; }

public:
    std::unique_ptr<Model> model_;

protected:
    ProjectileManager*  manager_    = nullptr;
    Character*          parent_     = nullptr;  // 自分を生成したキャラクターのポインタ
    std::string         name_       = "";
    float               radius_     = 0.5f;

    bool                isPoison_;

};

