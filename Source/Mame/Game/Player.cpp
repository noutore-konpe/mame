#include "Player.h"

#include <algorithm>

#include "../../Taki174/Common.h"

#include "../Graphics/Graphics.h"
#include "../Other/Easing.h"
#include "../Other/MathHelper.h"
#include "../Scene/SceneGame.h"

#include "PlayerState.h"
#include "BlackHole.h"
#include "EnemyManager.h"

#include "UserInterface.h"

#include "PlayerManager.h"

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

        swordModel = std::make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/Character/Sword_Motion.fbx");
            //"./Resources/Model/Character/mdl_sword.fbx");

#if _DEBUG
        //stageDebugSphere = std::make_unique<Model>(graphics.GetDevice(),"./Resources/Model/Collision/sqhere.fbx");
#endif // _DEBUG


        // pixelShader
        CreatePsFromCso(graphics.GetDevice(),
            "./Resources/Shader/playerPS.cso",
            playerPS.GetAddressOf());
    }

    //��炢����A�U������Z�b�g
    {
        swordColliderNum = 5;
        swordColliderRadius = 0.1f;
        for (int i = 0; i < swordColliderNum; i++)
        {
            attackCollider.emplace_back(SphereCollider(swordColliderRadius));
        }

        for (int i = 0; i < static_cast<int>(HitColName::END); i++)
        {
            hitCollider.emplace_back(SphereCollider(0.15f));
        }
    }

    laserEffect = std::make_unique<Effect>("./Resources/Effect/laser.efk");
}

// �f�X�g���N�^
Player::~Player()
{
    // �A�r���e�B�}�l�[�W���[�I������
    abilityManager_.Finalize();
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
    stateMachine->RegisterState(new PlayerState::HardAttackState(this));
    stateMachine->RegisterState(new PlayerState::AvoidState(this));
    stateMachine->RegisterState(new PlayerState::DieState(this));
    stateMachine->RegisterState(new PlayerState::SoftStaggerState(this));
    stateMachine->RegisterState(new PlayerState::HardStaggerState(this));
    stateMachine->RegisterState(new PlayerState::CounterState(this));

    stateMachine->SetState(STATE::NORMAL);

    GetTransform()->SetScaleFactor(0.7f);

    //-------------------------�X�L�����֌W����p�����[�^�̏�����--------------------------------
    level = 1;//���x��
    curExp = 0;//�o���l
    totalExp = 0;//���v�o���l
    levelUpExp = 100;//���x���A�b�v�ɕK�v�Ȍo���l
    isSelectingSkill = false;//�X�L���̑I�����o�����ǂ����̃t���O

    maxEyeSpeed = 4.2f;
    maxDodgeSpeed = 6.0f;
    baseAttackPower = 10.0f;
    attackSpeed = 1.0f;

    jabMotionAtkMuls[0] = 1.0f;
    jabMotionAtkMuls[1] = 1.2f;
    jabMotionAtkMuls[2] = 2.3f;

    health = 400.0f;
    maxHealth = 400.0f;

    defence = 0.0f;


    deceleration = 7.0f;
    acceleration = InitAcceleration;

    //�X�L�����}�l�[�W���[����擾
    //------------------------------------------------------------------------------------------

    LockOnInitialize();

    swordModel->transform.SetScaleFactor(GetTransform()->GetScaleFactor());

    //�U������
    swordColliderNum = 5;
    swordColliderRadius = 0.1f;
    swordScale = 0.7f;
    swordModel->GetTransform()->SetScaleFactor(swordScale);

    //��炢����
    hitCollider[static_cast<int>(HitColName::NECK)].radius = 0.1f;
    hitCollider[static_cast<int>(HitColName::HIP)].radius = 0.12f;
    hitCollider[static_cast<int>(HitColName::R_LEG)].radius = 0.07f;
    hitCollider[static_cast<int>(HitColName::L_LEG)].radius = 0.07f;
    hitCollider[static_cast<int>(HitColName::R_LEG_END)].radius = 0.07f;
    hitCollider[static_cast<int>(HitColName::L_LEG_END)].radius = 0.07f;
    hitCollider[static_cast<int>(HitColName::R_ELBOW)].radius = 0.07f;
    hitCollider[static_cast<int>(HitColName::L_ELBOW)].radius = 0.07f;
    
    for (auto& collider : hitCollider)
    {
        collider.radius *= 1.3f;
    }
    

    lifeTimer = 0;
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
void Player::Update(const float elapsedTime)
{
    //���b�N�I�������A����
    if (InputLockOn())
    {
        Camera::Instance().activeLockOn = !Camera::Instance().activeLockOn;
        if (Camera::Instance().activeLockOn)LockOnInitialize();
    }

    if (Camera::Instance().activeLockOn)
    {
        LockOnUpdate();
    }

    Character::Update(elapsedTime);

    stateMachine->Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);

    swordModel->transform.SetPosition(GetTransform()->GetPosition());
    swordModel->transform.SetRotation(GetTransform()->GetRotation());
    swordModel->UpdateAnimation(elapsedTime);

    //MoveUpdate(elapsedTime);

    CameraControllerUpdate(elapsedTime);

    LevelUpdate();

    skillArray = &PlayerManager::Instance().GetSkillArray();

    // �A�r���e�B�}�l�[�W���[�X�V(��)
    abilityManager_.Update(elapsedTime);

    lifeTimer += elapsedTime;
}

