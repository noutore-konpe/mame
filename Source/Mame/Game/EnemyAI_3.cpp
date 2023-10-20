#include "EnemyAI_3.h"

#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Game/ActionDerived.h"
#include "../Game/JudgmentDerived.h"

int EnemyAI_3::nameNum_ = 0;

// コンストラクタ
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


    // BehaviorTree設定
    {
        behaviorData_ = make_unique<BehaviorData>();        // ビヘイビアデータ生成
        behaviorTree_ = make_unique<BehaviorTree>(this);    // ビヘイビアツリー生成

        // ビヘイビアノード追加
        behaviorTree_->AddNode("", "Root", 0, SelectRule::Priority, nullptr, nullptr);                                                                  // 根っこ
        behaviorTree_->AddNode("Root", "BlowOff",          1, SelectRule::Non, new BlowOffJudgment(this),          new BlowOffAction(this));            // 吹っ飛び
        behaviorTree_->AddNode("Root", "Flinch",           2, SelectRule::Non, new FlinchJudgment(this),           new FlinchAction(this));             // ひるみ
        behaviorTree_->AddNode("Root", "EntryStage",       3, SelectRule::Non, new EntryStageJudgment(this),       new EntryStageAction(this));         // ステージ入場
        behaviorTree_->AddNode("Root", "LongRangeAttack",  4, SelectRule::Non, new LongRangeAttackJudgment(this),  new LongRangeAttackAction(this));    // 遠距離攻撃
        behaviorTree_->AddNode("Root", "Pursuit",          5, SelectRule::Non, new PursuitJudgment(this),          new PursuitAction(this));            // 追跡
        behaviorTree_->AddNode("Root", "Idle",             6, SelectRule::Non, nullptr,                            new IdleAction(this));               // 待機
    }

    SetType(Enemy::TYPE::Normal);

    // ImGui名前設定
    SetName("EnemyAI_3 : " + std::to_string(nameNum_++));
}

// デストラクタ
EnemyAI_3::~EnemyAI_3()
{
    --nameNum_;
}

// 初期化
void EnemyAI_3::Initialize()
{
    BaseEnemyAI::Initialize();

    constexpr float scale = 0.7f;
    this->GetTransform()->SetScale(DirectX::XMFLOAT3(scale, scale, scale));

    attackLength_ = 6.0f; // 攻撃が可能な距離をのばす

    ColliderInitialize();
}


// 更新処理
void EnemyAI_3::Update(const float& elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);

}


// 描画処理
void EnemyAI_3::Render(const float& scale, ID3D11PixelShader* /*psShader*/)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // 定数バッファー更新
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
        // emissiveの強さ
        SetEmissiveIntensity(1.25f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color
        SetEmissiveColor(DirectX::XMFLOAT4(0.00f, 0.80f, 0.81f, 1.0f));
    }

}
