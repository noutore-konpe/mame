#include "EnemyGolem.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Other/MathHelper.h"

#include "EnemyGolemState.h"

// コンストラクタ
EnemyGolem::EnemyGolem()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/golem.fbx");

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/playerPS.cso",
        golemPS.GetAddressOf());

    magicCircleGolem = std::make_unique<MagicCircleGolem>();
    magicCircleEnemySummon = std::make_unique<MagicCircleEnemySummon>();
    comboAttackStone = std::make_unique<ComboAttackStone>();


    // ステートマシン
    {
        stateMachine.reset(new StateMachine<State<EnemyGolem>>);

        GetStateMachine()->RegisterState(new EnemyGolemState::IdleState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::EntryState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::RoarState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::SummonState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::GetUpState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::Attack1State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::ComboAttack1State(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::IdleState));
    }

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

    comboAttackStone->Initialize();

    GetTransform()->SetPositionY(10.0f);

    // アニメーション再生
    Character::PlayAnimation(static_cast<UINT>(Animation::Idle), true);

    currentState = static_cast<UINT>(StateMachineState::IdleState);

#ifdef _DEBUG
    currentStateDebug = 0;
#endif // _DEBUG
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
    comboAttackStone->SetOwnerTransform(GetTransform());
    comboAttackStone->Update(elapsedTime);

    UpdateSummoningMagicCircle(4.0f, 3.0f, DirectX::XMConvertToRadians(45));

    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);

    // アニメーション更新
    Character::UpdateAnimation(elapsedTime);
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

    //Enemy::Render(scale, psShader);
    Enemy::Render(scale, golemPS.Get());

    magicCircleGolem->Render();
    magicCircleEnemySummon->Render(DirectX::XMFLOAT4(magicCircleColor[2]));
    comboAttackStone->Render();

    if (currentState == static_cast<UINT>(StateMachineState::ComboAttack1State))
    {
    }
}

// 影描画用
void EnemyGolem::Render(const float& scale, bool shadow, ID3D11PixelShader* psShader)
{
    // 定数バッファー更新
    UpdateConstants();

    Enemy::Render(scale, psShader);
}

void EnemyGolem::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        Character::DrawDebug();

        model->skinned_meshes->Drawdebug();

        magicCircleGolem->DrawDebug();
        magicCircleEnemySummon->DrawDebug();

        comboAttackStone->DrawDebug();

        ImGui::SliderInt("currentState", &currentStateDebug, 0, 4);

        if (ImGui::Button(stateName[currentStateDebug]))
        {
            switch (currentStateDebug)
            {
            case 0:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::EntryState));
                break;
            case 1:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::SummonState));
                break;
            case 2:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::RoarState));
                break;
            case 3:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::Attack1State));
                break;
            case 4:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::ComboAttack1State));
                break;
            }
        }

        if (ImGui::Button("magicCircle"))
        {
            magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));
        }

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

void EnemyGolem::UpdateConstants()
{
}

// 召喚魔法陣更新処理
void EnemyGolem::UpdateSummoningMagicCircle(const float& lengthX, const float& lengthZ, const float& angle)
{
    // ゴーレムの位置
    DirectX::XMFLOAT3 ownerPosition = GetTransform()->GetPosition();
    // ゴーレムの前ベクトル
    DirectX::XMFLOAT3 ownerFrontVec = GetTransform()->CalcForward();
    // ゴーレムの右ベクトル
    DirectX::XMFLOAT3 ownerRightVec = GetTransform()->CalcRight();

    // base
    {
        {   // 一つ目
            // 方向取得
            DirectX::XMFLOAT3 setPosition = {
                ownerFrontVec.x * sinf(angle) + ownerRightVec.x * cosf(angle),
                0.0f,
                ownerFrontVec.z * sinf(angle) + ownerRightVec.z * cosf(angle) };
            // ベクトルを求める
            setPosition = { setPosition.x * lengthX, 0.0f, setPosition.z * lengthZ };
            // 位置を算出
            setPosition = setPosition + ownerPosition;

            // 位置を設定
            magicCircleEnemySummon->magicCircle[0]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
        }
        {   // 二つ目
             // 方向取得
            DirectX::XMFLOAT3 setPosition = {
                ownerFrontVec.x * sinf(angle) - ownerRightVec.x * cosf(angle),
                0.0f,
                ownerFrontVec.z * sinf(angle) - ownerRightVec.z * cosf(angle) };
            // ベクトルを求める
            setPosition = { setPosition.x * lengthX, 0.0f, setPosition.z * lengthZ };
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
                ownerFrontVec.x* sinf(angle) + ownerRightVec.x * cosf(angle),
                0.0f,
                ownerFrontVec.z* sinf(angle) + ownerRightVec.z * cosf(angle) };
                // ベクトルを求める
                setPosition = { setPosition.x * lengthX, 0.0f, setPosition.z * lengthZ };
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
                ownerFrontVec.x* sinf(angle) - ownerRightVec.x * cosf(angle),
                0.0f,
                ownerFrontVec.z* sinf(angle) - ownerRightVec.z * cosf(angle) };
                // ベクトルを求める
                setPosition = { setPosition.x * lengthX, 0.0f, setPosition.z * lengthZ };
                // 位置を算出
                setPosition = setPosition + ownerPosition;
                setPosition.y = magicCirclePosY;

                // 位置を設定
                magicCircleEnemySummon->magicCircleMove[1][i]->GetTransform()->SetPosition(DirectX::XMFLOAT3(setPosition));
            }
        }
    }
}
