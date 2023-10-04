#include "EnemyGolem.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

// コンストラクタ
EnemyGolem::EnemyGolem()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/golem.fbx");

    magicCircleGolem = std::make_unique<MagicCircleGolem>();
    magicCircleEnemySummon = std::make_unique<MagicCircleEnemySummon>();

    // ImGui名前設定
    SetName("EnemyGolem" + std::to_string(nameNum++));
}

// デストラクタ
EnemyGolem::~EnemyGolem()
{
}

// 初期化
void EnemyGolem::Initialize()
{
    Enemy::Initialize();

    magicCircleGolem->Initialize();
    magicCircleEnemySummon->Initialize();

    // アニメーション再生
    Character::PlayAnimation(0, true);
}

// 終了化
void EnemyGolem::Finalize()
{
}

void EnemyGolem::Begin()
{
}

// 更新処理
void EnemyGolem::Update(const float& elapsedTime)
{
    Enemy::Update(elapsedTime);

    magicCircleGolem->Update(elapsedTime);
    magicCircleEnemySummon->Update(elapsedTime);

    // アニメーション更新
    //Character::UpdateAnimation(elapsedTime);
}

void EnemyGolem::End()
{
}

// 描画処理
void EnemyGolem::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Graphics& graphics = Graphics::Instance();

    // 定数バッファー更新
    UpdateConstants();

    Enemy::Render(scale, psShader);

    magicCircleGolem->Render();
    magicCircleEnemySummon->Render(DirectX::XMFLOAT4(0.8f, 0.44f, 0.24f, 1.0f));
}

void EnemyGolem::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

        model->skinned_meshes->Drawdebug();

        if (ImGui::Button("magicCircle"))
        {
            magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));
        }

        ImGui::EndMenu();
    }
#endif // USE_IMGUI
}

void EnemyGolem::UpdateConstants()
{
}
