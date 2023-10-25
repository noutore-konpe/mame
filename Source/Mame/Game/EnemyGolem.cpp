#include "EnemyGolem.h"
#include "../Graphics/Graphics.h"
#include "../Resource/texture.h"

#include "../Other/MathHelper.h"

#include "EnemyGolemState.h"

#include "../Scene/SceneGame.h"

#include "EnemyManager.h"
#include "PlayerManager.h"

#include "Collision.h"

#include "../Resource/texture.h"
#include "../Other/misc.h"

#include "../Resource/AudioManager.h"

int EnemyGolem::nameNum_;


// コンストラクタ
EnemyGolem::EnemyGolem()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(),
        "./Resources/Model/Character/Enemy/golem.fbx");

    CreatePsFromCso(graphics.GetDevice(),
        "./Resources/Shader/GolemPS.cso",
        golemPS.GetAddressOf());

    D3D11_TEXTURE2D_DESC texture2dDesc;
    ::load_texture_from_file(graphics.GetDevice(),
        L"./Resources/Model/Character/Enemy/golem_emissive.png",
        golemEmissive.GetAddressOf(),
        &texture2dDesc);
    ::load_texture_from_file(graphics.GetDevice(),
        L"./Resources/Model/Character/Enemy/golem_eye_emissive.png",
        eyeEmissive.GetAddressOf(),
        &texture2dDesc);
    ::load_texture_from_file(graphics.GetDevice(),
        L"./Resources/Model/Character/Enemy/golem_normal.png",
        golemNormal.GetAddressOf(),
        &texture2dDesc);

    {
        HRESULT hr{ S_OK };

        D3D11_BUFFER_DESC bufferDesc{};

        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

        bufferDesc.ByteWidth = sizeof(EyeConstants);
        // CBParametric
        hr = graphics.GetDevice()->CreateBuffer(&bufferDesc, nullptr,
            eyeConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
    }

    magicCircleGolem = std::make_unique<MagicCircleGolem>();
    magicCircleEnemySummon = std::make_unique<MagicCircleEnemySummon>();
    magicCircleGolemAttack2 = std::make_unique<MagicCircleGolemAttack2>(this);
    comboAttackStone = std::make_unique<ComboAttackStone>(this);

    for (int i = 0; i < 3; ++i)
    {
        comboAttackStones[i] = std::make_unique<ComboAttackStone>(this);
    }

    // ステートマシン
    {
        stateMachine.reset(new StateMachine<State<EnemyGolem>>);

        GetStateMachine()->RegisterState(new EnemyGolemState::DummyState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::IdleState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::EntryState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::RoarState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::SummonState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::GetUpState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::Attack1State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::ComboAttack1State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::DownState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::ComboAttack2State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::ChoseState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::DeathState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::WalkState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::Attack2State(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::DelayState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::EyeEmissiveDownState(this));
        GetStateMachine()->RegisterState(new EnemyGolemState::EyeEmissiveUpState(this));

        GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::DummyState));
    }

    SetType(Enemy::TYPE::Golem);

    // ImGui名前設定
    SetName("EnemyGolem" + std::to_string(nameNum_++));

    //身長設定
    lockOnHeight = 2.0f;
}

// デストラクタ
EnemyGolem::~EnemyGolem()
{
    if (isDead)
    {
        EnemyManager::Instance().AddEnemyGolemKillNum();


        PlayerManager::Instance().isChangeBGM = true;
    }

    PlayerManager::Instance().isChange = true;
    
    //PlayerManager::Instance().SetFogTimer(0.0f);
}

