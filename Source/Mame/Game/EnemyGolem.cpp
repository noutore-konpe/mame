#include "EnemyGolem.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Other/MathHelper.h"

// �R���X�g���N�^
EnemyGolem::EnemyGolem()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/golem.fbx");

    magicCircleGolem = std::make_unique<MagicCircleGolem>();
    magicCircleEnemySummon = std::make_unique<MagicCircleEnemySummon>();

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

    // �A�j���[�V�����Đ�
    Character::PlayAnimation(0, true);
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

    // �������@�w�X�V����
    UpdateSummoningMagicCircle(4.0f, 3.0f, DirectX::XMConvertToRadians(45));

    // �A�j���[�V�����X�V
    //Character::UpdateAnimation(elapsedTime);
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

    Enemy::Render(scale, psShader);

    magicCircleGolem->Render();
    magicCircleEnemySummon->Render(DirectX::XMFLOAT4(0.8f, 0.44f, 0.24f, 1.0f));
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

        if (ImGui::Button("magicCircle"))
        {
            magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));
        }

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

void EnemyGolem::UpdateConstants()
{
}

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