// Update�̌�ɌĂ΂��
void Player::End()
{
}

Character::DamageResult Player::ApplyDamage(float damage, const DirectX::XMFLOAT3 hitPosition, Character* attacker, float invincibleTime)
{
    DamageResult result;

    //���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
    if (health <= 0)
    {
        result.hit = true;
        return result;
    }

    //�J�E���^�[��t���Ԓ���
    if (isCounter)
    {
        result.damage = damage;
        result.hit = true;
        counterCompleted = true;

        if (attacker)
        {
            //�U�������҂��Ђ�܂���
            attacker->Flinch();
        }
        return result;
    }

    return Character::ApplyDamage(damage, hitPosition, attacker, invincibleTime);
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
    MoveVecUpdate(ax, ay);

    UpdateVelocity(elapsedTime,ax,ay);
#endif // 0

    //GetTransform()->SetPosition(pos);
    DirectX::XMFLOAT3 move = {
        velocity.x * elapsedTime,
        velocity.y * elapsedTime,
        velocity.z * elapsedTime
    };

    ////�X�e�[�W����
    //DirectX::XMFLOAT3 collectPos;
    //float length = Length(movePos);
    //if (SceneGame::stageRadius < length)
    //{
    //    DirectX::XMFLOAT3 moveNormal = Normalize(movePos);
    //    collectPos = moveNormal * SceneGame::stageRadius;
    //}
    //else
    //{
    //    collectPos = movePos;
    //}

    DirectX::XMFLOAT3 movePos = GetTransform()->GetPosition() + move;
    GetTransform()->SetPosition(CollidedPosition(movePos));

    Turn(elapsedTime,moveVec.x, moveVec.z,360.0f);
}

void Player::MoveVecUpdate(float ax,float ay)
{
    if (ax || ay)
    {
        //auto cTransform = Camera::Instance().GetTransform();

        auto forward = Camera::Instance().GetForward();
        auto right = Camera::Instance().GetRight();
        forward.x *= ay;
        forward.z *= ay;
        right.x *= ax;
        right.z *= ax;

        moveVec = { right.x + forward.x,0,right.z + forward.z };
        float length = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
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
    //model->weight = length / maxSpeed;
    model->weight = (std::min)(1.0f, length / maxEyeSpeed);
    swordModel->weight = model->weight;


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
        if (length > maxEyeSpeed)
        {
            DirectX::XMVECTOR vec = { moveVec.x,moveVec.z };
            vec = DirectX::XMVector2Normalize(vec);
            DirectX::XMVECTOR velo = DirectX::XMVectorScale(vec, maxEyeSpeed);
            velocity.x = DirectX::XMVectorGetX(velo);
            velocity.z = DirectX::XMVectorGetY(velo);

            length = maxEyeSpeed;
        }
    }
}


void Player::AvoidUpdate(float elapsedTime)
{
    auto front = GetTransform()->CalcForward();

    float acceleration = this->dodgeAcceleration * elapsedTime;

    //�ړ��x�N�g���ɂ���������
    velocity.x += front.x * acceleration;
    velocity.z += front.z * acceleration;

    //�ő呬�x����
    float maxDodgeSpeed = this->maxDodgeSpeed + Easing::OutSine(
        static_cast<float>(model->GetCurrentKeyframeIndex()),
        static_cast<float>(model->GetCurrentKeyframeMaxIndex()/2.0f),
        2.0f, 0.0f);
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
    Turn(elapsedTime, moveVec.x, moveVec.z, 180.0f);

    DirectX::XMFLOAT3 move = {
        velocity.x * elapsedTime,
        velocity.y * elapsedTime,
        velocity.z * elapsedTime
    };
    DirectX::XMFLOAT3 movePos = GetTransform()->GetPosition() + move;
    GetTransform()->SetPosition(CollidedPosition(movePos));
}

