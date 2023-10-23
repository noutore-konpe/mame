#pragma once

#include <d3d11.h>
#include <string>
#include "../../Taki174/Common.h"
#include "../Graphics/Model.h"

// 経験値クラス
class ExperiencePoint
{
private:
    // ステップ
    enum class STEP
    {
        FALL_INIT,              // 落下初期化
        FALL,                   // 落下
        FLOTING_INIT,           // 浮遊初期化
        FLOTING,                // 浮遊
        MOVE_TO_PLAYER_INIT,    // プレイヤーの方へ移動初期化
        MOVE_TO_PLAYER,         // プレイヤーの方へ移動
    };

private:
    // 円運動構造体
    struct CircularMotion
    {
        DirectX::XMFLOAT3 center_       = { 0.0f, 0.25f, 0.0f };
        DirectX::XMFLOAT4 rotation_     = {};                   // Radian
        float             radius_       = 0.08f;
        float             addRotate_    = ::ToRadian(180.0f);
    };

public:
    ExperiencePoint();
    virtual ~ExperiencePoint();

    virtual void Initialize();
    virtual void Finalize() {}
    virtual void Begin() {}
    virtual void Update(const float elapsedTime);
    virtual void End() {}
    virtual void Render(const float scale, ID3D11PixelShader* psShader);
    virtual void DrawDebug();

    void UpdateConstants();


public:
    Transform* GetTransform() { return model_->GetTransform(); }

    const STEP& GetStep() const { return step_; }

    const DirectX::XMFLOAT3& GetVelocity() const { return velocity_; }
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ = velocity; }

    const std::string& GetName() const { return name_; }
    void SetName(const std::string& name) { name_ = name; }

    const float GetAttractTimer() const { return attractTimer_; }
    void SetAttractTimer(const float attractTimer) { attractTimer_ = attractTimer; }

    const float GetRadius() const { return radius_; }
    void SetRadius(const float radius) { radius_ = radius; }

private:
    const bool SearchPlayer();

    void UpdateVerticalVelocity(const float elapsedFrame);
    void UpdateHorizontalVelocity(const float elapsedFrame);
    void UpdateHorizontalMove(const float elapsedTime);

    void SetEmissiveIntensity(float intensity) { model_->skinned_meshes->data.emissiveIntensity = intensity; }
    void SetEmissiveScrollDirection(DirectX::XMFLOAT2 scroll) { model_->skinned_meshes->data.emissiveScrollDirection = scroll; }
    void SetEmissiveColor(DirectX::XMFLOAT4 color) { model_->skinned_meshes->data.emissiveColor = color; }

private:
    static int nameNum_;

public:
    std::unique_ptr<Model> model_;

private:
    STEP                step_           = STEP::FALL_INIT;
    CircularMotion      circularMotion_ = {};
    std::string         name_           = "";
    DirectX::XMFLOAT3   targetPosition_ = {};
    DirectX::XMFLOAT3   velocity_       = {};
    float               attractTimer_   = 0.8f;     // 最初の数フレームだけプレイヤーに向かわないにするタイマー
    float               lifeTimer_      = 60.0f;
    float               acceleration_   = 0.7f;
    float               gravity_        = (-0.1f);
    float               friction_       = 0.1f;
    float               airControl_     = 0.1f;
    float               radius_         = 0.1f;
    bool                isGround_       = false;
    bool                isMoveToPlayer_ = false;

    Microsoft::WRL::ComPtr<ID3D11PixelShader> expModelPS;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> emissiveTexture;
};