// 初期化
void EnemyGolem::Initialize()
{
    Enemy::Initialize();

    magicCircleGolem->Initialize();
    magicCircleEnemySummon->Initialize();
    magicCircleGolemAttack2->Initialize();

    comboAttackStone->Initialize();

    for (int i = 0; i < 3; ++i)
    {
        comboAttackStones[i]->Initialize();
    }

    //GetTransform()->SetPositionY(10.0f);

    GetStateMachine()->SetState(static_cast<UINT>(StateMachineState::EntryState));

    // アニメーション再生
    Character::PlayAnimation(static_cast<UINT>(Animation::Landing), true);

    currentState = static_cast<UINT>(StateMachineState::EntryState);

    // ステージに入場済み扱いにする
    entryStageFlag_ = true;

    //判定設定
    ColliderInitialize();

    //押し出し判定
    radius_ = 1.0f;

#ifdef _DEBUG
    currentStateDebug = 0;
#endif // _DEBUG

    AudioManager::Instance().StopBGM(BGM::Game);
    AudioManager::Instance().PlayBGM(BGM::Golem);

    AudioManager::Instance().GetBGM(BGM::Golem)->Volume(0.35f);
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
    PlayerManager::Instance().isChange = true;

    // ステートマシン更新
    GetStateMachine()->Update(elapsedTime);

    Enemy::Update(elapsedTime);

    magicCircleGolem->Update(elapsedTime);
    magicCircleEnemySummon->Update(elapsedTime);
    magicCircleGolemAttack2->Update(elapsedTime);
    comboAttackStone->SetOwnerTransform(GetTransform());
    comboAttackStone->Update(elapsedTime);

    for (int i = 0; i < 3; ++i)
    {
        comboAttackStones[i]->Update(elapsedTime);
        comboAttackStones[i]->SetOwnerTransform(GetTransform());
    }

    UpdateSummoningMagicCircle(4.0f, 3.0f, DirectX::XMConvertToRadians(45));
    UpdateAttack2MagicCircle(2.0f, 3.5f);

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

    // 必要な時だけ描画する
    SubRender();

    //magicCircleGolemAttack2->Render(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));

#if _DEBUG
    if (SceneGame::isDispCollision_)
    {
        for (auto& collider : hitCollider)
        {
            collider.DebugRender();
        }

        for (auto& collider : attackCollider)
        {
            collider.DebugRender(DirectX::XMFLOAT4(1, 0, 0, 1));
        }
    }
#endif

    ColliderPosUpdate(scale);//判定位置を各ジョイントに追従
}

// 影描画用
void EnemyGolem::Render(const float& scale, bool shadow, ID3D11PixelShader* psShader)
{
    // 定数バッファー更新
    UpdateConstants();

    model->Render(scale, golemPS.Get());
}

