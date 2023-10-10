#include "EnemySlime.h"
#include "../Graphics/Graphics.h"

int EnemySlime::nameNum_;

// �R���X�g���N�^
EnemySlime::EnemySlime()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        //"./Resources/Model/sanaModel/mameoall.fbx");
        //"./Resources/Model/testModel/character.fbx");
        //"./Resources/Model/Cube.fbx");
        //"./Resources/Model/yoshiakiModel/Cube.fbx");
        "./Resources/Model/testModel/plantune.fbx");
        //"./Resources/Model/testModel/latha.fbx");
        //"./Resources/Model/Collision/sqhere.fbx");

    // ImGui���O�ݒ�
    SetName("EnemySlime" + std::to_string(nameNum_++));
}

// �f�X�g���N�^
EnemySlime::~EnemySlime()
{
}

// ������
void EnemySlime::Initialize()
{
    Enemy::Initialize();

    Character::PlayAnimation(0, true);
}

// �I����
void EnemySlime::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void EnemySlime::Begin()
{
}

// �X�V����
void EnemySlime::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);
}

// Update�̌�ɌĂ΂��
void EnemySlime::End()
{
}

// �`�揈��
void EnemySlime::Render(const float& scale, ID3D11PixelShader* psShader)
{
    // �萔�X�V
    UpdateConstants();

    Enemy::Render(scale, psShader);
}

// ImGui�p
void EnemySlime::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

        float range = GetRange();
        ImGui::DragFloat("range", &range);
        SetRange(range);

        model->skinned_meshes->Drawdebug();

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

void EnemySlime::UpdateConstants()
{
    // emissive
    {
        // emissive�̋���
        SetEmissiveIntensity(1.5f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color �Ƃ肠������
        SetEmissiveColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
    }
}
