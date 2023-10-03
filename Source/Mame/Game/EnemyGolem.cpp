#include "EnemyGolem.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

// �R���X�g���N�^
EnemyGolem::EnemyGolem()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/golem.fbx");

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
}

void EnemyGolem::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

        model->skinned_meshes->Drawdebug();

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

void EnemyGolem::UpdateConstants()
{
}
