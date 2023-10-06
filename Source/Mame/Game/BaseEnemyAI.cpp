#include "BaseEnemyAI.h"

#include "../../Taki174/Common.h"
#include "EnemyManager.h"

BaseEnemyAI::BaseEnemyAI()
{
    // EnemyManager��ProjectileManager�ɃA�N�Z�X�ł���悤�Ƀ|�C���^�������Ă���
    projectileManager_ = EnemyManager::Instance().GetProjectileManager();
}

void BaseEnemyAI::Initialize()
{
    Character::Initialize();
}

void BaseEnemyAI::Update(const float& elapsedTime)
{
    using std::make_unique;

    Character::Update(elapsedTime);

    // �m�[�h�X�V
    UpdateNode(elapsedTime);

    // ���͏����X�V
    UpdateVelocity(elapsedTime);

    //// ���G���ԍX�V
    //UpdateInvincibleTimer(elapsedTime);

    // �A�j���[�V�����X�V
    Character::UpdateAnimation(elapsedTime);

}

void BaseEnemyAI::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);
}

// ImGui�p
void BaseEnemyAI::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        std::string str = (activeNode_ != nullptr)
                        ? activeNode_->GetName()
                        : u8"�Ȃ�";
        ImGui::Text(u8"Behavior�F%s", str.c_str());

        ImGui::InputFloat("RunTimer", &runTimer_);

        Character::DrawDebug();

        //float r = GetRange();
        //ImGui::DragFloat("range", &r);
        //SetRange(r);

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}


void BaseEnemyAI::Move(
    const float vx,
    const float vz,
    const float moveSpeed)
{
    moveVec_.x = vx;
    moveVec_.z = vz;
    maxMoveSpeed_ = moveSpeed;
}


void BaseEnemyAI::Turn(
    const float elapsedTime,
    float vx,
    float vz,
    float turnSpeed /*Degree*/)
{
    if (vx == 0.0f && vz == 0.0f) return;

    using DirectX::XMFLOAT3;
    using DirectX::XMFLOAT4;

    const float length = sqrtf(vx * vx + vz * vz);
    if (length < 0.001f) return;

    vx /= length;
    vz /= length;

    XMFLOAT4 rotation = GetTransform()->GetRotation();
    const float frontX = ::sinf(rotation.y);
    const float frontZ = ::cosf(rotation.y);

    const float dot = (frontX * vx) + (frontZ * vz);

    // ��]�p�������ȏꍇ�͉�]���s��Ȃ�
    const float angle = ::acosf(dot); // ���W�A����Βl
    if (::fabsf(angle) <= 0.01f) return;

    float rot = 1.0f - dot;
    turnSpeed = ToRadian(turnSpeed) * elapsedTime;
    if (rot > turnSpeed) rot = turnSpeed;

    //���E����̂��߂̊O��
    const float cross = (frontZ * vx) - (frontX * vz);

    rotation.y += (cross < 0.0f) ? -rot : rot;

    GetTransform()->SetRotation(rotation);
}


void BaseEnemyAI::UpdateNode(const float elapsedTime)
{
    // ���ݎ��s����m�[�h������΁A�r�w�C�r�A�c���[����m�[�h�����s
    if (activeNode_ != nullptr)
    {
        activeNode_ = behaviorTree_->Run(
            activeNode_,
            behaviorData_.get(),
            elapsedTime
        );
    }
    // ���ݎ��s����Ă���m�[�h���Ȃ���΁A���Ɏ��s����m�[�h�𐄘_����
    else
    {
        activeNode_ = behaviorTree_->ActiveNodeInference(
            behaviorData_.get()
        );
    }

}


void BaseEnemyAI::UpdateVelocity(const float elapsedTime)
{
    // �o�߃t���[��
    const float elapsedFrame = elapsedTime * 60.0f;

    UpdateVerticalVelocity(elapsedFrame);
    UpdateVerticalMove(elapsedTime);

    UpdateHorizontalVelocity(elapsedFrame);
    UpdateHorizontalMove(elapsedTime);
}



void BaseEnemyAI::UpdateVerticalVelocity(const float elapsedFrame)
{
    velocity_.y += gravity_ * elapsedFrame;
}


