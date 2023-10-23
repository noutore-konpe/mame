#include "ExperiencePoint.h"

#include "../../Taki174/OperatorXMFloat3.h"
#include "../../Taki174/FunctionXMFloat3.h"
#include "../Graphics/Graphics.h"
#include "../Scene/SceneGame.h"
#include "ExperiencePointManager.h"
#include "PlayerManager.h"
#include "../Resource/texture.h"

int ExperiencePoint::nameNum_ = 0;

ExperiencePoint::ExperiencePoint()
{
    Graphics& graphics = Graphics::Instance();

    model_ = std::make_unique<Model>(
        graphics.GetDevice(),
        "./Resources/Model/cube.fbx"
    );

    D3D11_TEXTURE2D_DESC texture2dDesc;
    ::load_texture_from_file(graphics.GetDevice(),
        L"./Resources/Image/Mask/noise3.png",
        emissiveTexture.GetAddressOf(),
        &texture2dDesc);

    ::CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/ExpModelPS.cso",
        expModelPS.GetAddressOf());

    SetName("Exp_" + std::to_string(nameNum_++));
}


ExperiencePoint::~ExperiencePoint()
{
    --nameNum_;
}


void ExperiencePoint::Initialize()
{
    GetTransform()->SetScaleFactor(0.25f);

    step_ = STEP::FALL_INIT;
}


void ExperiencePoint::Update(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    // �����X�V
    {
        ExperiencePointManager& expManager = ExperiencePointManager::Instance();
        lifeTimer_ = (std::max)(0.0f, lifeTimer_ - elapsedTime);
        if (lifeTimer_ <= 0.0f) { expManager.Remove(this); }

        // �_��
        {
            const int blinkTimer = static_cast<int>(lifeTimer_ * 100.0f);
            if (lifeTimer_ <= 4)
            {
                model_->color.w = (blinkTimer & 0x08) ? 1.0f : 0.25f;
            }
            else if (lifeTimer_ <= 8)
            {
                model_->color.w = (blinkTimer & 0x10) ? 1.0f : 0.25f;
            }
        }

        // ������^�C�}�[�X�V
        attractTimer_ = (std::max)(0.0f, attractTimer_ - elapsedTime);

    }

    PlayerManager& plManager    = PlayerManager::Instance();
    Transform*     t            = GetTransform();
    const float    elapsedFrame = elapsedTime * 60.0f;

    switch (step_)
    {
    case STEP::FALL_INIT:

        if (velocity_.y < 0.0f) velocity_.y = 0.0f; // ���łɗ������Ă����瑬�x�����Z�b�g
        isGround_       = false;                    // �󒆂ɂ���
        isMoveToPlayer_ = false;                    // �v���C���[�Ɍ������Ă��Ȃ�

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
                UpdateVerticalVelocity(elapsedFrame);
                t->AddPositionY(velocity_.y * elapsedTime);

                // �㏸���łȂ��~�^���̊J�n�n�_�܂ō~�����Ă����畂�V��ԂɈڍs����
                if (velocity_.y <= 0.0f &&
                    t->GetPositionY() <= circularMotion_.center_.y)
                {
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
        t->SetPositionY(circularMotion_.center_.y); // �~�^���̊J�n�n�_Y�Ɉʒu�ݒ�
#if 0
        {
            const float localPositionX = t->GetPositionX() - circularMotion_.center_.x;
            const float localPositionY = t->GetPositionY() - circularMotion_.center_.y;
            circularMotion_.angle_.z = ::atan2f(localPositionX, localPositionY); // return Radian Angle
        }
#else
        // ���S�̈ʒu����J�n����̂�90�x��]������ԂŎn�߂�
        circularMotion_.rotation_.z = ToRadian(90.0f);
#endif

        isGround_       = true;     // �n�ʂɂ���
        isMoveToPlayer_ = false;    // �v���C���[�Ɍ������Ă��Ȃ�

        step_ = STEP::FLOTING;
        [[fallthrough]];
    case STEP::FLOTING:

        // �v���C���[������������v���C���[�̕��֌�����
        if (true == SearchPlayer())
        {
            step_ = STEP::MOVE_TO_PLAYER_INIT;
            break;
        }

        // �󒆂ɂ����痎����ԂɈڍs����(+�����e�l)
        if (t->GetPositionY() > circularMotion_.center_.y + 0.1f)
        {
            step_ = STEP::FALL_INIT;
            break;
        }

        // XZ�ʒu�X�V
        {
            UpdateHorizontalVelocity(elapsedFrame);
            UpdateHorizontalMove(elapsedTime);
        }

        // Z����]
        {
            circularMotion_.rotation_.z += circularMotion_.addRotate_ * elapsedTime;
            if (circularMotion_.rotation_.z > 360.0f) { circularMotion_.rotation_.z -= 360.0f; }
            t->SetPositionY(
                circularMotion_.center_.y +
                ::cosf(circularMotion_.rotation_.z) *
                circularMotion_.radius_
            );
        }

        break;
    case STEP::MOVE_TO_PLAYER_INIT:

        isGround_       = false;    // �󒆂ɂ���
        isMoveToPlayer_ = true;     // �v���C���[�Ɍ������Ă��Ȃ�

        step_ = STEP::MOVE_TO_PLAYER;
        [[fallthrough]];
    case STEP::MOVE_TO_PLAYER:

        UpdateHorizontalVelocity(elapsedFrame); // ����

        // �v���C���[���痣��Ă�����ق��̏�ԂɈڂ�
        if (false == SearchPlayer())
        {
            // �ʒu���n�ʈȉ��Ȃ畂�V��ԂɈڍs����
            if (t->GetPositionY() <= circularMotion_.center_.y)
            {
                step_ = STEP::FLOTING_INIT;
                break;
            }

            // �󒆂ɂ����痎����ԂɈڍs����(+�����e�l)
            if (t->GetPositionY() > circularMotion_.center_.y + 0.1f)
            {
                step_ = STEP::FALL_INIT;
                break;
            }
        }
        else
        {
            // �v���C���[�̕��֌�����
            {
                const XMFLOAT3& pos      = GetTransform()->GetPosition();
                      XMFLOAT3  plPos    = plManager.GetPlayer()->GetTransform()->GetPosition();
                                plPos.y += plManager.GetPlayer()->GetHeight() * 0.5f;
                const XMFLOAT3  vecN     = ::XMFloat3Normalize(plPos - pos);

                velocity_ += vecN * acceleration_ * elapsedFrame; // ��(�v���C���[�Ɉˑ��ɂ���\��)
                t->AddPosition(velocity_ * elapsedTime);
            }

            // �n�ʂ��ђʂ��Ȃ��悤�ɂ���
            if (t->GetPositionY() < 0.0f)
            {
                t->SetPositionY(0.0f);
                velocity_.y = velocity_.y * 0.5f; // Y���x�𔼌�������
            }
        }

        break;
    }

    // ��]
    t->AddRotationY(::ToRadian(45.0f) * elapsedTime);

}


void ExperiencePoint::Render(const float scale, ID3D11PixelShader* psShader)
{
    Graphics::Instance().GetShader()->SetRasterizerState(
        static_cast<int>(Shader::RASTER_STATE::SOLID)
    );

    Graphics::Instance().GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    //�萔�o�b�t�@�[�X�V
    UpdateConstants();

    model_->Render(scale, expModelPS.Get());

    // �����蔻�苅�`��
#ifdef _DEBUG
    if (true == SceneGame::isDispCollision_)
    {
        DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
        debugRenderer->DrawSphere(GetTransform()->GetPosition(), radius_, { 1,1,1,1 });
    }
#endif
}


void ExperiencePoint::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(GetName().c_str()))
    {
        model_->DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

void ExperiencePoint::UpdateConstants()
{
    SetEmissiveIntensity(3.0f);
    SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));
    SetEmissiveColor(DirectX::XMFLOAT4(1.0f, 1.0f, 0.3f, 1.0f));
}


