#include "Player.h"

#include "../Graphics/Graphics.h"
#include "../Other/Easing.h"
#include "../Other/MathHelper.h"

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
            //"./Resources/Model/Character/Player/sotai.fbx");
            //"./Resources/Model/Character/Player/P_Chara.fbx");
            "./Resources/Model/Character/Player/P_Motion.fbx");

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
    //Character::PlayAnimation(2, true);
    

    //�J�������v���C���[��ǂ�������悤�ݒ�
    Camera::Instance().SetTraget(GetTransform());

    stateMachine = std::make_unique<StateMachine<State<Player>>>();

    stateMachine->RegisterState(new PlayerState::NormalState(this));
    stateMachine->RegisterState(new PlayerState::JabAttackState(this));
    stateMachine->RegisterState(new PlayerState::AvoidState(this));
    stateMachine->RegisterState(new PlayerState::DieState(this));

    stateMachine->SetState(STATE::NORMAL);

    GetTransform()->SetScaleFactor(0.7f);

    //-------------------------�X�L�����֌W����p�����[�^�̏�����--------------------------------
    level = 1;//���x��
    curExp = 0;//�o���l
    totalExp = 0;//���v�o���l
    levelUpExp = 100;//���x���A�b�v�ɕK�v�Ȍo���l
    isSelectingSkill = false;//�X�L���̑I�����o�����ǂ����̃t���O

    maxSpeed = 4.2f;
    maxDodgeSpeed = 6.0f;
    baseAttackPower = 10.0f;
    attackSpeed = 1.0f;

    deceleration = 7.0f;
    acceleration = InitAcceleration;

    //���񎞂̂݃X�L���̐����Ɣz��}��������
    if (drainSkill == nullptr)
    {
        //�h���C��
        drainSkill = std::make_unique<PlayerSkill::Drain>(this);
        skillArray.emplace_back(drainSkill.get());
        //�ړ����x�A�b�v
        moveSpeedUpSkill = std::make_unique<PlayerSkill::MoveSpeedUp>(this);
        skillArray.emplace_back(moveSpeedUpSkill.get());
        //�U���̓A�b�v
        attackPowerUpSkill = std::make_unique<PlayerSkill::AttackPowerUp>(this);
        skillArray.emplace_back(attackPowerUpSkill.get());
        //�U�����x�A�b�v
        attackSpeedUpSkill = std::make_unique<PlayerSkill::AttackSpeedUp>(this);
        skillArray.emplace_back(attackSpeedUpSkill.get());
        //�{�̐�����
        bookIncreaseSkill = std::make_unique<PlayerSkill::BookIncrease>(this);
        skillArray.emplace_back(bookIncreaseSkill.get());
    }
    for (auto& skill : skillArray)
    {
        skill->Initialize();
    }
    //------------------------------------------------------------------------------------------
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

    stateMachine->Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    //MoveUpdate(elapsedTime);

    CameraControllerUpdate(elapsedTime);

    for (auto& skill : skillArray)
    {
        skill->Update(elapsedTime);
    }
}

// Update�̌�ɌĂ΂��
void Player::End()
{
}

void Player::MoveUpdate(float elapsedTime,float ax,float ay)
{
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
    if (ax || ay)
    {
        auto forward = cTransform->CalcForward();
        auto right = cTransform->CalcRight();
        forward.x *= ay;
        forward.z *= ay;
        right.x *= ax;
        right.z *= ax;

        moveVec = { right.x + forward.x,0,right.z + forward.z };
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.x /= length;
        moveVec.z /= length;
    }

    UpdateVelocity(elapsedTime,ax,ay);
#endif // 0
    
    //GetTransform()->SetPosition(pos);
    DirectX::XMFLOAT3 move = {
        velocity.x * elapsedTime,
        velocity.y * elapsedTime,
        velocity.z * elapsedTime
    };
    GetTransform()->AddPosition(move);

    Turn(elapsedTime,moveVec.x, moveVec.z,360.0f);
}

void Player::MoveVecUpdate(float ax,float ay)
{
    if (ax || ay)
    {
        auto cTransform = Camera::Instance().GetTransform();

        auto forward = cTransform->CalcForward();
        auto right = cTransform->CalcRight();
        forward.x *= ay;
        forward.z *= ay;
        right.x *= ax;
        right.z *= ax;

        moveVec = { right.x + forward.x,0,right.z + forward.z };
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.y * moveVec.y);
        moveVec.x /= length;
        moveVec.z /= length;
    }
}

