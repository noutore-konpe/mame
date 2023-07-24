#include "EnemyTofu.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"
#include "PlayerManager.h"

int EnemyTofu::nameNum = 0;

// コンストラクタ
EnemyTofu::EnemyTofu()
{
    Graphics& graphics = Graphics::Instance();

    model = std::make_unique<Model>(graphics.GetDevice(), "./resources/tohuall.fbx", true);
    //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhuwalk.fbx", true);

    // imgui名前かぶりが起きないように...
    name = "EnemyTofu" + std::to_string(nameNum);
    SetName(name.c_str());
    ++nameNum;
}

// デストラクタ
EnemyTofu::~EnemyTofu()
{
}

// 初期化
void EnemyTofu::Initialize()
{
    // ※ここの初期回転値によって振り向きの方向が変わるので注意
    GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(180), 0, 0));

    health      = 1;                // 体力設定
    moveSpeed_  = 1.0f;             // 移動速度設定(ステートで変わるので最初だけ)
    turnSpeed_  = ToRadian(90.0f);  // 旋回速度設定(ステートで変わるので最初だけ)
    jumpSpeed_  = 12.0f;            // ジャンプ速度設定

    characterType_ = CharacterType::Enemy;
    enemyType_ = EnemyType::Tofu;

    // ステートマシン
    {
        // 現在のX位置を移動範囲の中心に設定
        moveRangeCenterX_ = GetTransform()->GetPosition().x;

        // 移動範囲の中心から移動方向に移動範囲の半径分進んだ位置を目的地に設定
        destination_ = moveRangeCenterX_ + (moveDirectionX_ * moveRangeRadius_);

        stateMachine.reset(new StateMachine);
        GetStateMachine()->RegisterState(new TOFU::WalkState(this));
        GetStateMachine()->RegisterState(new TOFU::TurnState(this));
        GetStateMachine()->RegisterState(new TOFU::FindState(this));
        GetStateMachine()->RegisterState(new TOFU::TrackState(this));
        GetStateMachine()->RegisterState(new TOFU::IdleBattleState(this));
        GetStateMachine()->RegisterState(new TOFU::DeathState(this));
    }

    // 進行方向に向かせるために最初は旋回ステートに遷移させる
    //GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Walk));
    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Turn));
}

// 終了化
void EnemyTofu::Finalize()
{
}

// Updateの前に呼び出される
void EnemyTofu::Begin()
{
}

// 更新処理
void EnemyTofu::Update(const float& elapsedTime)
{
    // ステート更新
    if (stateMachine) GetStateMachine()->Update(elapsedTime);

    // 地形の端を超えて落ちそうになったら修正
    {
        if (lastLandingTerrainAABB_.max.x != 0.0f && aabb_.max.x > lastLandingTerrainAABB_.max.x)
        {
            const float fixLeft = -fabsf(aabb_.max.x - lastLandingTerrainAABB_.max.x);
            GetTransform()->AddPositionX(fixLeft);
        }
        else if (lastLandingTerrainAABB_.min.x != 0.0f && aabb_.min.x < lastLandingTerrainAABB_.min.x)
        {
            const float fixRight = fabsf(aabb_.min.x - lastLandingTerrainAABB_.min.x);
            GetTransform()->AddPositionX(fixRight);
        }
    }

    UpdateAABB();                       // AABBの更新処理

    UpdateVelocity(elapsedTime);        // 速力処理更新処理

    CollisionEnemyVsPlayer();           // プレイヤーとの衝突判定処理
 
    UpdateInvincibleTimer(elapsedTime); // 無敵時間更新
    
    UpdateAnimation(elapsedTime);       // アニメーション更新
}

// Updateの後に呼び出される
void EnemyTofu::End()
{
}

// 描画処理
void EnemyTofu::Render(const float& elapsedTime)
{
    Enemy::Render(elapsedTime);
}

// debug用
void EnemyTofu::DrawDebug()
{
#if USE_IMGUI
    ImGui::Begin(GetName());

    GetTransform()->DrawDebug();

    if (stateMachine)GetStateMachine()->DrawDebug();

    ImGui::End();
#endif // USE_IMGUI
}


void EnemyTofu::OnLanding()
{
    // 着地時に敵に乗っかっているかのフラグをリセットする
    isOnFriend_ = false;
}


void EnemyTofu::OnDamaged()
{
}


void EnemyTofu::OnDead()
{
    // 死亡ステートへ遷移
    GetStateMachine()->ChangeState(static_cast<int>(TOFU::STATE::Death));
}


void EnemyTofu::OnFallDead()
{
    /// 自分を消去
    Destroy();
}


// 壁に当たった時に呼ばれる処理
void EnemyTofu::OnHitWall()
{
    // 歩行ステートのときのみ反転処理を行う
    const int stateWalk         = static_cast<int>(TOFU::STATE::Walk);
    const int currentStateIndex = GetStateMachine()->GetStateIndex();
    if (currentStateIndex != stateWalk) return;

    moveDirectionX_ = -moveDirectionX_;  // 移動方向を反転

    // 旋回ステートへ遷移
    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Turn));
}

// 攻撃したときに呼ばれる処理
void EnemyTofu::OnAttacked()
{
    // 追跡中でなければreturn
    const int stateTrack        = static_cast<int>(TOFU::STATE::Track);
    const int currentStateIndex = GetStateMachine()->GetStateIndex();
    if (currentStateIndex != stateTrack) return;

    // 戦闘待機ステートへ遷移
    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::IdleBattle));
}


// 味方に当たった時に呼ばれる処理
void EnemyTofu::OnHitFriend()
{
    // 歩行ステートのときのみ反転処理を行う
    const int stateWalk         = static_cast<int>(TOFU::STATE::Walk);
    const int currentStateIndex = GetStateMachine()->GetStateIndex();
    if (currentStateIndex != stateWalk) return;

    moveDirectionX_ = -moveDirectionX_;  // 移動方向を反転

    // 旋回ステートへ遷移
    GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Turn));
}
