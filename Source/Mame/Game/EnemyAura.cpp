#include "EnemyAura.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

int EnemyAura::nameNum_ = 0;

// コンストラクタ
EnemyAura::EnemyAura()
{
    using std::make_unique;
    using std::to_string;
    using SelectRule = BehaviorTree::SelectRule;

    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/testModel/plantune.fbx");

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

    behaviorData_ = make_unique<BehaviorData>();
    behaviorTree_ = make_unique<BehaviorTree>(this);

    behaviorTree_->AddNode("", "Root", 0, BehaviorTree::SelectRule::Priority, nullptr, nullptr);

    // ImGui名前設定
    SetName("AuraEnemy" + std::to_string(nameNum_++));
}

// デストラクタ
EnemyAura::~EnemyAura()
{
}

// 初期化
void EnemyAura::Initialize()
{
    BaseEnemyAI::Initialize();

    // アニメーション再生
    Character::PlayAnimation(0, true);
}

// 終了化
void EnemyAura::Finalize()
{
}

// Updateの前に呼ばれる
void EnemyAura::Begin()
{
}

// 更新処理
void EnemyAura::Update(const float& elapsedTime)
{
    BaseEnemyAI::Update(elapsedTime);

    // アニメーション更新
    Character::UpdateAnimation(elapsedTime);
}

// Updateの後に呼ばれる
void EnemyAura::End()
{
}

// 描画処理
void EnemyAura::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();

    // emissiveTexture Set
    graphics.GetDeviceContext()->PSSetShaderResources(16, 1, emissiveTexture.GetAddressOf());

    // 定数バッファー更新
    UpdateConstants();

    // Aura enemy
    BaseEnemyAI::Render(scale, emissiveTextureUVScroll.Get());
}

void EnemyAura::DrawDebug()
{
}

void EnemyAura::UpdateConstants()
{
    // emissive
    {
        // emissiveの強さ
        SetEmissiveIntensity(1.5f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color とりあえず赤
        SetEmissiveColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    }
}
