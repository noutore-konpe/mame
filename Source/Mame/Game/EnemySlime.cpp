#include "EnemySlime.h"
#include "../Graphics/Graphics.h"

int EnemySlime::nameNum_;

// コンストラクタ
EnemySlime::EnemySlime()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        //"./Resources/Model/sanaModel/mameoall.fbx");
        //"./Resources/Model/testModel/character.fbx");
        //"./Resources/Model/Cube.fbx");
        //"./Resources/Model/yoshiakiModel/Cube.fbx");
        "./Resources/Model/testModel/plantune.fbx");
        //"./Resources/Model/testModel/latha.fbx");
        //"./Resources/Model/Collision/sqhere.fbx");

    // ImGui名前設定
    SetName("EnemySlime" + std::to_string(nameNum_++));
}

// デストラクタ
EnemySlime::~EnemySlime()
{
}

// 初期化
void EnemySlime::Initialize()
{
    Enemy::Initialize();

    Character::PlayAnimation(0, true);
}

// 終了化
void EnemySlime::Finalize()
{
}

// Updateの前に呼ばれる
void EnemySlime::Begin()
{
}

// 更新処理
void EnemySlime::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    Character::UpdateAnimation(elapsedTime);
}

// Updateの後に呼ばれる
void EnemySlime::End()
{
}

// 描画処理
void EnemySlime::Render(const float& scale, ID3D11PixelShader* psShader)
{
    // 定数更新
    UpdateConstants();

    Enemy::Render(scale, psShader);
}

// ImGui用
void EnemySlime::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

        float range = GetRange();
        ImGui::DragFloat("range", &range);
        SetRange(range);

        model->skinned_meshes->Drawdebug();

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

void EnemySlime::UpdateConstants()
{
    // emissive
    {
        // emissiveの強さ
        SetEmissiveIntensity(1.5f);

        // emissiveTexture ScrollDirection
        SetEmissiveScrollDirection(DirectX::XMFLOAT2(0.25f, 0.5f));

        // color とりあえず赤
        SetEmissiveColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
    }
}
