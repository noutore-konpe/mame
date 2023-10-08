#include "EnemyAI_2.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Game/ActionDerived.h"
#include "../Game/JudgmentDerived.h"

int EnemyAI_2::nameNum_ = 0;

// コンストラクタ
EnemyAI_2::EnemyAI_2()
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


    // BehaviorTree設定
    {
        behaviorData_ = make_unique<BehaviorData>();
        behaviorTree_ = make_unique<BehaviorTree>(this);

        behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);
        behaviorTree_->AddNode("Root", "CloseRangeAttack", 1, SelectRule::Non, new CloseRangeAttackJudgment(this), new CloseRangeAttackAction(this));
        behaviorTree_->AddNode("Root", "Pursuit",          2, SelectRule::Non, new PursuitJudgment(this), new PursuitAction(this));
        behaviorTree_->AddNode("Root", "Idle",             3, SelectRule::Non, nullptr, new IdleAction(this));
    }

    // ImGui名前設定
    SetName("EnemyAI_2 : " + std::to_string(nameNum_++));

}

// デストラクタ
EnemyAI_2::~EnemyAI_2()
{
    --nameNum_;
}

// 初期化
void EnemyAI_2::Initialize()
{
    using DirectX::XMFLOAT3;

    BaseEnemyAI::Initialize();

    constexpr float scale = 1.5f;
    GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));

    // アニメーション再生
    Character::PlayAnimation(0, true);

}


// 更新処理
void EnemyAI_2::Update(const float& elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);

}


// 描画処理
void EnemyAI_2::Render(const float& scale, ID3D11PixelShader* /*psShader*/)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // 定数バッファー更新
    UpdateConstants();

    graphics.GetShader()->SetBlendState(static_cast<UINT>(Shader::BLEND_STATE::ALPHA));

    // Aura enemy
    BaseEnemyAI::Render(scale, emissiveTextureUVScroll.Get());
}


void EnemyAI_2::DrawDebug()
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


void EnemyAI_2::UpdateConstants()
{
    // emissive
    {
        // emissiveの強さ
        SetEmissiveIntensity(1.5f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // midnightblue
        SetEmissiveColor(DirectX::XMFLOAT4(0.00f, 0.80f, 0.81f, 1.0f));
        //SetEmissiveColor(DirectX::XMFLOAT4(0.09f, 0.09f, 0.43f, 1.0f));
    }

}