const bool ExperiencePoint::SearchPlayer()
{
    // �o���l���o�鉉�o���Ԓ��͔�΂�
    if (attractTimer_ > 0.0f) return false;

    using DirectX::XMFLOAT3;

    PlayerManager& plManager = PlayerManager::Instance();

    const XMFLOAT3& pos       = this->GetTransform()->GetPosition();
    XMFLOAT3        plPos     = plManager.GetPlayer()->GetTransform()->GetPosition();
                    plPos.y  += plManager.GetPlayer()->GetHeight() * 0.5f;
    const XMFLOAT3  vec       = plPos - pos;
    const float     lengthSq  = ::XMFloat3LengthSq(vec);

    constexpr float plCollectExpLengthSq = 6.0f * 6.0f; // ��(�v���C���[�Ɉˑ�����\��)
    if (lengthSq > plCollectExpLengthSq) return false;

    return true;

}


void ExperiencePoint::UpdateVerticalVelocity(const float elapsedFrame)
{
    velocity_.y += gravity_ * elapsedFrame;
    constexpr float velocityY_min = -1.0f;
    if (velocity_.y < velocityY_min) velocity_.y = velocityY_min;
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

        // �v���C���[�Ɍ������Ă���ꍇ�͖��C�����߂�
        // (�v���C���[�̎�������邮�邵�����ĉ��S�͂Ńv���C���[�̎��W�����O�ɏo�Ă��܂��ĉ����ɐ������ł����Ȃ��悤�ɂ��邽��)
        if (true == isMoveToPlayer_) friction = 0.2f * elapsedFrame;

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