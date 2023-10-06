#include "EnemyAura.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Game/ActionDerived.h"
#include "../Game/JudgmentDerived.h"

int EnemyAura::nameNum_ = 0;

// �R���X�g���N�^
EnemyAura::EnemyAura()
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
        //behaviorTree_->AddNode("Root", "CloseRangeAttack", 1, SelectRule::Non, new CloseRangeAttackJudgment(this), new CloseRangeAttackAction(this));
        //behaviorTree_->AddNode("Root", "Pursuit",          2, SelectRule::Non, new PursuitJudgment(this), new PursuitAction(this));
        //behaviorTree_->AddNode("Root", "Idle",             3, SelectRule::Non, nullptr, new IdleAction(this));
    }


    // ImGui���O�ݒ�
    SetName("AuraEnemy" + std::to_string(nameNum_++));
}

// �f�X�g���N�^
EnemyAura::~EnemyAura()
{
    --nameNum_;
}

// ������
void EnemyAura::Initialize()
{
    BaseEnemyAI::Initialize();

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
    BaseEnemyAI::Update(elapsedTime);

    //// �A�j���[�V�����X�V
    //Character::UpdateAnimation(elapsedTime);
}

// Update�̌�ɌĂ΂��
void EnemyAura::End()
{
}

// �`�揈��
void EnemyAura::Render(const float& /*scale*/, ID3D11PixelShader* /*psShader*/)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // �萔�o�b�t�@�[�X�V
    UpdateConstants();

    // Aura enemy
    BaseEnemyAI::Render(0.01f, emissiveTextureUVScroll.Get());
}

void EnemyAura::DrawDebug()
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

void EnemyAura::UpdateConstants()
{
    // emissive
    {
        // emissive�̋���
        SetEmissiveIntensity(1.5f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color
        SetEmissiveColor(DirectX::XMFLOAT4(0.2f, 0.7f, 1.0f, 1.0f));
        // color �Ƃ肠������
        //SetEmissiveColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    }
}