void BaseEnemyAI::UpdateVerticalMove(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    XMFLOAT3 position = GetPosition();

    // ������
    const float velocityY = velocity_.y * elapsedTime;
    if (velocityY < 0.0f)
    {
        // �n�ʂɐڒn���Ă���
        constexpr float groundPos = 0.0f;
        if (position.y <= groundPos)
        {
            position.y = groundPos;

            // ���n����
            if (!isGround_)
            {
                OnLanding();
            }
            isGround_ = true;
            velocity_.y = 0.0f;
        }
        // �󒆂ɕ����Ă���
        else
        {
            position.y += velocityY;
            isGround_ = false;
        }
    }
    // �㏸��
    else if (velocityY > 0.0f)
    {
        position.y += velocityY;
        isGround_ = false;
    }

    SetPosition(position);
}


void BaseEnemyAI::UpdateHorizontalVelocity(const float elapsedFrame)
{
    const float oldLength = ::sqrtf(
        (velocity_.x * velocity_.x) +
        (velocity_.z * velocity_.z)
    );

    // XZ���ʂ̑��͂���������
    if (oldLength > 0.0f)
    {
        // ���C��
        float friction = friction_ * elapsedFrame;

        // �󒆂ɂ���Ƃ��͖��C�͂����炷
        if (!isGround_) friction *= airControl_;

        // ���C�ɂ�鉡�����̌�������
        if (oldLength > friction)
        {
            // �P�ʃx�N�g����
            const float vx = velocity_.x / oldLength;
            const float vz = velocity_.z / oldLength;

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

    // XZ���ʂ̑��͂���������
    if (oldLength <= maxMoveSpeed_)
    {
#if 0
        const float moveVecLength = sqrtf(
            moveVec_.x * moveVec_.x +
            moveVec_.z * moveVec_.z
        );
        // �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        if (moveVecLength > 0.0f)
#else
        const float moveVecLengthSq = {
            moveVec_.x * moveVec_.x +
            moveVec_.z * moveVec_.z
        };
        // �ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
        if (moveVecLengthSq > 0.0f)
#endif
        {
            // ������
            float acceleration = acceleration_ * elapsedFrame;
            // �󒆂ɂ���Ƃ��͉����͂����炷
            if (!isGround_) acceleration *= airControl_;

            // �ړ��x�N�g���ɂ���������
            velocity_.x += moveVec_.x * acceleration;
            velocity_.z += moveVec_.z * acceleration;

            // �ő呬�x����
            float length = ::sqrtf(
                velocity_.x * velocity_.x +
                velocity_.z * velocity_.z
            );
            if (length > maxMoveSpeed_)
            {
                float vx = velocity_.x / length;
                float vz = velocity_.z / length;

                velocity_.x = vx * maxMoveSpeed_;
                velocity_.z = vz * maxMoveSpeed_;
            }
        }
    }
    // �ړ��x�N�g�������Z�b�g
    moveVec_.x = 0.0f;
    moveVec_.z = 0.0f;

}

// #define USE_RAYCAST
void BaseEnemyAI::UpdateHorizontalMove(const float elapsedTime)
{
    using DirectX::XMFLOAT3;

    XMFLOAT3 position = GetPosition();

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
        position.x += velocity_.x * elapsedTime;
        position.z += velocity_.z * elapsedTime;

#endif
    }

    SetPosition(position);
}
#undef USE_RAYCAST


// �ړI�n�_�ֈړ�
void BaseEnemyAI::MoveToTarget(
    const float elapsedTime,
    const float speedRate,
    const bool isLookAtTarget)
{
    using DirectX::XMFLOAT3;

    // �^�[�Q�b�g�����ւ̐i�s�x�N�g�����Z�o
    const XMFLOAT3 position = GetTransform()->GetPosition();
    float vx = targetPosition_.x - position.x;
    float vz = targetPosition_.z - position.z;
    const float dist = sqrtf(vx * vx + vz * vz);
    vx /= dist;
    vz /= dist;

    // �ړ�����
    Move(vx, vz, moveSpeed_ * speedRate);
    if (true == isLookAtTarget)
    {
        Turn(elapsedTime, vx, vz, turnSpeed_ * speedRate);
    }
}
