#include "MagicCircle.h"

#include "../Graphics/Graphics.h"

#include "../Resource/texture.h"

// �R���X�g���N�^
MagicCircle::MagicCircle()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Item/MagicCircle/base.fbx");

    D3D11_TEXTURE2D_DESC texture2dDesc;
    load_texture_from_file(graphics.GetDevice(),
        L"./Resources/Model/Item/MagicCircle/base.png",
        emissiveTexture.GetAddressOf(), &texture2dDesc);
    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/MagicCirclePS.cso", magicCirclePS.GetAddressOf());

    // ImGui���O�ݒ�
    SetName("MagicCircle" + std::to_string(nameNum++));
}

// �R���X�g���N�^
MagicCircle::MagicCircle(const char* fbxFilename, const wchar_t* spriteFilename)
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), fbxFilename);

    D3D11_TEXTURE2D_DESC texture2dDesc;
    load_texture_from_file(graphics.GetDevice(), spriteFilename,
        emissiveTexture.GetAddressOf(), &texture2dDesc);
    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/MagicCirclePS.cso", magicCirclePS.GetAddressOf());

    // ImGui���O�ݒ�
    SetName("MagicCircle" + std::to_string(nameNum++));
}

// �f�X�g���N�^
MagicCircle::~MagicCircle()
{
}

// ������
void MagicCircle::Initialize()
{
}

// �I����
void MagicCircle::Finalize()
{
}

void MagicCircle::Begin()
{
}

// �X�V����
void MagicCircle::Update(const float& elapsedTime)
{
}

void MagicCircle::End()
{
}

// �`�揈��
void MagicCircle::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics::Instance().GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    Graphics::Instance().GetShader()->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));

    Item::Render(1.0f, magicCirclePS.Get());
}

void MagicCircle::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        GetTransform()->DrawDebug();

        model->skinned_meshes->Drawdebug();

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}
