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
            "./Resources/Shader/EnemyPS.cso",
            emissiveTextureUVScroll.GetAddressOf());
    }


    // BehaviorTree�ݒ�
    {
        behaviorData_ = make_unique<BehaviorData>();        // �r�w�C�r�A�f�[�^����
        behaviorTree_ = make_unique<BehaviorTree>(this);    // �r�w�C�r�A�c���[����

        // �r�w�C�r�A�m�[�h�ǉ�
        behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);                                                                  // ������
        behaviorTree_->AddNode("Root", "BlowOff",          1, SelectRule::Non, new BlowOffJudgment(this),          new BlowOffAction(this));            // �������
        behaviorTree_->AddNode("Root", "Flinch",           2, SelectRule::Non, new FlinchJudgment(this),           new FlinchAction(this));             // �Ђ��
        behaviorTree_->AddNode("Root", "EntryStage",       3, SelectRule::Non, new EntryStageJudgment(this),       new EntryStageAction(this));         // �X�e�[�W����
        behaviorTree_->AddNode("Root", "LongRangeAttack",  4, SelectRule::Non, new LongRangeAttackJudgment(this),  new LongRangeAttackAction(this));    // �������U��
        behaviorTree_->AddNode("Root", "Pursuit",          5, SelectRule::Non, new PursuitJudgment(this),          new PursuitAction(this));            // �ǐ�
        behaviorTree_->AddNode("Root", "Idle",             6, SelectRule::Non, nullptr,                            new IdleAction(this));               // �ҋ@
    }

    SetType(Enemy::TYPE::Normal);

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

    constexpr float scale = 0.7f;
    this->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

    attackLength_ = 6.0f; // �U�����\�ȋ������̂΂�

    ColliderInitialize();
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

    ColliderPosUpdate(scale);
}


void EnemyAI_3::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(GetName()))
    {
        BaseEnemyAI::DrawDebug();

        ImGui::EndMenu();
    }

#endif // USE_IMGUI
}


void EnemyAI_3::UpdateConstants()
{
    // emissive
    {
        // emissive�̋���
        SetEmissiveIntensity(1.25f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color
        SetEmissiveColor(DirectX::XMFLOAT4(0.00f, 0.80f, 0.81f, 1.0f));
    }

}
