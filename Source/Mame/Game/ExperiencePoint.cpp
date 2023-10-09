#include "ExperiencePoint.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Graphics/Graphics.h"
#include "PlayerManager.h"

int ExperiencePoint::nameNum_ = 0;

ExperiencePoint::ExperiencePoint()
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(
        graphics.GetDevice(),
        "./Resources/Model/cube.fbx"
    );

    SetName("Exp_" + std::to_string(nameNum_++));
}


ExperiencePoint::~ExperiencePoint()
{
    --nameNum_;
}


void ExperiencePoint::Initialize()
{
    GetTransform()->SetScaleFactor(3.0f);

    step_ = STEP::FALL_INIT;

}


void ExperiencePoint::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();
    Transform* t = GetTransform();
    const float elapsedFrame = elapsedTime * 60;

    UpdateGroundPosition(); // �~�^���̒n�ʈʒu�X�V

    switch (step_)
    {
    case STEP::FALL_INIT:

        velocity_.y = 0.0f; // �������x���Z�b�g

        step_ = STEP::FALL;
        [[fallthrough]];
    case STEP::FALL:

        // �v���C���[������������v���C���[�̕��֌�����
        if (true == SearchPlayer())
        {
            step_ = STEP::MOVE_TO_PLAYER_INIT;
            break;
        }

        // �ʒu�X�V
        {
            // Y�ʒu�X�V
            {
                t->AddPositionY(gravity_ * elapsedFrame);

                // �~�^���̊J�n�n�_�܂ō~�������畂�V��ԂɈڍs����
                if (t->GetPositionY() <= circularMotion_.groundPositionY_)
                {
                    t->SetPositionY(circularMotion_.groundPositionY_);

                    step_ = STEP::FLOTING_INIT;
                    break;
                }
            }

            // XZ�ʒu�X�V
            {
                UpdateHorizontalVelocity(elapsedFrame);
                UpdateHorizontalMove(elapsedTime);
            }
        }

        break;
    case STEP::FLOTING_INIT:

        // �����̈ʒu�����]���n�߂�悤�ɂ���
        {
            t->SetPositionY(circularMotion_.groundPositionY_); // �~�^���̊J�n�n�_Y�Ɉʒu�ݒ�
            const float localPositionX = t->GetPositionX() - circularMotion_.center_.x;
            const float localPositionY = t->GetPositionY() - circularMotion_.center_.y;
            circularMotion_.angle_.z = ::atan2f(localPositionX, localPositionY); // return Radian Angle
        }

        step_ = STEP::FLOTING;
        [[fallthrough]];
    case STEP::FLOTING:

        // �v���C���[������������v���C���[�̕��֌�����
        if (true == SearchPlayer())
        {
            step_ = STEP::MOVE_TO_PLAYER_INIT;
            break;
        }

        // �󒆂ɂ����痎����ԂɈڍs����
        if (t->GetPositionY() >= circularMotion_.outGroundPositionY_)
        {
            step_ = STEP::FALL_INIT;
            break;
        }

        // XZ�ʒu�X�V
        {
            UpdateHorizontalVelocity(elapsedFrame);
            UpdateHorizontalMove(elapsedTime);
        }

        // Y����]
        {
            circularMotion_.angle_.z += circularMotion_.addRotate_ * elapsedTime;
            if (circularMotion_.angle_.z > 360.0f) { circularMotion_.angle_.z -= 360.0f; }
            t->SetPositionY(
                circularMotion_.center_.y +
                ::cosf(circularMotion_.angle_.z) *
                circularMotion_.radius_
            );
        }

        break;
    case STEP::MOVE_TO_PLAYER_INIT:

        step_ = STEP::MOVE_TO_PLAYER;
        [[fallthrough]];
    case STEP::MOVE_TO_PLAYER:

        // �v���C���[���痣��Ă�����ق��̏�ԂɈڂ�
        if (false == SearchPlayer())
        {
            // �ʒu���n�ʈȉ��Ȃ畂�V��ԂɈڍs����
            if (t->GetPositionY() <= circularMotion_.groundPositionY_)
            {
                t->SetPositionY(circularMotion_.groundPositionY_);

                step_ = STEP::FLOTING_INIT;
                break;
            }

            // �󒆂ɂ����痎����ԂɈڍs����
            if (t->GetPositionY() >= circularMotion_.outGroundPositionY_)
            {
                step_ = STEP::FALL_INIT;
                break;
            }
        }
        else
        {
            // �v���C���[�̕��֌�����
            const XMFLOAT3 pos    = GetTransform()->GetPosition();
            const XMFLOAT3 plPos  = plManager.GetPlayer()->GetTransform()->GetPosition();
            const XMFLOAT3 vecN = ::XMFloat3Normalize(plPos - pos);
            velocity_ += 1.0f * elapsedTime; // ��(�v���C���[�Ɉˑ��ɂ���\��)
            t->AddPosition(vecN * velocity_ * elapsedTime);
        }

        break;
    }
}


