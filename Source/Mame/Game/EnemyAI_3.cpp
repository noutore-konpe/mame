#include "EnemyAI_3.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Game/ActionDerived.h"
#include "../Game/JudgmentDerived.h"

int EnemyAI_3::nameNum_ = 0;

// �R���X�g���N�^
EnemyAI_3::EnemyAI_3()
{
    using std::make_unique;
    using std::to_string;
    using SelectRule = BehaviorTree::SelectRule;

    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Player/P_Motion.fbx");

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


    // BehaviorTree�ݒ�
    {
        behaviorData_ = make_unique<BehaviorData>();
        behaviorTree_ = make_unique<BehaviorTree>(this);

        behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);
        behaviorTree_->AddNode("Root", "LongRangeAttack", 1, SelectRule::Non, new LongRangeAttackJudgment(this), new LongRangeAttackAction(this));
        behaviorTree_->AddNode("Root", "Pursuit", 2, SelectRule::Non, new PursuitJudgment(this), new PursuitAction(this));
        behaviorTree_->AddNode("Root", "Idle", 3, SelectRule::Non, nullptr, new IdleAction(this));
    }


    // ImGui���O�ݒ�
    SetName("EnemyAI_3 : " + std::to_string(nameNum_++));
}

// �f�X�g���N�^
EnemyAI_3::~EnemyAI_3()
{
    --nameNum_;
}

// ������
void EnemyAI_3::Initialize()
{
    BaseEnemyAI::Initialize();

    attackLength_ = 6.0f; // �U�����\�ȋ������̂΂�

}


// �X�V����
void EnemyAI_3::Update(const float& elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);

}


// �`�揈��
void EnemyAI_3::Render(const float& scale, ID3D11PixelShader* /*psShader*/)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // �萔�o�b�t�@�[�X�V
    UpdateConstants();

    // Aura enemy
    BaseEnemyAI::Render(scale, emissiveTextureUVScroll.Get());

}


void EnemyAI_3::DrawDebug()
{
    BaseEnemyAI::DrawDebug();

#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

        model->skinned_meshes->Drawdebug();

        ImGui::EndMenu();
    }

#endif // USE_IMGUI

}


void EnemyAI_3::UpdateConstants()
{
    // emissive
    {
        // emissive�̋���
        SetEmissiveIntensity(1.5f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color ��
        SetEmissiveColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
    }

}