void EnemyGolem::DrawDebug()
{
#ifdef USE_IMGUI
    if (ImGui::BeginMenu(GetName()))
    {
        ImGui::Begin("golemColor");
        ImGui::ColorEdit4("c", &golemColor.x);
        ImGui::End();
        ImGui::Begin("eye");
        ImGui::ColorEdit4("eyecolor", &eyeConstants.emissiveColor.x);
        ImGui::DragFloat("eyeIntensity", &eyeConstants.emissiveIntensity);
        ImGui::End();

        Character::DrawDebug();

        model->skinned_meshes->Drawdebug();

        magicCircleGolem->DrawDebug();
        magicCircleEnemySummon->DrawDebug();

        comboAttackStone->DrawDebug();

        ImGui::SliderInt("currentState", &currentStateDebug, 0, 10);

        if (ImGui::Button(stateName[currentStateDebug]))
        {
            switch (currentStateDebug)
            {
            case 0:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::DummyState));
                break;
            case 1:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::EntryState));
                break;
            case 2:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::SummonState));
                break;
            case 3:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::RoarState));
                break;
            case 4:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::Attack1State));
                break;
            case 5:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::ComboAttack1State));
                break;
            case 6:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::DownState));
                break;
            case 7:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::ComboAttack2State));
                break;
            case 8:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::DeathState));
                break;
            case 9:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::WalkState));
                break;
            case 10:
                GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::Attack2State));
                break;
            }
        }

        if (ImGui::Button("magicCircle"))
        {
            magicCircleEnemySummon->GetStateMachine()->ChangeState(static_cast<UINT>(MagicCircleEnemySummon::StateMachineState::AppearState));
        }

        ImGui::Begin("Collider");
        
        ImGui::SliderFloat("hip", &hitCollider[static_cast<float>(ColliderName::HIP)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("R_leg", &hitCollider[static_cast<float>(ColliderName::R_LEG)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("L_leg", &hitCollider[static_cast<float>(ColliderName::L_LEG)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("R_knee", &hitCollider[static_cast<float>(ColliderName::R_LEG_END)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("L_knee", &hitCollider[static_cast<float>(ColliderName::L_LEG_END)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("R_shoulder", &hitCollider[static_cast<float>(ColliderName::R_SHOULDER)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("L_shoulder", &hitCollider[static_cast<float>(ColliderName::L_SHOULDER)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("R_hand", &hitCollider[static_cast<float>(ColliderName::R_HAND)].radius, 0.0f, 2.0f);
        ImGui::SliderFloat("L_hand", &hitCollider[static_cast<float>(ColliderName::L_HAND)].radius, 0.0f, 2.0f);

        ImGui::End();
        

        ImGui::EndMenu();
    }
    ImGui::Separator();

#endif // USE_IMGUI
}

void EnemyGolem::UpdateConstants()
{
    Graphics& graphics = Graphics::Instance();

    // emissiveの強さ
    SetEmissiveIntensity(1.0f);

    // color
    SetEmissiveColor(golemColor);
    SetEmissiveColor(DirectX::XMFLOAT4(0.7f, 0.0f, 1.0f, 1.0f));
    ID3D11ShaderResourceView* shaderResourceViews[] =
    {
        golemEmissive.Get(),
        eyeEmissive.Get(),
    };
    graphics.GetDeviceContext()->PSSetShaderResources(16, _countof(shaderResourceViews), shaderResourceViews);
    graphics.GetDeviceContext()->PSSetShaderResources(14, 1, golemNormal.GetAddressOf());
    
    graphics.GetDeviceContext()->UpdateSubresource(eyeConstantBuffer.Get(), 0, 0, &eyeConstants, 0, 0);
    graphics.GetDeviceContext()->PSSetConstantBuffers(7, 1, eyeConstantBuffer.GetAddressOf());

}

void EnemyGolem::OnDead(DamageResult result)
{
    GetStateMachine()->ChangeState(static_cast<UINT>(StateMachineState::DeathState));
}

void EnemyGolem::SubRender()
{
    switch (currentState)
    {
    case static_cast<UINT>(StateMachineState::SummonState):
        // 召喚魔法
        magicCircleEnemySummon->Render(DirectX::XMFLOAT4(magicCircleColor[0]));
        break;
    case static_cast<UINT>(StateMachineState::ComboAttack1State):
        // コンボ攻撃１
        comboAttackStone->Render();
        break;
    case static_cast<UINT>(StateMachineState::ComboAttack2State):
        // コンボ攻撃２
        for (int i = 0; i < 3; ++i)
        {
            comboAttackStones[i]->Render();
        }
        break;
    }
    
    if (magicCircleGolemAttack2->isAttack2)
    {
        // 攻撃２
        magicCircleGolemAttack2->Render(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
    }
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

// 攻撃魔法陣更新処理
void EnemyGolem::UpdateAttack2MagicCircle(const float& lengthX, const float& lengthZ)
{
    // ゴーレムの位置
    DirectX::XMFLOAT3 ownerPosition = GetTransform()->GetPosition();
    // ゴーレムの前ベクトル
    DirectX::XMFLOAT3 ownerFrontVec = GetTransform()->CalcForward();
    // ゴーレムの右ベクトル
    DirectX::XMFLOAT3 ownerRightVec = GetTransform()->CalcRight();
    

    // 真ん中の魔法陣
    DirectX::XMFLOAT3 centerPos =
    {
        ownerPosition.x + ownerFrontVec.x * 2.0f,
        ownerPosition.y + 2.0f,
        ownerPosition.z + ownerFrontVec.z * 2.0f
    };

    DirectX::XMFLOAT3 centerRightVec = 
        magicCircleGolemAttack2->baseMagicCircle[static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Center)]->GetTransform()->CalcRight();

    float direction = 2.0f;
    DirectX::XMFLOAT3 sidePos[2] =
    {
        {
            centerPos.x + centerRightVec.x * direction,
            centerPos.y + centerRightVec.y,
            centerPos.z + centerRightVec.z * direction
        },
        {
            centerPos.x - centerRightVec.x * direction,
            centerPos.y - centerRightVec.y,
            centerPos.z - centerRightVec.z * direction
        }
    };

    magicCircleGolemAttack2->baseMagicCircle[static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Center)]->GetTransform()->SetPosition(centerPos);
    magicCircleGolemAttack2->baseMagicCircle[static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Right)]->GetTransform()->SetPosition(sidePos[0]);
    magicCircleGolemAttack2->baseMagicCircle[static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Left)]->GetTransform()->SetPosition(sidePos[1]);
    
    for (int i = 0; i < static_cast<UINT>(MagicCircleGolemAttack2::MAGIC_CIRCLE::Max); ++i)
    {
        magicCircleGolemAttack2->baseMagicCircle[i]->GetTransform()->
            SetRotationY(GetTransform()->GetRotation().y);
    }
}

void EnemyGolem::ColliderInitialize()
{
    //初期化
    if (hitCollider.size() == 0)
    {
        for (int i = 0; i < static_cast<int>(ColliderName::END); i++)
        {
            attackCollider.emplace_back(SphereCollider(0.8f));
        }

        for (int i = 0; i < static_cast<int>(ColliderName::END); i++)
        {
            hitCollider.emplace_back(SphereCollider(0.55f));
        }
    }
    const float scale = GetTransform()->GetScale().x;
    /*hitCollider[static_cast<int>(ColliderName::NECK)].radius = 0.15f * scale;
    hitCollider[static_cast<int>(ColliderName::HIP)].radius = 0.17f * scale;
    hitCollider[static_cast<int>(ColliderName::R_LEG)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::L_LEG)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::R_LEG_END)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::L_LEG_END)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::R_ELBOW)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::L_ELBOW)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::R_HAND)].radius = 0.12f * scale;
    hitCollider[static_cast<int>(ColliderName::L_HAND)].radius = 0.12f * scale;*/
}

void EnemyGolem::ColliderPosUpdate(const float& scale)
{
    hitCollider[static_cast<int>(ColliderName::HIP)].position           = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface11", "ref_E:ref_E:j_Spine", scale);
    hitCollider[static_cast<int>(ColliderName::R_SHOULDER)].position    = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface6",  "ref_E:ref_E:j_RightArm", scale);
    hitCollider[static_cast<int>(ColliderName::L_SHOULDER)].position    = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface5",  "ref_E:ref_E:j_LeftArm", scale);
    hitCollider[static_cast<int>(ColliderName::R_HAND)].position        = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface3",  "ref_E:ref_E:j_RightHand", scale);
    hitCollider[static_cast<int>(ColliderName::L_HAND)].position        = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface1",  "ref_E:ref_E:j_LeftHand", scale);
    hitCollider[static_cast<int>(ColliderName::R_LEG)].position         = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface11", "ref_E:ref_E:j_RightUptLeg", scale);
    hitCollider[static_cast<int>(ColliderName::L_LEG)].position         = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface11", "ref_E:ref_E:j_LeftUptLeg", scale);
    hitCollider[static_cast<int>(ColliderName::R_LEG_END)].position     = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface9",  "ref_E:ref_E:j_RighttLeg", scale);
    hitCollider[static_cast<int>(ColliderName::L_LEG_END)].position     = GetJointPosition("ref_E:ref_E:golem_mdl_1023:polySurface8",  "ref_E:ref_E:j_LefttLeg", scale);

    for (int i = 0; i < static_cast<int>(ColliderName::END); i++)
    {
        attackCollider[i].position = hitCollider[i].position;
    }
}

const bool EnemyGolem::AttackCollisionVsPlayer(ColliderName index,float damage)
{
    auto* player = PlayerManager::Instance().GetPlayer().get();
    auto myHitCollider = hitCollider.at(static_cast<int>(index));
    for  (auto& pHitCollider : player->GetHitCollider())
    {
        if (Collision::IntersectSphereVsSphere(
            pHitCollider.position, pHitCollider.radius,
            myHitCollider.position, myHitCollider.radius))
        {
            player->ApplyDamage(damage, pHitCollider.position,Player::HitReaction::HARD,this,0.01f);
            return true;
        }
    }

    return false;
}
