#pragma once

#include <d3d11.h>
#include <string>
#include "../../Taki174/Common.h"
#include "../Graphics/Model.h"

// �o���l�N���X
class ExperiencePoint
{
private:
    // �X�e�b�v
    enum class STEP
    {
        FALL_INIT,              // ����������
        FALL,                   // ����
        FLOTING_INIT,           // ���V������
        FLOTING,                // ���V
        MOVE_TO_PLAYER_INIT,    // �v���C���[�̕��ֈړ�������
        MOVE_TO_PLAYER,         // �v���C���[�̕��ֈړ�
    };

private:
    // �~�^���\����
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

public:
    Transform* GetTransform() { return model_->GetTransform(); }

    const STEP& GetStep() const { return step_; }


    const DirectX::XMFLOAT3& GetVelocity() const { return velocity_; }
    void SetVelocity(const DirectX::XMFLOAT3& velocity) { velocity_ = velocity; }

    const std::string& GetName() const { return name_; }
    void SetName(const std::string& name) { name_ = name; }

    const float GetRadius() const { return radius_; }
    void SetRadius(const float radius) { radius_ = radius; }

private:
    const bool SearchPlayer();

    void UpdateVerticalVelocity(const float elapsedFrame);
    void UpdateHorizontalVelocity(const float elapsedFrame);
    void UpdateHorizontalMove(const float elapsedTime);

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
    float               lifeTimer_      = 60.0f;
    float               acceleration_   = 0.7f;
    float               gravity_        = (-0.1f);
    float               friction_       = 0.1f;
    float               airControl_     = 0.1f;
    float               radius_         = 0.1f;
    bool                isGround_       = false;
    bool                isMoveToPlayer_ = false;

};

