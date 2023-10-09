#pragma once

#include <d3d11.h>
#include <string>
#include "../../Taki174/Common.h"
#include "../Graphics/Model.h"

// 経験値クラス
class ExperiencePoint
{
private:
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
    struct CircularMotion
    {
        DirectX::XMFLOAT3 center_             = { 0.0f, 0.1f, 0.0f };
        DirectX::XMFLOAT3 angle_              = {};                          // Radian
        float             radius_             = 0.25f;
        float             addRotate_          = ::ToRadian(25.0f);
        float             correct_            = 0.25f;                       // 補正値
        float             groundPositionY_    = center_.y + radius_;         // 円運動の開始地点Y(経験値にとっての地面位置Y)
        float             outGroundPositionY_ = groundPositionY_ + correct_; // 円運動の開始地点Y(+修正値)より上にいたら空中にいるものとする
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

public:
    Transform* GetTransform() { return model_->GetTransform(); }

    const char* const GetName() const { return name_.c_str(); }
    void SetName(const std::string& name) { name_ = name; }

private:
    const bool SearchPlayer();

    void UpdateHorizontalVelocity(const float elapsedFrame);
    void UpdateHorizontalMove(const float elapsedTime);

    void UpdateGroundPosition(); // 円運動の地面位置更新(念のため)


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
    float               gravity_        = -1.0f;
    float               friction_       = 0.5f;
    float               airControl_     = 0.3f;
    bool                isGround_       = false;

};