void Player::ModelRotZUpdate(float elapsedTime)
{
    static float lastRotValue = 0;

    /*float rotSpeed = elapsedTime * 10.0f * actualRotValue - rotValue;
    if (actualRotValue - rotValue < 0.01f && actualRotValue - rotValue > -0.01f)
    {
        actualRotValue = 0;
    }
    if (actualRotValue < rotValue)
    {
        actualRotValue += rotSpeed;
    }
    else if (actualRotValue > rotValue)
    {
        actualRotValue -= rotSpeed;
    }*/

    if (rotTimer > 0.1f)
    {
        lastRotValue = GetTransform()->GetRotation().z;
        rotTimer = 0;
    }
    //float rotZ = Easing::InSine(rotTimer,0.1f,-rotValue * 5,lastRotValue);
    float rotZ = Easing::InSine(std::fabsf(rotValue), 0.1f, 0.5f, 0.0f);
    rotZ = rotValue < 0 ? rotZ : -rotZ;
    GetTransform()->SetRotationZ(rotZ);

    //GetTransform()->SetRotationZ(actualRotValue);

    rotTimer += elapsedTime;
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
        cTransform->AddRotationX(cameraRotSpeed * -ay * elapsedTime);
        float rotX = cTransform->GetRotation().x;
        rotX = std::clamp(rotX,-0.10f,0.8f);
        cTransform->SetRotationX(rotX);
    }

    //cTransform->SetRotationX(DirectX::XMConvertToRadians(15.0f));

    Camera::Instance().ScreenVibrationUpdate(elapsedTime);
}

// �`�揈��
void Player::Render(const float scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, playerPS.Get());

    swordModel->Render(scale, playerPS.Get());

    /*for (auto& skill : skillArray)
    {
        skill->Render();
    }*/

#if _DEBUG
    //stageDebugSphere->Render(stageRadius, playerPS.Get());
    if (SceneGame::isDispCollision_)
    {
        for (auto& collider : hitCollider)
        {
            collider.DebugRender();
        }

        for (auto& collider : attackCollider)
        {
            collider.DebugRender(DirectX::XMFLOAT4(1,0,0,1));
        }
    }
    
#endif // _DEBUG


    // �A�r���e�B�}�l�[�W���[�`��(��)
    abilityManager_.Render(scale);

    //����ړ�
    ColliderPosUpdate(scale);
}