void Player::UpdateVelocity(float elapsedTime,float ax,float ay)
{
    //GamePad& gamePad = Input::Instance().GetGamePad();

    /*float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();*/

    float length{ sqrtf(velocity.x * velocity.x + velocity.z * velocity.z) };

    //�A�j���[�V�����̏d�݂̕ύX
    model->weight = length / maxSpeed;

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

        ////�_�b�V��
        //if (InputDash())
        //{
        //    acceleration = this->acceleration * elapsedTime;
        //    maxSpeed = maxDashSpeed;
        //}
        //else
        //{
        //    acceleration = this->acceleration * elapsedTime;
        //    maxSpeed = this->maxSpeed;
        //}

        acceleration = this->acceleration * elapsedTime;

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

void Player::AvoidUpdate(float elapsedTime)
{
    auto front = GetTransform()->CalcForward();

    float acceleration = this->acceleration * elapsedTime;

    //�ړ��x�N�g���ɂ���������
    velocity.x += front.x * acceleration;
    velocity.z += front.z * acceleration;

    //�ő呬�x����
    float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
    if (length > maxDodgeSpeed)
    {
        DirectX::XMVECTOR vec = { moveVec.x,moveVec.z };
        vec = DirectX::XMVector2Normalize(vec);
        DirectX::XMVECTOR velo = DirectX::XMVectorScale(vec, maxDodgeSpeed);
        velocity.x = DirectX::XMVectorGetX(velo);
        velocity.z = DirectX::XMVectorGetY(velo);

        length = maxDodgeSpeed;
    }

    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    MoveVecUpdate(ax,ay);
    Turn(elapsedTime, moveVec.x, moveVec.z, 360.0f);

    DirectX::XMFLOAT3 velo = {
        velocity.x * elapsedTime,
        velocity.y * elapsedTime,
        velocity.z * elapsedTime
    };
    GetTransform()->AddPosition(velo);
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
        rotX = std::clamp(rotX,-0.35f,0.8f);
        cTransform->SetRotationX(rotX);
    }

    //cTransform->SetRotationX(DirectX::XMConvertToRadians(15.0f));

    Camera::Instance().ScreenVibrationUpdate(elapsedTime);
}

// �`�揈��
void Player::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);

    /*for (auto& skill : skillArray)
    {
        skill->Render();
    }*/
}

void Player::SkillImagesRender()
{
    int i = 0;
    for (auto& skill : skillArray)
    {
        if (skill->GetOverlapNum() == 0)continue;
        //skill->SetIconPos();
        skill->Render();
        i++;
    }
}

// ImGui�p
void Player::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        stateMachine->DrawDebug();

        if (ImGui::TreeNode("Camera"))
        {
            ImGui::SliderFloat("RotSpeed", &cameraRotSpeed, 0.01f, 5.0f);

            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Movement"))
        {
            ImGui::SliderFloat("MaxMoveSpeed", &maxSpeed, 0.1f, 10.0f);

            ImGui::TreePop();
        }

        ImGui::Begin("Skill");

        for (auto& skill : skillArray)
        {
            skill->DrawDebug();
        }

        ImGui::End();

        if (ImGui::Button("Initialize"))
        {
            Initialize();
        }

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

void Player::AttackSteppingUpdate(float elapsedTime)
{
    //�^�C�}�[�̃��Z�b�g�̓X�e�[�g�}�V���̕��ōs��
    if (steppingTimer > steppingTime)return;

    //���x�Z�o
    float maxSpeed = Easing::OutQuart(steppingTimer,steppingTime,steppingSpeed,0.0f);
    auto front = GetTransform()->CalcForward();
    velocity = front * maxSpeed;

    //�ړ�����
    DirectX::XMFLOAT3 move = velocity * elapsedTime;
    GetTransform()->AddPosition(move);

    //��]����
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    MoveVecUpdate(ax,ay);
    Turn(elapsedTime, moveVec.x, moveVec.z, 360.0f);

    steppingTimer += elapsedTime;
}

void Player::LevelUpdate()
{
    if(curExp > levelUpExp)
    {
        level++;
        curExp -= levelUpExp;

        //���x�����オ��Ɣ\�͂��擾�o����
        isSelectingSkill = true;
    }
}
