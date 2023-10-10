#include "EnemyGolem.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Other/MathHelper.h"

#include "EnemyGolemState.h"

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
    comboAttackStone = std::make_unique<ComboAttackStone>();


    // �X�e�[�g�}�V��
    {
        stateMachine.reset(new StateMachine<State<EnemyGolem>>);

        GetStateMachine()->RegisterState(new EnemyGolemState::IdleState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::EntryState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::RoarState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::SummonState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::GetUpState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::Attack1State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::ComboAttack1State(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::IdleState));
    }

    // ImGui���O�ݒ�
    SetName("EnemyGolem" + std::to_string(nameNum++));
}

// �f�X�g���N�^
EnemyGolem::~EnemyGolem()
{
}

// ������
void EnemyGolem::Initialize()
{
    Enemy::Initialize();

    magicCircleGolem->Initialize();
    magicCircleEnemySummon->Initialize();

    comboAttackStone->Initialize();

    GetTransform()->SetPositionY(10.0f);

    // �A�j���[�V�����Đ�
    Character::PlayAnimation(static_cast<UINT>(Animation::Idle), true);

    currentState = static_cast<UINT>(StateMachineState::IdleState);

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
    comboAttackStone->SetOwnerTransform(GetTransform());
    comboAttackStone->Update(elapsedTime);

    UpdateSummoningMagicCircle(4.0f, 3.0f, DirectX::XMConvertToRadians(45));

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

    magicCircleGolem->Render();
    magicCircleEnemySummon->Render(DirectX::XMFLOAT4(magicCircleColor[2]));
    comboAttackStone->Render();

    if (currentState == static_cast<UINT>(StateMachineState::ComboAttack1State))
    {
    }
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

        ImGui::SliderInt("currentState", &currentStateDebug, 0, 4);

        if (ImGui::Button(stateName[currentStateDebug]))
        {
            switch (currentStateDebug)
            {
            case 0:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::EntryState));
                break;
            case 1:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::SummonState));
                break;
            case 2:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::RoarState));
                break;
            case 3:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::Attack1State));
                break;
            case 4:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::ComboAttack1State));
                break;
            }
        }

        if (ImGui::Button("magicCircle"))
        {
            magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));
        }

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

void EnemyGolem::UpdateConstants()
{
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
