#include "Player.h"

#include "../Graphics/Graphics.h"


#include "AbilityManager.h"
#include "PlayerState.h"

#include <algorithm>

// �R���X�g���N�^
Player::Player()
{
    Graphics& graphics = Graphics::Instance();

    // ���f������
    {
        model = std::make_unique<Model>(graphics.GetDevice(),
            //"./Resources/Model/testModel/character.fbx");
            //"./Resources/Model/P_Chara.fbx");
            "./Resources/Model/P_Motion.fbx");
            //"./Resources/Model/sotai.fbx");
            //"./Resources/Model/sanaModel/mameoall.fbx");
            //"./Resources/Model/testModel/nico.fbx");
    }
}

// �f�X�g���N�^
Player::~Player()
{
}

// ������
void Player::Initialize()
{
    Character::Initialize();

    // �ҋ@�A�j���[�V�����ɐݒ肵�Ă�
    Character::PlayAnimation(0, true);

    //�J�������v���C���[��ǂ�������悤�ݒ�
    Camera::Instance().SetTraget(GetTransform());

    //stateMachine.RegisterState(new PlayerState::NormalState());
}

// �I����
void Player::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void Player::Begin()
{
}

// �X�V����
void Player::Update(const float& elapsedTime)
{
    Character::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    MoveUpdate(elapsedTime);

    CameraControllerUpdate(elapsedTime);
}

// Update�̌�ɌĂ΂��
void Player::End()
{
}

void Player::MoveUpdate(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float aLx = gamePad.GetAxisLX();
    float aLy = gamePad.GetAxisLY();

    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();

    auto cTransform = Camera::Instance().GetTransform();

#if 0
    if (aLx || aLy)
    {
        auto forward = cTransform->CalcForward();
        auto right = cTransform->CalcRight();
        forward.x *= aLy;
        forward.z *= aLy;
        right.x *= aLx;
        right.z *= aLx;

        moveVec = {right.x + forward.x,0,right.z + forward.z};
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.x /= length;
        moveVec.z /= length;

        float speed = acceleration * elapsedTime;
        pos.x += speed * moveVec.x;
        pos.z += speed * moveVec.z;
    }
#else
    if (aLx || aLy)
    {
        auto forward = cTransform->CalcForward();
        auto right = cTransform->CalcRight();
        forward.x *= aLy;
        forward.z *= aLy;
        right.x *= aLx;
        right.z *= aLx;

        moveVec = { right.x + forward.x,0,right.z + forward.z };
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.x /= length;
        moveVec.z /= length;
    }

    UpdateVelocity(elapsedTime);
#endif // 0
    
    //GetTransform()->SetPosition(pos);
    DirectX::XMFLOAT3 velo = {
        velocity.x * elapsedTime,
        velocity.y * elapsedTime,
        velocity.z * elapsedTime
    };
    GetTransform()->AddPosition(velo);

    Turn(elapsedTime,moveVec.x, moveVec.z,360.0f);
}

void Player::UpdateVelocity(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();

    float length{ sqrtf(velocity.x * velocity.x + velocity.z * velocity.z) };
    if (length > 0.0f)
    {
        //���͂��Ȃ��Ȃ猸��
        if (ax == 0 && ay == 0)
        {
            //�����͌v�Z
            float deceleration = this->deceleration * elapsedTime;

            if ((length -= deceleration) > 0.0f)
            {
                DirectX::XMVECTOR Velocity{velocity.x, velocity.z};
                DirectX::XMVECTOR vecNormal{DirectX::XMVector2Normalize(Velocity)};
                Velocity = DirectX::XMVectorScale(vecNormal, length);

                velocity.x = DirectX::XMVectorGetX(Velocity);
                velocity.z = DirectX::XMVectorGetY(Velocity);

                //���������������ꍇ���������͔�΂�
                return;
            }
            else
            {
                velocity.x = 0.0f;
                velocity.z = 0.0f;

                return;
            }
        }
    }

    //���͂��Ȃ��Ȃ�������Ȃ�
    if (ax == 0 && ay == 0)return;
    //�ړ��x�N�g�����[���x�N�g���o�Ȃ��Ȃ��������
    float moveVecLength = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
    if (moveVecLength > 0.0f)
    {
        float acceleration;//������
        float maxSpeed;//�ő呬�x

        //�_�b�V��
        if (InputDash())
        {
            acceleration = this->acceleration * elapsedTime;
            maxSpeed = maxDashSpeed;
        }
        else
        {
            acceleration = this->acceleration * elapsedTime;
            maxSpeed = this->maxSpeed;
        }


        //�ړ��x�N�g���ɂ���������
        velocity.x += moveVec.x * acceleration;
        velocity.z += moveVec.z * acceleration;

        //�ő呬�x����
        float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
        if (length > maxSpeed)
        {
            DirectX::XMVECTOR vec = { moveVec.x,moveVec.z };
            vec = DirectX::XMVector2Normalize(vec);
            DirectX::XMVECTOR velo = DirectX::XMVectorScale(vec, maxSpeed);
            velocity.x = DirectX::XMVectorGetX(velo);
            velocity.z = DirectX::XMVectorGetY(velo);

            length = maxSpeed;
        }

    }
}

void Player::CameraControllerUpdate(float elapsedTime)
{
    //�J���������i���͉�]�̂݁j
    auto* cTransform = Camera::Instance().GetTransform();
    
    GamePad& gamePad = Input::Instance().GetGamePad();

    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    if (ax)
    {
        cTransform->AddRotationY(cameraRotSpeed * ax * elapsedTime);
    }
    if (ay)
    {
        cTransform->AddRotationX(cameraRotSpeed * ay * elapsedTime);
        float rotX = cTransform->GetRotation().x;
        rotX = std::clamp(rotX,-0.4f,0.8f);
        cTransform->SetRotationX(rotX);
    }
    


    //cTransform->SetRotationX(DirectX::XMConvertToRadians(15.0f));
}

// �`�揈��
void Player::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);
}

// ImGui�p
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        if (ImGui::TreeNode("Camera"))
        {
            ImGui::SliderFloat("RotSpeed", &cameraRotSpeed, 0.01f, 5.0f);

            ImGui::TreePop();
        }

        ImGui::DragFloat("MaxMoveSpeed", &maxSpeed, 0.05f, 0.1f);

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
