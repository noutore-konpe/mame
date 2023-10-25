#include "EnemyAI_Tutorial.h"

#include "../../Taki174/NumeralManager.h"
#include "../../Taki174/Common.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "ActionDerived.h"
#include "JudgmentDerived.h"
#include "EnemyManager.h"
#include "WaveManager.h"

int EnemyAI_Tutorial::nameNum_ = 0;

// �R���X�g���N�^
EnemyAI_Tutorial::EnemyAI_Tutorial()
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
        behaviorTree_->AddNode("",     "Root",       0, SelectRule::Priority, nullptr,                      nullptr);                            // ������
        behaviorTree_->AddNode("Root", "BlowOff",    1, SelectRule::Non,      new BlowOffJudgment(this),    new BlowOffAction(this));            // �������
        behaviorTree_->AddNode("Root", "Flinch",     2, SelectRule::Non,      new FlinchJudgment(this),     new FlinchAction(this));             // �Ђ��
        behaviorTree_->AddNode("Root", "EntryStage", 3, SelectRule::Non,      new EntryStageJudgment(this), new EntryStageAction(this));         // �X�e�[�W����
        behaviorTree_->AddNode("Root", "Pursuit",    5, SelectRule::Non,      new PursuitJudgment(this),    new PursuitAction(this));            // �ǐ�
        behaviorTree_->AddNode("Root", "Idle",       6, SelectRule::Non,      nullptr,                      new IdleAction(this));               // �ҋ@
    }

    SetType(Enemy::TYPE::Normal);

    // ImGui���O�ݒ�
    name_ = { "EnemyAI_Tutorial : " + std::to_string(nameNum_++) };
}


// �f�X�g���N�^
EnemyAI_Tutorial::~EnemyAI_Tutorial()
{
    --nameNum_;
    if (isDead) EnemyManager::Instance().AddEnemy1KillNum();
}


// ������
void EnemyAI_Tutorial::Initialize()
{
    BaseEnemyAI::Initialize();

    constexpr float scale = 0.7f;
    this->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
    sword_->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

    sword_->PlayAnimation(0, true); // ���̃f�t�H���g�A�j���[�V�����Đ��Đ�

    maxHealth     = 20.0f;
    health        = maxHealth;
    attack_       = 0.0f;

    // �h���b�v�o���l�[���ݒ�
    dropExpCount_ = 0;

    // �[���Ԗڂ̃Q�[�g�ʒu�������ʒu�ɐݒ�
    SetPosition(::GetGatewayPosition(0));

}


// �X�V����
void EnemyAI_Tutorial::Update(const float& elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);
}


// �`�揈��
void EnemyAI_Tutorial::Render(const float& scale, ID3D11PixelShader* /*psShader*/)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // �萔�o�b�t�@�[�X�V
    UpdateConstants();

    // Aura enemy
    BaseEnemyAI::Render(scale, emissiveTextureUVScroll.Get());

    // ���`��
    sword_->Render(scale, emissiveTextureUVScroll.Get());

    BaseEnemyAI::ColliderPosUpdate(scale);

}


void EnemyAI_Tutorial::DrawDebug()
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


void EnemyAI_Tutorial::UpdateConstants()
{
    // emissive
    {
        // emissive�̋���
        SetEmissiveIntensity(1.25f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color
        SetEmissiveColor(DirectX::XMFLOAT4(0.11f, 0.56f, 1.00f, 1.0f));
    }
}
