#include "EnemyGolem.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Other/MathHelper.h"

#include "EnemyGolemState.h"

#include "../Scene/SceneGame.h"

#include "EnemyManager.h"

int EnemyGolem::nameNum_;


// �R���X�g���N�^
EnemyGolem::EnemyGolem()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/golem.fbx");

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/playerPS.cso",
        golemPS.GetAddressOf());

    magicCircleGolem = std::make_unique<MagicCircleGolem>();
    magicCircleEnemySummon = std::make_unique<MagicCircleEnemySummon>();
    magicCircleGolemAttack2 = std::make_unique<MagicCircleGolemAttack2>();
    comboAttackStone = std::make_unique<ComboAttackStone>();

    for (int i = 0; i < 3; ++i)
    {
        comboAttackStones[i] = std::make_unique<ComboAttackStone>();
    }

    // �X�e�[�g�}�V��
    {
        stateMachine.reset(new StateMachine<State<EnemyGolem>>);

        GetStateMachine()->RegisterState(new EnemyGolemState::DummyState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::IdleState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::EntryState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::RoarState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::SummonState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::GetUpState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::Attack1State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::ComboAttack1State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::DownState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::ComboAttack2State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::ChoseState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::DeathState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::WalkState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::Attack2State(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::DummyState));
    }

    SetType(Enemy::TYPE::Golem);

    // ImGui���O�ݒ�
    SetName("EnemyGolem" + std::to_string(nameNum_++));

    //�g���ݒ�
    lockOnHeight = 2.0f;
}

// �f�X�g���N�^
EnemyGolem::~EnemyGolem()
{
    if (isDead)
        EnemyManager::Instance().AddEnemyGolemKillNum();
}

// ������
void EnemyGolem::Initialize()
{
    Enemy::Initialize();

    magicCircleGolem->Initialize();
    magicCircleEnemySummon->Initialize();
    magicCircleGolemAttack2->Initialize();

    comboAttackStone->Initialize();

    for (int i = 0; i < 3; ++i)
    {
        comboAttackStones[i]->Initialize();
    }

    //GetTransform()->SetPositionY(10.0f);

    // �A�j���[�V�����Đ�
    Character::PlayAnimation(static_cast<UINT>(Animation::Idle), true);

    currentState = static_cast<UINT>(StateMachineState::EntryState);

    // �X�e�[�W�ɓ���ς݈����ɂ���
    entryStageFlag_ = true;

    //����ݒ�
    ColliderInitialize();

#ifdef _DEBUG
    currentStateDebug = 0;
#endif // _DEBUG
}

// �I����
void EnemyGolem::Finalize()
{
}

void EnemyGolem::Begin()
{
}

// �X�V����
void EnemyGolem::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    magicCircleGolem->Update(elapsedTime);
    magicCircleEnemySummon->Update(elapsedTime);
    magicCircleGolemAttack2->Update(elapsedTime);
    comboAttackStone->SetOwnerTransform(GetTransform());
    comboAttackStone->Update(elapsedTime);

    for (int i = 0; i < 3; ++i)
    {
        comboAttackStones[i]->Update(elapsedTime);
        comboAttackStones[i]->SetOwnerTransform(GetTransform());
    }

    UpdateSummoningMagicCircle(4.0f, 3.0f, DirectX::XMConvertToRadians(45));
    UpdateAttack2MagicCircle(2.0f, 3.5f);

    // �X�e�[�g�}�V���X�V
    GetStateMachine()->Update(elapsedTime);

    // �A�j���[�V�����X�V
    Character::UpdateAnimation(elapsedTime);
}

void EnemyGolem::End()
{
}

// �`�揈��
void EnemyGolem::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();

    // �萔�o�b�t�@�[�X�V
    UpdateConstants();

    //Enemy::Render(scale, psShader);
    Enemy::Render(scale, golemPS.Get());

    // �K�v�Ȏ������`�悷��
    SubRender();

    //magicCircleGolemAttack2->Render(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

