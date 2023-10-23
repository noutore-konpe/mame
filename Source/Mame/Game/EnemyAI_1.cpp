#include "EnemyAI_1.h"

#include "../../Taki174/NumeralManager.h"
#include "../../Taki174/Common.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "ActionDerived.h"
#include "JudgmentDerived.h"

#include "EnemyManager.h"

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
            "./Resources/Shader/EnemyPS.cso",
            emissiveTextureUVScroll.GetAddressOf());
    }

    // BehaviorTree設定
    {
        behaviorData_ = make_unique<BehaviorData>();        // ビヘイビアデータ生成
        behaviorTree_ = make_unique<BehaviorTree>(this);    // ビヘイビアツリー生成

        // ビヘイビアノード追加
        behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);                                                                  // 根っこ
        behaviorTree_->AddNode("Root", "BlowOff",          1, SelectRule::Non, new BlowOffJudgment(this),          new BlowOffAction(this));            // 吹っ飛び
        behaviorTree_->AddNode("Root", "Flinch",           2, SelectRule::Non, new FlinchJudgment(this),           new FlinchAction(this));             // ひるみ
        behaviorTree_->AddNode("Root", "EntryStage",       3, SelectRule::Non, new EntryStageJudgment(this),       new EntryStageAction(this));         // ステージ入場
        behaviorTree_->AddNode("Root", "CloseRangeAttack", 4, SelectRule::Non, new CloseRangeAttackJudgment(this), new CloseRangeAttackAction(this));   // 近距離攻撃
        behaviorTree_->AddNode("Root", "Pursuit",          5, SelectRule::Non, new PursuitJudgment(this),          new PursuitAction(this));            // 追跡
        behaviorTree_->AddNode("Root", "Idle",             6, SelectRule::Non, nullptr,                            new IdleAction(this));               // 待機
    }

    SetType(Enemy::TYPE::Normal);

    // ImGui名前設定
    name_ = { "EnemyAI_1 : " + std::to_string(nameNum_++) };
}


// デストラクタ
EnemyAI_1::~EnemyAI_1()
{
    --nameNum_;
    if (isDead)
        EnemyManager::Instance().AddEnemy1KillNum();
}


// 初期化
void EnemyAI_1::Initialize()
{
    BaseEnemyAI::Initialize();

    constexpr float scale = 0.7f;
    this->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));
    sword_->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

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

    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());
    // 剣描画
    sword_->Render(scale, emissiveTextureUVScroll.Get());

    BaseEnemyAI::ColliderPosUpdate(scale);

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
        SetEmissiveIntensity(1.25f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color
        SetEmissiveColor(DirectX::XMFLOAT4(0.11f, 0.56f, 1.00f, 1.0f));
    }
}
