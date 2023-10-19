#include "EnemyAI_2.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Game/ActionDerived.h"
#include "../Game/JudgmentDerived.h"

int EnemyAI_2::nameNum_ = 0;

// �R���X�g���N�^
EnemyAI_2::EnemyAI_2()
{
    using std::make_unique;
    using std::to_string;
    using SelectRule = BehaviorTree::SelectRule;

    Graphics& graphics = Graphics::Instance();

    // ���f���쐬
    {
        model = make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/Character/Player/P_Motion.fbx");

        // ���쐬
        sword_ = make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/Character/Sword_Motion.fbx");
    }


    // emissiveTextureUVScroll
    {
        // load texture
        D3D11_TEXTURE2D_DESC texture2dDesc;
        ::load_texture_from_file(graphics.GetDevice(),
            L"./Resources/Image/Mask/noise3.png",
            emissiveTexture.GetAddressOf(),
            &texture2dDesc);

        // pixelShader Set (Aura)
        ::CreatePsFromCso(graphics.GetDevice(),
            "./Resources/Shader/EnemyPS.cso",
            emissiveTextureUVScroll.GetAddressOf());
    }


    // BehaviorTree�ݒ�
    {
        behaviorData_ = make_unique<BehaviorData>();        // �r�w�C�r�A�f�[�^����
        behaviorTree_ = make_unique<BehaviorTree>(this);    // �r�w�C�r�A�c���[����

        // �r�w�C�r�A�m�[�h�ǉ�
        behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);                                                                  // ������
        behaviorTree_->AddNode("Root", "Flinch",           1, SelectRule::Non, new FlinchJudgment(this),           new FlinchAction(this));             // �Ђ��
        behaviorTree_->AddNode("Root", "EntryStage",       2, SelectRule::Non, new EntryStageJudgment(this),       new EntryStageAction(this));         // �X�e�[�W����
        behaviorTree_->AddNode("Root", "CloseRangeAttack", 3, SelectRule::Non, new CloseRangeAttackJudgment(this), new CloseRangeAttackAction(this));   // �ߋ����U��
        behaviorTree_->AddNode("Root", "Pursuit",          4, SelectRule::Non, new PursuitJudgment(this),          new PursuitAction(this));            // �ǐ�
        behaviorTree_->AddNode("Root", "Idle",             5, SelectRule::Non, nullptr,                            new IdleAction(this));               // �ҋ@
    }
    SetType(Enemy::TYPE::Normal);



    // ImGui���O�ݒ�
    name_ = { "EnemyAI_2 : " + std::to_string(nameNum_++) };

}

// �f�X�g���N�^
EnemyAI_2::~EnemyAI_2()
{
    --nameNum_;
}

// ������
void EnemyAI_2::Initialize()
{
    using DirectX::XMFLOAT3;

    BaseEnemyAI::Initialize();

    constexpr float scale = 1.0f;
    this->GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));
    sword_->GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));

    moveSpeed_      = 2.0f;
    animationSpeed_ = 0.5f;

    sword_->PlayAnimation(0, true); // �f�t�H���g�A�j���[�V�����Đ��Đ�
}


// �X�V����
void EnemyAI_2::Update(const float& elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);
}


// �`�揈��
void EnemyAI_2::Render(const float& scale, ID3D11PixelShader* /*psShader*/)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // �萔�o�b�t�@�[�X�V
    UpdateConstants();

    graphics.GetShader()->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));

    // Aura enemy
    BaseEnemyAI::Render(scale, emissiveTextureUVScroll.Get());

    // ���`��
    sword_->Render(scale, nullptr);

}


void EnemyAI_2::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(GetName()))
    {
        BaseEnemyAI::DrawDebug();

        sword_->DrawDebug(); // ���f�o�b�O�`��

        ImGui::EndMenu();
    }

#endif // USE_IMGUI
}


void EnemyAI_2::UpdateConstants()
{
    // emissive
    {
        // emissive�̋���
        SetEmissiveIntensity(1.0f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color
        SetEmissiveColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
    }

}