#if _DEBUG
    if (SceneGame::isDispCollision_)
    {
        for (auto& collider : hitCollider)
        {
            collider.DebugRender();
        }

        for (auto& collider : attackCollider)
        {
            collider.DebugRender(DirectX::XMFLOAT4(1, 0, 0, 1));
        }
    }
#endif

    ColliderPosUpdate(scale);//����ʒu���e�W���C���g�ɒǏ]
}

// �e�`��p
void EnemyGolem::Render(const float& scale, bool shadow, ID3D11PixelShader* psShader)
{
    // �萔�o�b�t�@�[�X�V
    UpdateConstants();

    Enemy::Render(scale, psShader);
}

void EnemyGolem::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

        model->skinned_meshes->Drawdebug();

        magicCircleGolem->DrawDebug();
        magicCircleEnemySummon->DrawDebug();

        comboAttackStone->DrawDebug();

        ImGui::SliderInt("currentState", &currentStateDebug, 0, 10);

        if (ImGui::Button(stateName[currentStateDebug]))
        {
            switch (currentStateDebug)
            {
            case 0:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::DummyState));
                break;
            case 1:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::EntryState));
                break;
            case 2:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::SummonState));
                break;
            case 3:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::RoarState));
                break;
            case 4:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::Attack1State));
                break;
            case 5:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::ComboAttack1State));
                break;
            case 6:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::DownState));
                break;
            case 7:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::ComboAttack2State));
                break;
            case 8:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::DeathState));
                break;
            case 9:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::WalkState));
                break;
            case 10:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::Attack2State));
                break;
            }
        }

        if (ImGui::Button("magicCircle"))
        {
            magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));
        }

        ImGui::Begin("Collider");
        
        ImGui::SliderFloat("hip", &hitCollider[static_cast<float>(ColliderName::HIP)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("R_leg", &hitCollider[static_cast<float>(ColliderName::R_LEG)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("L_leg", &hitCollider[static_cast<float>(ColliderName::L_LEG)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("R_knee", &hitCollider[static_cast<float>(ColliderName::R_LEG_END)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("L_knee", &hitCollider[static_cast<float>(ColliderName::L_LEG_END)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("R_shoulder", &hitCollider[static_cast<float>(ColliderName::R_SHOULDER)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("L_shoulder", &hitCollider[static_cast<float>(ColliderName::L_SHOULDER)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("R_hand", &hitCollider[static_cast<float>(ColliderName::R_HAND)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("L_hand", &hitCollider[static_cast<float>(ColliderName::L_HAND)].radius, 0.0f, 2.0f);

        ImGui::End();
        

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

void EnemyGolem::UpdateConstants()
{
}

void EnemyGolem::OnDead(DamageResult result)
{
    GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::DeathState));
}

void EnemyGolem::SubRender()
{
    switch (currentState)
    {
    case static_cast<UINT>(StateMachineState::SummonState):
        // �������@
        magicCircleEnemySummon->Render(DirectX::XMFLOAT4(magicCircleColor[0]));
        break;
    case static_cast<UINT>(StateMachineState::ComboAttack1State):
        // �R���{�U���P
        comboAttackStone->Render();
        break;
    case static_cast<UINT>(StateMachineState::ComboAttack2State):
        // �R���{�U���Q
        for (int i = 0; i < 3; ++i)
        {
            comboAttackStones[i]->Render();
        }
        break;
    }
    
    if (magicCircleGolemAttack2->isAttack2)
    {
        // �U���Q
        magicCircleGolemAttack2->Render(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    }
}

// �������@�w�X�V����
void EnemyGolem::UpdateSummoningMagicCircle(const float& lengthX, const float& lengthZ, const float& angle)
{
    // �S�[�����̈ʒu
    DirectX::XMFLOAT3 ownerPosition = GetTransform()->GetPosition();
    // �S�[�����̑O�x�N�g��
    DirectX::XMFLOAT3 ownerFrontVec = GetTransform()->CalcForward();
    // �S�[�����̉E�x�N�g��
    DirectX::XMFLOAT3 ownerRightVec = GetTransform()->CalcRight();

    // base
    {
        {   // ���
            // �����擾
            DirectX::XMFLOAT3 setPosition = {
                ownerFrontVec.x * sinf(angle) + ownerRightVec.x * cosf(angle),
                0.0f,
                ownerFrontVec.z * sinf(angle) + ownerRightVec.z * cosf(angle) };
            // �x�N�g�������߂�
            setPosition = { setPosition.x * lengthX, 0.0f, setPosition.z * lengthZ };
            // �ʒu���Z�o
            setPosition = setPosition + ownerPosition;

            // �ʒu��ݒ�
            magicCircleEnemySummon->magicCircle[0]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
        }
        {   // ���
             // �����擾
            DirectX::XMFLOAT3 setPosition = {
                ownerFrontVec.x * sinf(angle) - ownerRightVec.x * cosf(angle),
                0.0f,
                ownerFrontVec.z * sinf(angle) - ownerRightVec.z * cosf(angle) };
            // �x�N�g�������߂�
            setPosition = { setPosition.x * lengthX, 0.0f, setPosition.z * lengthZ };
            // �ʒu���Z�o
            setPosition = setPosition + ownerPosition;

            // �ʒu��ݒ�
            magicCircleEnemySummon->magicCircle[1]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
        }
    }
    // move
    {
        // ���
        {
            for (int i = 0; i < 3; ++i)
            {
                float magicCirclePosY = magicCircleEnemySummon->magicCircleMove[0][i]->GetTransform()->GetPosition().y;
                // �����擾
                DirectX::XMFLOAT3 setPosition = {
                ownerFrontVec.x* sinf(angle) + ownerRightVec.x * cosf(angle),
                0.0f,
                ownerFrontVec.z* sinf(angle) + ownerRightVec.z * cosf(angle) };
                // �x�N�g�������߂�
                setPosition = { setPosition.x * lengthX, 0.0f, setPosition.z * lengthZ };
                // �ʒu���Z�o
                setPosition = setPosition + ownerPosition;
                setPosition.y = magicCirclePosY;

                // �ʒu��ݒ�
                magicCircleEnemySummon->magicCircleMove[0][i]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
            }
        }

        // ���
        {
            for (int i = 0; i < 3; ++i)
            {
                float magicCirclePosY = magicCircleEnemySummon->magicCircleMove[0][i]->GetTransform()->GetPosition().y;
                // �����擾
                DirectX::XMFLOAT3 setPosition = {
                ownerFrontVec.x* sinf(angle) - ownerRightVec.x * cosf(angle),
                0.0f,
                ownerFrontVec.z* sinf(angle) - ownerRightVec.z * cosf(angle) };
                // �x�N�g�������߂�
                setPosition = { setPosition.x * lengthX, 0.0f, setPosition.z * lengthZ };
                // �ʒu���Z�o
                setPosition = setPosition + ownerPosition;
                setPosition.y = magicCirclePosY;

                // �ʒu��ݒ�
                magicCircleEnemySummon->magicCircleMove[1][i]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
            }
        }
    }
}

// �U�����@�w�X�V����
void EnemyGolem::UpdateAttack2MagicCircle(const float& lengthX, const float& lengthZ)
{
    // �S�[�����̈ʒu
    DirectX::XMFLOAT3 ownerPosition = GetTransform()->GetPosition();
    // �S�[�����̑O�x�N�g��
    DirectX::XMFLOAT3 ownerFrontVec = GetTransform()->CalcForward();
    // �S�[�����̉E�x�N�g��
    DirectX::XMFLOAT3 ownerRightVec = GetTransform()->CalcRight();
    

    // �^�񒆂̖��@�w
    DirectX::XMFLOAT3 centerPos =
    {
        ownerPosition.x + ownerFrontVec.x * 2.0f,
        ownerPosition.y + 2.0f,
        ownerPosition.z + ownerFrontVec.z * 2.0f
    };

    DirectX::XMFLOAT3 centerRightVec = 
        magicCircleGolemAttack2->baseMagicCircle[static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Center)]->GetTransform()->CalcRight();

    float direction = 2.0f;
    DirectX::XMFLOAT3 sidePos[2] =
    {
        {
            centerPos.x + centerRightVec.x * direction,
            centerPos.y + centerRightVec.y,
            centerPos.z + centerRightVec.z * direction
        },
        {
            centerPos.x - centerRightVec.x * direction,
            centerPos.y - centerRightVec.y,
            centerPos.z - centerRightVec.z * direction
        }
    };

    magicCircleGolemAttack2->baseMagicCircle[static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Center)]->GetTransform()->SetPosition(centerPos);
    magicCircleGolemAttack2->baseMagicCircle[static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Right)]->GetTransform()->SetPosition(sidePos[0]);
    magicCircleGolemAttack2->baseMagicCircle[static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Left)]->GetTransform()->SetPosition(sidePos[1]);
    
    for (int i = 0; i < static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Max); ++i)
    {
        magicCircleGolemAttack2->baseMagicCircle[i]->GetTransform()->
            SetRotationY(GetTransform()->GetRotation().y);
    }
}

void EnemyGolem::ColliderInitialize()
{
    //������
    if (hitCollider.size() == 0)
    {
        for (int i = 0; i < static_cast<int>(ColliderName::END); i++)
        {
            attackCollider.emplace_back(SphereCollider(0.3f));
        }

        for (int i = 0; i < static_cast<int>(ColliderName::END); i++)
        {
            hitCollider.emplace_back(SphereCollider(0.55f));
        }
    }
    const float scale = GetTransform()->GetScale().x;
    /*hitCollider[static_cast<int>(ColliderName::NECK)].radius = 0.15f * scale;
    hitCollider[static_cast<int>(ColliderName::HIP)].radius = 0.17f * scale;
    hitCollider[static_cast<int>(ColliderName::R_LEG)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::L_LEG)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::R_LEG_END)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::L_LEG_END)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::R_ELBOW)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::L_ELBOW)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::R_HAND)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::L_HAND)].radius = 0.12f * scale;*/
}

void EnemyGolem::ColliderPosUpdate(const float& scale)
{
    hitCollider[static_cast<int>(ColliderName::HIP)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface11", "golem_setup_1004:golem_rig_1005:j_Spine", scale);
    hitCollider[static_cast<int>(ColliderName::R_SHOULDER)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface6", "golem_setup_1004:golem_rig_1005:j_RightArm", scale);
    hitCollider[static_cast<int>(ColliderName::L_SHOULDER)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface5", "golem_setup_1004:golem_rig_1005:j_LeftArm", scale);
    hitCollider[static_cast<int>(ColliderName::R_HAND)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface3", "golem_setup_1004:golem_rig_1005:j_RightHand", scale);
    hitCollider[static_cast<int>(ColliderName::L_HAND)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface1", "golem_setup_1004:golem_rig_1005:j_LeftHand", scale);
    hitCollider[static_cast<int>(ColliderName::R_LEG)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface11", "golem_setup_1004:golem_rig_1005:j_RightUptLeg", scale);
    hitCollider[static_cast<int>(ColliderName::L_LEG)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface11", "golem_setup_1004:golem_rig_1005:j_LeftUptLeg", scale);
    hitCollider[static_cast<int>(ColliderName::R_LEG_END)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface9", "golem_setup_1004:golem_rig_1005:j_RighttLeg", scale);
    hitCollider[static_cast<int>(ColliderName::L_LEG_END)].position = GetJointPosition("golem_setup_1004:golem_rig_1005:golem_mdl_1004:polySurface8", "golem_setup_1004:golem_rig_1005:j_LefttLeg", scale);

    for (int i = 0; i < static_cast<int>(ColliderName::END); i++)
    {
        attackCollider[i].position = hitCollider[i].position;
    }
}
