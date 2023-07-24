#include "EnemyRedTofu.h"

#include "../Mame/Graphics/Graphics.h"

#include "BossStateDerived.h"
#include "PlayerManager.h"
#include "EnemyTofu.h"

namespace RED_TOFU
{
    //int EnemyRedTofu::nameNum = 0;

    // コンストラクタ
    EnemyRedTofu::EnemyRedTofu()
    {
        Graphics& graphics = Graphics::Instance();

        model = std::make_unique<Model>(graphics.GetDevice(), "./resources/tohuall.fbx", true);
        //model = std::make_unique<Model>(graphics.GetDevice(), "./resources/touhuwalk.fbx", true);

        // imgui名前かぶりが起きないように...
        name = "EnemyTofu" + std::to_string(EnemyTofu::nameNum);    // 白豆腐のnaneNumを使う
        SetName(name.c_str());
        ++EnemyTofu::nameNum;
    }

    // デストラクタ
    EnemyRedTofu::~EnemyRedTofu()
    {
    }

    // 初期化
    void EnemyRedTofu::Initialize()
    {
        // ※ここの初期回転値によって振り向きの方向が変わるので注意
        GetTransform()->SetRotation(DirectX::XMFLOAT4(0, ToRadian(180), 0, 0));

        modelColor = DirectX::XMFLOAT4(1.0f, 0.25f, 0.25f, 1.0f); // 赤色に設定

        health      = 1;                // 体力設定
        moveSpeed_  = 2.0f;             // 移動速度設定(ステートで変わるので最初だけ)
        turnSpeed_  = ToRadian(180.0f); // 旋回速度設定(ステートで変わるので最初だけ)
        jumpSpeed_  = 12.0f;            // ジャンプ速度設定

        moveRangeRadius_ = 8.0f;        // 移動範囲の半径
        searchLengthX_   = 6.0f;        // X軸で索敵できる距離
        searchLengthY_   = 6.0f;        // Y軸で索敵できる距離

        characterType_ = CharacterType::Enemy;
        enemyType_ = EnemyType::RedTofu;    // 敵の種類を設定

        // ステートマシン
        {
            // 現在のX位置を移動範囲の中心に設定
            moveRangeCenterX_ = GetTransform()->GetPosition().x;

            // 移動範囲の中心から移動方向に移動範囲の半径分進んだ位置を目的地に設定
            destination_ = moveRangeCenterX_ + (moveDirectionX_ * moveRangeRadius_);

            stateMachine.reset(new StateMachine);
            GetStateMachine()->RegisterState(new WalkState(this));
            GetStateMachine()->RegisterState(new TurnState(this));
            GetStateMachine()->RegisterState(new FindState(this));
            GetStateMachine()->RegisterState(new TrackState(this));
            GetStateMachine()->RegisterState(new IdleBattleState(this));
            GetStateMachine()->RegisterState(new DeathState(this));
        }

        // 進行方向に向かせるために最初は旋回ステートに遷移させる
        //GetStateMachine()->SetState(static_cast<int>(TOFU::STATE::Walk));
        GetStateMachine()->SetState(static_cast<int>(STATE::Turn));
    }

    // 終了化
    void EnemyRedTofu::Finalize()
    {
    }

    // Updateの前に呼び出される
    void EnemyRedTofu::Begin()
    {
    }

    // 更新処理
    void EnemyRedTofu::Update(const float& elapsedTime)
    {
        // ステート更新
        if (stateMachine) GetStateMachine()->Update(elapsedTime);

        //// 地形の端を超えて落ちそうになったら修正
        //{
        //    if (lastLandingTerrainAABBMaxX != 0.0f && aabb_.max.x > lastLandingTerrainAABBMaxX)
        //    {
        //        const float fixLeft = -fabsf(aabb_.max.x - lastLandingTerrainAABBMaxX);
        //        GetTransform()->AddPositionX(fixLeft);
        //    }
        //    else if (lastLandingTerrainAABBMinX != 0.0f && aabb_.min.x < lastLandingTerrainAABBMinX)
        //    {
        //        const float fixRight = fabsf(aabb_.min.x - lastLandingTerrainAABBMinX);
        //        GetTransform()->AddPositionX(fixRight);
        //    }
        //}

        UpdateAABB();                       // AABBの更新処理

        UpdateVelocity(elapsedTime);        // 速力処理更新処理

        CollisionEnemyVsPlayer();           // プレイヤーとの衝突判定処理

        UpdateInvincibleTimer(elapsedTime); // 無敵時間更新

        UpdateAnimation(elapsedTime);       // アニメーション更新
    }

