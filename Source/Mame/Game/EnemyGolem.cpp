#include "EnemyGolem.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Other/MathHelper.h"

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

    {
        // ゴーレムの位置
        DirectX::XMFLOAT3 ownerPosition = GetTransform()->GetPosition();
        // ゴーレムの前ベクトル
        DirectX::XMFLOAT3 ownerFrontVec = GetTransform()->CalcForward();
        // ゴーレムの右ベクトル
        DirectX::XMFLOAT3 ownerRightVec = GetTransform()->CalcRight();
        // ゴーレムの前・右ベクトルの合計
        DirectX::XMFLOAT3 ownerVec = ownerFrontVec + ownerRightVec;

        // ベクトルの大きさ調整
        float length = 5.0f;

        // 角度
        float angle = DirectX::XMConvertToRadians(45);

        // base
        {
            {   // 一つ目
                // 方向取得
                DirectX::XMFLOAT3 setPosition = {
                    /*ownerVec.x * sinf(DirectX::XMConvertToRadians(angle)),
                    0.0f,
                    ownerVec.z * cosf(DirectX::XMConvertToRadians(angle)) };*/
                    ownerFrontVec.x * sinf(angle) + ownerRightVec.x * cosf(angle),
                    0.0f,
                    ownerFrontVec.z* sinf(angle) + ownerRightVec.z * cosf(angle) };
                // ベクトルを求める
                setPosition = setPosition * length;
                // 位置を算出
                setPosition = setPosition + ownerPosition;

                // 位置を設定
                magicCircleEnemySummon->magicCircle[0]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
            }
            {   // 二つ目  
                 // 方向取得
                DirectX::XMFLOAT3 setPosition = {
                    /*-ownerVec.x * sinf(angle),
                    0.0f,
                    ownerVec.z * cosf(angle) };*/
                    ownerFrontVec.x* sinf(angle) - ownerRightVec.x * cosf(angle),
                    0.0f,
                    ownerFrontVec.z* sinf(angle) - ownerRightVec.z * cosf(angle) };
                // ベクトルを求める
                setPosition = setPosition * length;
                // 位置を算出
                setPosition = setPosition + ownerPosition;

                // 位置を設定
                magicCircleEnemySummon->magicCircle[1]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
            }
        }
        // move
        {
            // 一つ目
            {
                for (int i = 0; i < 3; ++i)
                {
                    float magicCirclePosY = magicCircleEnemySummon->magicCircleMove[0][i]->GetTransform()->GetPosition().y;
                    // 方向取得
                    DirectX::XMFLOAT3 setPosition = {
                        ownerVec.x * sinf(DirectX::XMConvertToRadians(angle)),
                        0.0f,
                        ownerVec.z * cosf(DirectX::XMConvertToRadians(angle)) };
                    // ベクトルを求める
                    setPosition = setPosition * length;
                    // 位置を算出
                    setPosition = setPosition + ownerPosition;
                    setPosition.y = magicCirclePosY;

                    // 位置を設定
                    magicCircleEnemySummon->magicCircleMove[0][i]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
                }
            }

            // 二つ目
            {
                for (int i = 0; i < 3; ++i)
                {
                    float magicCirclePosY = magicCircleEnemySummon->magicCircleMove[0][i]->GetTransform()->GetPosition().y;
                    // 方向取得
                    DirectX::XMFLOAT3 setPosition = {
                        -ownerVec.x * sinf(DirectX::XMConvertToRadians(angle)),
                        0.0f,
                        ownerVec.z * cosf(DirectX::XMConvertToRadians(angle)) };
                    // ベクトルを求める
                    setPosition = setPosition * length;
                    // 位置を算出
                    setPosition = setPosition + ownerPosition;
                    setPosition.y = magicCirclePosY;

                    // 位置を設定
                    magicCircleEnemySummon->magicCircleMove[1][i]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
                }
            }
        }
    }

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
