#include "EnemyAura.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

// �R���X�g���N�^
EnemyAura::EnemyAura()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/testModel/plantune.fbx");

    // emissiveTextureUVScroll
    {
        // load texture
        D3D11_TEXTURE2D_DESC texture2dDesc;
        load_texture_from_file(graphics.GetDevice(),
            L"./Resources/Image/Mask/noise3.png",
            emissiveTexture.GetAddressOf(),
            &texture2dDesc);

        // pixelShader Set (Aura)
        CreatePsFromCso(graphics.GetDevice(),
            "./Resources/Shader/EmissiveTextureUVScrollPS.cso",
            emissiveTextureUVScroll.GetAddressOf());
    }

    // ImGui���O�ݒ�
    SetName("AuraEnemy" + std::to_string(nameNum++));
}

// �f�X�g���N�^
EnemyAura::~EnemyAura()
{
}

// ������
void EnemyAura::Initialize()
{
    Enemy::Initialize();

    // �A�j���[�V�����Đ�
    Character::PlayAnimation(0, true);
}

// �I����
void EnemyAura::Finalize()
{
}

// Update�̑O�ɌĂ΂��
void EnemyAura::Begin()
{
}

// �X�V����
void EnemyAura::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    // �A�j���[�V�����X�V
    Character::UpdateAnimation(elapsedTime);
}

// Update�̌�ɌĂ΂��
void EnemyAura::End()
{
}

// �`�揈��
void EnemyAura::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // �萔�o�b�t�@�[�X�V
    UpdateConstants();

    // Aura enemy
    Enemy::Render(scale, emissiveTextureUVScroll.Get());
}

void EnemyAura::DrawDebug()
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

void EnemyAura::UpdateConstants()
{
    // emissive
    {
        // emissive�̋���
        SetEmissiveIntensity(1.5f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color �Ƃ肠������
        SetEmissiveColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    }
}