// ImGui�p
void Player::DrawDebug()
{
#ifdef USE_IMGUI


    static float damage = 0;
    if (ImGui::BeginMenu("player"))
    {
        Character::DrawDebug();

        stateMachine->DrawDebug();
        

        //ImGui::Checkbox("ShoeCollider",&showCollider);

        if (ImGui::TreeNode("Camera"))
        {
            ImGui::SliderFloat("RotSpeed", &cameraRotSpeed, 0.01f, 5.0f);

            if (ImGui::Button("R Change LockOn Target"))
            {
                ChangeLockOnTarget(1.0f);
            }

            if (ImGui::Button("L Change LockOn Target"))
            {
                ChangeLockOnTarget(-1.0f);
            }

            ImGui::TreePop();
        }

        if(ImGui::TreeNode("Movement"))
        {
            ImGui::SliderFloat("MaxMoveSpeed", &maxEyeSpeed, 0.1f, 10.0f);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Damage"))
        {
            ImGui::SliderFloat("Damage", &damage,0.0f,20.0f);
            if (ImGui::Button("Apply Damage"))
            {
                ApplyDamage(damage,GetTransform()->GetPosition(), nullptr);
            }

            ImGui::TreePop();
        }

        ImGui::Begin("Skill");
        ImGui::InputFloat("Exp", &curExp);
        if (ImGui::Button("Exp + 100"))
        {
            curExp += 100;
        }

        if (ImGui::Button("Skip"))
        {
            isSelectingSkill = false;
        }

        

        ImGui::End();

        if (ImGui::Button("Initialize"))
        {
            Initialize();
        }

#if _DEBUG
        //ImGui::DragFloat("StageRadius",&stageRadius);
#endif // _DEBUG

        // �u���b�N�z�[������
        if (ImGui::Button("Create BlackHole"))
        {
            BlackHole* blackHole = new BlackHole(&abilityManager_);
            blackHole->Initialize();
            abilityManager_.Register(blackHole);
        }

        // �A�r���e�B�}�l�[�W���[�f�o�b�O�`��(��)
        abilityManager_.DrawDebug();

        laserEffect->DrawDebug();

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

void Player::AttackSteppingUpdate(float elapsedTime)
{
    //�^�C�}�[�̃��Z�b�g�̓X�e�[�g�}�V���̕��ōs��
    if (steppingTimer > steppingTime)return;

    //���x�Z�o
    float maxEyeSpeed = Easing::OutQuart(steppingTimer,steppingTime,steppingSpeed,0.0f);
    auto front = GetTransform()->CalcForward();
    velocity = front * maxEyeSpeed;

    //�ړ�����
    DirectX::XMFLOAT3 movePos = velocity * elapsedTime + GetTransform()->GetPosition();
    GetTransform()->SetPosition(CollidedPosition(movePos));

    //��]����
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisLX();
    float ay = gamePad.GetAxisLY();
    MoveVecUpdate(ax,ay);
    Turn(elapsedTime, moveVec.x, moveVec.z, 360.0f);

    steppingTimer += elapsedTime;
}

void Player::SelectSkillUpdate(float elapsedTime)
{
    static float _timer;
    switch (drawDirectionState)
    {
    case 0:
        DrawCards();
        selectCard = 0;

        for (int i = 0; i < 3; i++)
        {
            drawingSkillCards[i]->card->GetSpriteDissolve()->SetDissolveValue(-0.1f);
        }

        drawDirectionState++;
        break;

    case 1:
        for (int i = 0; i < 3; i++)
        {
            auto* transform = drawingSkillCards[i]->card->GetSpriteTransform();
            float timer = drawDirectionTimer - i * 0.1f;
            if (timer < 0)timer = 0;
            else if (timer > drawDirectionTime)timer = drawDirectionTime;
            float posY = Easing::OutSine(timer, drawDirectionTime, 165.0f, -transform->GetTexSize().y);
            transform->SetPos(DirectX::XMFLOAT2(65.0f + 400.0f * static_cast<float>(i), posY));

        }

        if (drawDirectionTimer >= 1.0f)
        {
            drawDirectionState++;
        }
        break;
    case 2:
    {
        //�J�[�h�I��
        static bool buttonDown = false;
        auto ax = Input::Instance().GetGamePad().GetAxisLX();
        if (ax >= 0.5f && !buttonDown)
        {
            selectCard++;
            if (selectCard > 2)
            {
                selectCard = 2;
            }
            buttonDown = true;
        }
        else if (ax <= -0.5f && !buttonDown)
        {
            selectCard--;
            if (selectCard < 0)
            {
                selectCard = 0;
            }
            buttonDown = true;
        }
        else if (ax >= 0.05f || ax <= -0.05f)buttonDown = true;
        else
        {
            buttonDown = false;
        }
    }

        //�J�[�h��I��
        if (Player::InputDecide())
        {
            _timer = 0;
            drawingSkillCards[selectCard]->Overlaping();
            drawDirectionState++;
        }
        break;

    case 3:
        if (drawingSkillCards[selectCard]->card->GetSpriteDissolve()->GetDissolveValue() < 2.0f)
        {
            drawingSkillCards[selectCard]->card->GetSpriteDissolve()->AddDissolveValue(elapsedTime);
        }

        for (int i = 0; i < 3; i++)
        {
            if (selectCard == i)continue;
            auto* transform = drawingSkillCards[i]->card->GetSpriteTransform();
            float timer = _timer;
            if (timer > 0.5)timer = 0.5;
            float posY = Easing::OutSine(timer, 0.5f, -transform->GetTexSize().y,165.0f);
            transform->SetPos(DirectX::XMFLOAT2(65.0f + 400.0f * static_cast<float>(i), posY));
        }

        if (_timer > 2.2f)isSelectingSkill = false;

        _timer += elapsedTime;
        break;
    }

    for (int i = 0; i < 3; i++)
    {
        drawingSkillCards[i]->card->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
    }
    drawingSkillCards[selectCard]->card->GetSpriteTransform()->SetColor(DirectX::XMFLOAT4(1,1,1,1));

    drawDirectionTimer += elapsedTime;
}

BaseSkill* Player::Lottery()
{
    std::vector<BaseSkill*> lotSkills;
    while (true)
    {
        //�m���̕�����Z�o
        int denominator =
            BaseSkill::pCommon +
            BaseSkill::pUncommon +
            BaseSkill::pRare +
            BaseSkill::pSuperRare +
            BaseSkill::pUltraRare;
        int rarity = rand() % denominator;
        int checker = BaseSkill::pCommon;
        if (rarity < checker)rarity = BaseSkill::COMMON;
        else if (rarity < (checker += BaseSkill::pUncommon))rarity = BaseSkill::UNCOMMON;
        else if (rarity < (checker += BaseSkill::pRare))rarity = BaseSkill::RARE;
        else if (rarity < (checker += BaseSkill::pSuperRare))rarity = BaseSkill::SUPER_RARE;
        else if (rarity < (checker += BaseSkill::pUncommon))rarity = BaseSkill::ULTRA_RARE;

        //���I�Ώۂ̃��A�x�̃X�L�����i�[
        for (auto& skill : *skillArray)
        {
            //���łɈ����ꂽ�X�L���̓X�L�b�v
            if (skill->isSelect)continue;
            if (skill->isOneSheet)continue;

            if (skill->rarity == rarity)
            {
                lotSkills.emplace_back(skill);
            }
        }
        if (lotSkills.size() > 0)break;
        else lotSkills.clear();
    }

    auto* drawSkill = lotSkills.at(rand() % lotSkills.size());
    drawSkill->isSelect = true;
    return drawSkill;

}

void Player::DrawCards()
{
    for (auto& skill : *skillArray)
    {
        skill->isSelect = false;
    }

    for (int i = 0; i < 3; i++)
    {
        drawingSkillCards[i] = Lottery();
    }
    drawDirectionTimer = 0;
}

bool Player::ChangeLockOnTarget(float ax)
{
    if (!(ax > 0.5f || ax < -0.5))return false;
    auto& camera = Camera::Instance();
    DirectX::XMFLOAT3 curLockOnTargetPos = camera.GetLockOnTarget()->GetPosition();

    //�O�ςɃv���C���[����Б��ɂ���G�̂ݎ擾
    std::vector<Enemy*> sideEnemys;
    auto& enemyManager = EnemyManager::Instance();

    DirectX::XMFLOAT3 playerPos = GetTransform()->GetPosition();
    DirectX::XMFLOAT3 plVec = curLockOnTargetPos - playerPos;//�v���C���[���烍�b�N�I����̃^�[�Q�b�g�܂ł̃x�N�g��
    //���K��
    const float length = sqrtf(plVec.x * plVec.x + plVec.z * plVec.z);
    plVec.x /= length;
    plVec.z /= length;

    for (size_t i = 0; i < enemyManager.GetEnemyCount(); i++)
    {
        auto* enemy = enemyManager.GetEnemy(i);

        if (enemy->isDead)continue;//����ł���Ȃ珈�����΂�

        DirectX::XMFLOAT3 enemyPos = enemy->GetTransform()->GetPosition();

        DirectX::XMFLOAT3 peVec = enemyPos - playerPos;//�v���C���[����G�܂ł̃x�N�g��
        //���K��
        const float length = sqrtf(peVec.x * peVec.x + peVec.z * peVec.z);
        peVec.x /= length;
        peVec.z /= length;

        //�O�ϒl����v���C���[���猩�ēG���ǂ��瑤�ɂ��Ă��邩���ׂ�(���̒l�Ȃ�E���ɓG������)
        //�����̃X�e�B�b�N���͕����ɍ��킹�Ăǂ��瑤�̓G���擾���邩���f����
        float crossY = (plVec.x * peVec.z) - (plVec.z * peVec.x);
        if (ax > 0) //�E��
        {
            if (crossY < 0)sideEnemys.emplace_back(enemy);
        }
        else if (ax < 0)//����
        {
            if (crossY > 0)sideEnemys.emplace_back(enemy);
        }
    }

    //�C�ӂ̕����ɓG�����Ȃ��Ȃ烍�b�N�I����؂�ւ��Ȃ�
    if (sideEnemys.size() == 0)return false;

    //��݂̂̂Ȃ炻�̓G�����b�N�I��
    if (sideEnemys.size() == 1)
    {
        camera.SetLockOnTarget(sideEnemys.at(0));
        UserInterface::Instance().SetLockOnInitialize();
        return true;
    }

    //�����̓G���C�ӂ̕����ɂ����ꍇ�A�����Ƃ����ϒl�̒Ⴂ�G�Ƀ��b�N�I��
    Enemy* nextLockOnTarget = sideEnemys.at(0);
    float dot0 = -1;
    for (int i = 1; i < sideEnemys.size(); i++)
    {
        DirectX::XMFLOAT3 peVec = sideEnemys.at(i)->GetPosition() - playerPos;
        //���K��
        const float length = sqrtf(peVec.x * peVec.x + peVec.z * peVec.z);
        peVec.x /= length;
        peVec.z /= length;
        float dot1 = (plVec.x * peVec.x) + (plVec.z * peVec.z);

        if (dot0 < dot1)
        {
            dot0 = dot1;
            nextLockOnTarget = sideEnemys.at(i);
        }
    }

    camera.SetLockOnTarget(nextLockOnTarget);
    UserInterface::Instance().SetLockOnInitialize();

    return true;
}

void Player::LockOnUpdate()
{
    if (Camera::Instance().GetLockOnTarget() == nullptr)
    {
        LockOnInitialize();
        return;
    }

    //���b�N�I����̃G�l�~�[�����񂾂Ƃ����E�̓G�Ƀ��b�N�I���؂�ւ���
    // �G�����Ȃ������ꍇ�A���b�N�I������������
    if (Camera::Instance().GetLockOnTarget()->isDead)
    {
        if (ChangeLockOnTarget(1))return;
        else if (ChangeLockOnTarget(-1))return;

        Camera::Instance().activeLockOn = false;
        return;
    }

    float ax = Input::Instance().GetGamePad().GetAxisRX();

    static bool buttonDown = false;
    if (!buttonDown)
    {
        if(ChangeLockOnTarget(ax))buttonDown = true;
    }
    if (ax <= 0.1f && ax >= -0.1f)
    {
        buttonDown = false;
    }
}

void Player::LockOnInitialize()
{
    using DirectX::XMFLOAT3;

    if (EnemyManager::Instance().GetEnemyCount() == 0)return;
    float length0 = FLT_MAX;
    for (auto& enemy : EnemyManager::Instance().GetEnemies())
    {
        const XMFLOAT3 ePos = enemy->GetTransform()->GetPosition();
        float length1 = Length(ePos - GetTransform()->GetPosition());
        if (length0 > length1)
        {
            Camera::Instance().SetLockOnTarget(enemy);
            UserInterface::Instance().SetLockOnInitialize();
            length0 = length1;
        }
    }
}

void Player::BlownUpdate(float elapsedTime)
{
    if (blowTimer < 0)
    {
        velocity = DirectX::XMFLOAT3(0, 0, 0);
        return;
    }

    float blowSpeed = Easing::OutCubic(blowTimer, blowTime, this->blowSpeed, 0.0f);
    velocity = blowVec * blowSpeed;

    DirectX::XMFLOAT3 movePos = GetTransform()->GetPosition() + velocity * elapsedTime;
    GetTransform()->SetPosition(CollidedPosition(movePos));

    blowTimer -= elapsedTime;
}

void Player::Blow(DirectX::XMFLOAT3 blowVec)
{
    this->blowVec = Normalize(blowVec);
    blowTimer = blowTime;
}

void Player::ActiveCounter()
{
    if (InputCounter())
    {
        ChangeState(static_cast<float>(STATE::COUNTER));
    }
}

void Player::PlayLaserEffect()
{
    DirectX::XMFLOAT3 pos = GetTransform()->GetPosition();
    auto forward = GetTransform()->CalcForward();
    pos = pos + forward * 0.5f;
    pos.y += 0.7f;
    laserEffect->SetPosition(pos);
    auto r = GetTransform()->GetRotation();
    //laserEffect->SetRotate(DirectX::XMFLOAT3(r.x,r.y,r.z));
    laserEffect->angle = r.y - DirectX::XMConvertToRadians(180);

    laserEffect->Play(pos);
}

void Player::OnDamaged()
{
    //stateMachine->ChangeState(STAGGER_SOFT);

    
}

void Player::OnDead(DamageResult result)
{
    stateMachine->ChangeState(DIE);

    Blow(result.hitVector);

    float length = sqrtf(result.hitVector.x * result.hitVector.x + result.hitVector.z * result.hitVector.z);
    result.hitVector.z /= length;
    GetTransform()->SetRotationY(acosf(result.hitVector.z));

    PlayerManager::Instance().SetLifeTime(lifeTimer);
    PlayerManager::Instance().SetLevel(level);
}

void Player::LevelUpdate()
{
    if(curExp > levelUpExp)
    {
        for (auto& skill : *skillArray)
        {
            //�J�[�h���f�肱�܂Ȃ��悤�ɓK���ȍ��W�ɔ�΂��Ƃ�
            skill->card->GetSpriteTransform()->SetPos(DirectX::XMFLOAT2(0, 1000));
        }

        level++;
        curExp -= levelUpExp;

        drawDirectionState = 0;

        //���x�����オ��Ɣ\�͂��擾�o����
        isSelectingSkill = true;
    }
}

DirectX::XMFLOAT3 Player::CollidedPosition(const DirectX::XMFLOAT3 pos)
{
    //�X�e�[�W����
    DirectX::XMFLOAT3 collectPos;
    
    float length = Length(pos);
    if (SceneGame::stageRadius < length)
    {
        DirectX::XMFLOAT3 moveNormal = Normalize(pos);
        collectPos = moveNormal * SceneGame::stageRadius;
    }
    else
    {
        collectPos = pos;
    }

    return collectPos;
}

void Player::ColliderPosUpdate(const float& scale)
{
    //��炢����
    {
        const std::string meshBodyName = "ref_P:ref_P:ref_P:pasted__Body";
        const std::string meshLegName = "ref_P:ref_P:ref_P:pasted__Socks";
        hitCollider[static_cast<int>(HitColName::NECK)].position = GetJointPosition(meshBodyName, "ref_P:ref_P:j_Neck", scale);
        hitCollider[static_cast<int>(HitColName::HIP)].position = GetJointPosition(meshBodyName, "ref_P:ref_P:j_Hips", scale);
        hitCollider[static_cast<int>(HitColName::R_ELBOW)].position = GetJointPosition(meshBodyName, "ref_P:ref_P:j_RightForeArm", scale);
        hitCollider[static_cast<int>(HitColName::L_ELBOW)].position = GetJointPosition(meshBodyName, "ref_P:ref_P:j_LeftForeArm", scale);
    
        //hitCollider[static_cast<int>(HitColName::LEG)].position = GetJointPosition(meshBodyName,"setup_0927:chara_rig_0906:j_Sentar",scale);
        hitCollider[static_cast<int>(HitColName::R_LEG)].position = GetJointPosition(meshLegName, "ref_P:ref_P:j_RightLeg", scale);
        hitCollider[static_cast<int>(HitColName::L_LEG)].position = GetJointPosition(meshLegName, "ref_P:ref_P:j_LeftLeg", scale);
        hitCollider[static_cast<int>(HitColName::R_LEG_END)].position = GetJointPosition(meshLegName, "ref_P:ref_P:j_RightFoot", scale);
        hitCollider[static_cast<int>(HitColName::L_LEG_END)].position = GetJointPosition(meshLegName, "ref_P:ref_P:j_LeftFoot", scale);
    }

    //�U������
    {
        DirectX::XMFLOAT4X4 world{};
        DirectX::XMStoreFloat4x4(&world, swordModel->GetTransform()->CalcWorldMatrix(scale));
        const std::string swordMeshName = "ref_S:sword_rig_1005:sword_mdl_1005:Sword";
        const DirectX::XMFLOAT3 swordRoot = swordModel->skinned_meshes->JointPosition(swordMeshName, "ref_S:sword_rig_1005:j_sword",&swordModel->keyframe ,world);//���{
        const DirectX::XMFLOAT3 swordTip = swordModel->skinned_meshes->JointPosition(swordMeshName, "ref_S:sword_rig_1005:j_sword_end", &swordModel->keyframe, world);//��[
     
        const DirectX::XMFLOAT3 vec = swordTip - swordRoot;
        float swordLength = Length(vec);
        const DirectX::XMFLOAT3 vecNormal = Normalize(vec);

        const float collideInterval = swordLength / swordColliderNum;//���育�Ƃ̐ݒu�Ԋu

        for (int i = 0;i < attackCollider.size();++i)
        {
            attackCollider[i].position = swordRoot + vecNormal * collideInterval * i;
        }
    }
}
