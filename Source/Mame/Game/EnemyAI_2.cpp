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
            "./Resources/Shader/EnemyPS.cso",
            emissiveTextureUVScroll.GetAddressOf());
    }


    // BehaviorTree設定
    {
        behaviorData_ = make_unique<BehaviorData>();        // ビヘイビアデータ生成
        behaviorTree_ = make_unique<BehaviorTree>(this);    // ビヘイビアツリー生成

        // ビヘイビアノード追加
        behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);                                                                  // 根っこ
        behaviorTree_->AddNode("Root", "Flinch",           1, SelectRule::Non, new FlinchJudgment(this),           new FlinchAction(this));             // ひるみ
        behaviorTree_->AddNode("Root", "EntryStage",       2, SelectRule::Non, new EntryStageJudgment(this),       new EntryStageAction(this));         // ステージ入場
        behaviorTree_->AddNode("Root", "CloseRangeAttack", 3, SelectRule::Non, new CloseRangeAttackJudgment(this), new CloseRangeAttackAction(this));   // 近距離攻撃
        behaviorTree_->AddNode("Root", "Pursuit",          4, SelectRule::Non, new PursuitJudgment(this),          new PursuitAction(this));            // 追跡
        behaviorTree_->AddNode("Root", "Idle",             5, SelectRule::Non, nullptr,                            new IdleAction(this));               // 待機
    }
    SetType(Enemy::TYPE::Normal);



    // ImGui名前設定
    name_ = { "EnemyAI_2 : " + std::to_string(nameNum_++) };

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

    constexpr float scale = 1.0f;
    this->GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));
    sword_->GetTransform()->SetScale(XMFLOAT3(scale, scale, scale));

    moveSpeed_      = 2.0f;
    animationSpeed_ = 0.5f;

    sword_->PlayAnimation(0, true); // デフォルトアニメーション再生再生
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

    // 剣描画
    sword_->Render(scale, nullptr);

}


void EnemyAI_2::DrawDebug()
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


void EnemyAI_2::UpdateConstants()
{
    // emissive
    {
        // emissiveの強さ
        SetEmissiveIntensity(1.0f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color
        SetEmissiveColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
    }

}
