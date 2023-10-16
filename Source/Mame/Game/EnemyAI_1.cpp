#include "EnemyAI_1.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "ActionDerived.h"
#include "JudgmentDerived.h"

int EnemyAI_1::nameNum_ = 0;

// コンストラクタ
EnemyAI_1::EnemyAI_1()
{
    using std::make_unique;
    using std::to_string;
    using SelectRule = BehaviorTree::SelectRule;

    Graphics& graphics = Graphics::Instance();

    // モデル作成
    {
        model = make_unique<Model>(graphics.GetDevice(),
            "./Resources/Model/Character/Player/P_Motion.fbx");

        // 剣作成
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
            "./Resources/Shader/EmissiveTextureUVScrollPS.cso",
            emissiveTextureUVScroll.GetAddressOf());
    }

    // BehaviorTree設定
    {
        behaviorData_ = make_unique<BehaviorData>();
        behaviorTree_ = make_unique<BehaviorTree>(this);

        behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);
        behaviorTree_->AddNode("Root", "CloseRangeAttack", 1, SelectRule::Non, new CloseRangeAttackJudgment(this), new CloseRangeAttackAction(this));
        behaviorTree_->AddNode("Root", "Pursuit",          2, SelectRule::Non, new PursuitJudgment(this),          new PursuitAction(this));
        behaviorTree_->AddNode("Root", "Idle",             3, SelectRule::Non, nullptr,                            new IdleAction(this));
    }

    SetType(static_cast<UINT>(Enemy::TYPE::Normal));

    // ImGui名前設定
    name_ = { "EnemyAI_1 : " + std::to_string(nameNum_++) };
}


// デストラクタ
EnemyAI_1::~EnemyAI_1()
{
    --nameNum_;
}


// 初期化
void EnemyAI_1::Initialize()
{
    BaseEnemyAI::Initialize();

    sword_->PlayAnimation(0, true); // デフォルトアニメーション再生再生
}


// 更新処理
void EnemyAI_1::Update(const float& elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);
}


// 描画処理
void EnemyAI_1::Render(const float& scale, ID3D11PixelShader* /*psShader*/)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // 定数バッファー更新
    UpdateConstants();

    // Aura enemy
    BaseEnemyAI::Render(scale, emissiveTextureUVScroll.Get());

    // 剣描画
    sword_->Render(scale, nullptr);

}


void EnemyAI_1::DrawDebug()
{
#ifdef USE_IMGUI

    if (ImGui::BeginMenu(GetName()))
    {
        BaseEnemyAI::DrawDebug();

        sword_->DrawDebug(); // 剣デバッグ描画

        ImGui::EndMenu();
    }

#endif // USE_IMGUI
}


void EnemyAI_1::UpdateConstants()
{
    // emissive
    {
        // emissiveの強さ
        SetEmissiveIntensity(1.5f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color
        SetEmissiveColor(DirectX::XMFLOAT4(0.11f, 0.56f, 1.00f, 1.0f));
    }
}