    // Updateの後に呼び出される
    void EnemyRedTofu::End()
    {
    }

    // 描画処理
    void EnemyRedTofu::Render(const float& elapsedTime)
    {
        Enemy::Render(elapsedTime);
    }

    // debug用
    void EnemyRedTofu::DrawDebug()
    {
#if USE_IMGUI
        ImGui::Begin(GetName());

        GetTransform()->DrawDebug();

        if (stateMachine)GetStateMachine()->DrawDebug();

        ImGui::End();
#endif // USE_IMGUI
    }


    void EnemyRedTofu::OnLanding()
    {
        // 着地時に敵に乗っかっているかのフラグをリセットする
        isOnFriend_ = false;
    }


    void EnemyRedTofu::OnDamaged()
    {
    }


    void EnemyRedTofu::OnDead()
    {
        // プレイヤーの移動方向保存ベクトルを取得
        const float plSaveMoveVecX = PlayerManager::Instance().GetPlayer()->GetSaveMoveVecX();
        const float length = fabsf(plSaveMoveVecX);
        const float plSaveMoveVecX_n = (length > 0.0f) ? (plSaveMoveVecX / length) : 1.0f; // 単位ベクトル化(ゼロベクトルの予防込み)

        moveDirectionX_ = plSaveMoveVecX_n; // プレイヤーの攻撃方向に吹っ飛ぶようにする

        GetStateMachine()->ChangeState(static_cast<int>(STATE::Death));
    }


    void EnemyRedTofu::OnFallDead()
    {
        /// 自分を消去
        Destroy();
    }


    // 壁に当たった時に呼ばれる処理
    void EnemyRedTofu::OnHitWall()
    {
        const int stateWalk         = static_cast<int>(STATE::Walk);
        const int stateTrack        = static_cast<int>(STATE::Track);
        const int currentStateIndex = GetStateMachine()->GetStateIndex();

        // 歩行ステートのときは旋回ステートへ遷移
        if (currentStateIndex == stateWalk)
        {
            moveDirectionX_ = -moveDirectionX_;  // 移動方向を反転

            // 旋回ステートへ遷移
            GetStateMachine()->SetState(static_cast<int>(STATE::Turn));
            return;
        }
#if 0 // 壁があったら飛び越えようとする
        // 追跡ステートのときは地面についていたらジャンプさせる
        else if (currentStateIndex == stateTrack && isGround_)
        {         
            Jump(jumpSpeed_ * 1.5f);
            return;
        }        
#else // 壁をよじ登る
        // 追跡ステートのときは壁をよじ登る(落下中・登れないタイプの地形なら登れない)
        else if (currentStateIndex == stateTrack && velocity.y >= 0.0f &&
            saveWall_->terrainType_ != Terrain::Type::NoClimb)
        {         
            Jump(jumpSpeed_ * 0.5f);
            return;
        }
#endif
    }

    // 攻撃したときに呼ばれる処理
    void EnemyRedTofu::OnAttacked()
    {
        // 追跡中でなければreturn
        const int stateTrack = static_cast<int>(STATE::Track);
        const int currentStateIndex = GetStateMachine()->GetStateIndex();
        if (currentStateIndex != stateTrack) return;

        // 戦闘待機ステートへ遷移
        GetStateMachine()->SetState(static_cast<int>(STATE::IdleBattle));
    }


    // 味方に当たった時に呼ばれる処理
    void EnemyRedTofu::OnHitFriend()
    {
        // 歩行ステートのときのみ反転処理を行う
        const int stateWalk = static_cast<int>(STATE::Walk);
        const int currentStateIndex = GetStateMachine()->GetStateIndex();
        if (currentStateIndex != stateWalk) return;

        moveDirectionX_ = -moveDirectionX_;  // 移動方向を反転

        // 旋回ステートへ遷移
        GetStateMachine()->SetState(static_cast<int>(STATE::Turn));
    }

}