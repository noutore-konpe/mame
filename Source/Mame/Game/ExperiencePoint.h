#pragma once

#include <d3d11.h>
#include <string>
#include "../../Taki174/Common.h"
#include "../Graphics/Model.h"

// �o���l�N���X
class ExperiencePoint
{
private:
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
    struct CircularMotion
    {
        DirectX::XMFLOAT3 center_             = { 0.0f, 0.1f, 0.0f };
        DirectX::XMFLOAT3 angle_              = {};                          // Radian
        float             radius_             = 0.25f;
        float             addRotate_          = ::ToRadian(25.0f);
        float             correct_            = 0.25f;                       // �␳�l
        float             groundPositionY_    = center_.y + radius_;         // �~�^���̊J�n�n�_Y(�o���l�ɂƂ��Ă̒n�ʈʒuY)
        float             outGroundPositionY_ = groundPositionY_ + correct_; // �~�^���̊J�n�n�_Y(+�C���l)����ɂ�����󒆂ɂ�����̂Ƃ���
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

    void UpdateGroundPosition(); // �~�^���̒n�ʈʒu�X�V(�O�̂���)


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