void ExperiencePoint::Render(const float scale, ID3D11PixelShader* psShader)
{
    Graphics::Instance().GetShader()->SetRasterizerState(
        static_cast<int>(Shader::RASTER_STATE::SOLID)
    );

    model_->Render(scale, psShader);
}


void ExperiencePoint::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(GetName()))
    {
        model_->DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}


const bool ExperiencePoint::SearchPlayer()
{
    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();

    const XMFLOAT3 pos      = GetTransform()->GetPosition();
    const XMFLOAT3 plPos    = plManager.GetPlayer()->GetTransform()->GetPosition();
    const XMFLOAT3 vec      = plPos - pos;
    const float    lengthSq = ::XMFloat3LengthSq(vec);

    constexpr float plCollectExpLengthSq = 2.0f * 2.0f; // ��(�v���C���[�Ɉˑ�����\��)
    if (lengthSq > plCollectExpLengthSq) return false;

    return true;

}


void ExperiencePoint::UpdateHorizontalVelocity(const float elapsedFrame)
{
    const float velocityXZLength = ::sqrtf(
        (velocity_.x * velocity_.x) +
        (velocity_.z * velocity_.z)
    );

    if (velocityXZLength > 0.0f)
    {
        // ���C��
        float friction = friction_ * elapsedFrame;

        // �󒆂ɂ���Ƃ��͖��C�͂����炷
        if (!isGround_) friction *= airControl_;

        // ���C�ɂ�鉡�����̌�������
        if (velocityXZLength > friction)
        {
            // �P�ʃx�N�g����
            const float vx = velocity_.x / velocityXZLength;
            const float vz = velocity_.z / velocityXZLength;

            velocity_.x -= vx * friction;
            velocity_.z -= vz * friction;
        }
        // �������̑��x�����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
        else
        {
            velocity_.x = 0.0f;
            velocity_.z = 0.0f;
        }
    }

}

void ExperiencePoint::UpdateHorizontalMove(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    Transform* t = GetTransform();

    // �������͗ʌv�Z
    const float velocityLengthXZ = ::sqrtf(
        velocity_.x * velocity_.x +
        velocity_.z * velocity_.z
    );

    if (velocityLengthXZ > 0.0f)
    {
#ifdef USE_RAYCAST
        // �����ړ��l
        float velocityX = velocity_.x * elapsedTime;
        float velocityZ = velocity_.z * elapsedTime;

        // ���C�Ƃ̊J�n�ʒu�ƏI�_�ʒu
        DirectX::XMFLOAT3 start = { position.x , position.y + stepOffset , position.z };
        DirectX::XMFLOAT3 end = { position.x + velocityX , position.y + stepOffset , position.z + velocityZ };

        // ���C�L���X�g�ɂ��ǔ���
        HitResult hit;
        if (Stage::Instance().RayCast(start, end, hit))
        {
            // �ǂ܂ł̃x�N�g��
            DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
            DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
            DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

            // �ǂ̖@��
            DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

            //���˃x�N�g����@���Ɏˉe
            DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);

            // �␳�ʒu�̌v�Z
            DirectX::XMVECTOR CollectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
            DirectX::XMFLOAT3 collectPosition;
            DirectX::XMStoreFloat3(&collectPosition, CollectPosition);

            // �ǂ�������փ��C�L���X�g
            HitResult hit2;
            if (!Stage::Instance().RayCast(hit.position, collectPosition, hit2))
            {
                // �ǂ�������ŕǂɓ�����Ȃ�������␳�ʒu�Ɉړ�
                position.x = collectPosition.x;
                position.z = collectPosition.z;
            }
            else
            {
                position.x = hit2.position.x;
                position.z = hit2.position.z;
            }
        }
        else
        {
            // �ړ�
            position.x += velocityX;
            position.z += velocityZ;
        }

#else
        // �ړ�
        t->AddPositionX(velocity_.x * elapsedTime);
        t->AddPositionZ(velocity_.z * elapsedTime);

#endif
    }

}


void ExperiencePoint::UpdateGroundPosition()
{
    circularMotion_.groundPositionY_ = {
        circularMotion_.center_.y +
        circularMotion_.radius_
    };

    circularMotion_.outGroundPositionY_ = {
        circularMotion_.groundPositionY_ +
        circularMotion_.correct_
    };
}
