#pragma once

#include "Character.h"
#include "../Graphics/Effect.h"

enum ClearState
{
    LookToCamera,   // カメラの方向を見る
    JumpForJoy,     // 飛び跳ねさせる
    MoveToLeft,     // 左に下がる
    MoveToRight,    // 右に進む
};


class Player : public Character
{
// enum関連
public: 
    // ステート
    enum class State
    {
        Idle,    // 待機
        Walk,    // 歩行
        Dash,    // ダッシュ(一時的な急加速)
        Run,     // 走行
        Jump,    // ジャンプ
        HipDrop, // ヒップドロップ
        Death,   // 死亡
        Clear    // クリア
    };

private:
    // アニメーション
    enum Animation
    {
        Anim_Idle,      // 待機
        Anim_Dash,      // ダッシュ（一時的な急加速）
        Anim_Run,       // 走行
        Anim_Brake,     // ブレーキ
        Anim_JumpInit,  // ジャンプ開始
        Anim_Jump,      // ジャンプ
        Anim_Fall,      // 落下
        Anim_JumpEnd,   // 着地（ジャンプ終了）
        Anim_HipDrop,   // ヒップドロップ
        Anim_Walk,      // 歩行
        Anim_Max,       // アニメーション最大数
    };

    // ステージ選択
    enum SELECT
    {
        TutorialStage,  // チュートリアル
        Move_T_P,       // チュートリアル->野原
        Move_P_T,       // 野原->チュートリアル
        PlainsStage,    // 野原
        Move_P_B,       // 野原->ボス
        Move_B_P,       // ボス->野原
        BossStage,      // ボス
        Move_B_E,   // ボス->Ex
        Move_E_B,   // Ex->ボス
        ExStage,    // Ex
    };

public:
    Player();
    ~Player() override;

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Begin();                           // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);  // 更新処理
    void End();                             // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime);  // 描画処理

    void DrawDebug() override;              // デバッグ描画

    void UpdateSelectStage(const float& elapsedTime,int* state);
    bool isAnimationSet = false;
    void SetIsAnimationSet(bool a) { isAnimationSet = a; }
    bool GetIsAnimationSet() { return isAnimationSet; }

    const State& GetState() const { return state; };
    const int GetClearState() const { return clearState_; }
public:
    // ダメージを与える
    bool ApplyDamage(
        const int& damage, 
        const float& invincibleTime
    ) override;

private: // 入力処理関数関連   
    const float GetMoveVecX() const;                        // 左スティック入力値から移動X方向ベクトルを取得   
    const float GetMoveVecY() const;                        // 左スティック入力値から移動Y方向ベクトルを取得   
    const bool  InputMove(const float& elapsedTime);        // 移動入力処理   
    const bool  InputJump();                                // ジャンプ入力処理

private: // 衝突判定関数関連       
    void CollisionPlayerVsEnemies();                            // プレイヤーとエネミーの衝突判定

private: // 更新処理関数関連
    void UpdateDashCoolTimer(const float& elapsedTime);         // ダッシュクールタイム更新処理
    void UpdateBounceInvincibleTimer(const float& elapsedTime); // バウンス無敵時間更新

private:
    bool TurnToCamera(const float elapsedTime, NO_CONST float turnSpeed);  // カメラの方向に向く
       
private: // 瞬間的に呼ばれる関数関連
    void OnLanding()    override;                       // 着地したときに呼ばれる   
    void OnDash()       override;                       // ダッシュしているときに呼ばれる
    void OnBounce()     override;                       // バウンスするときに呼ばれる
    void OnDamaged()    override;                       // ダメージを受けたときに呼ばれる
    void OnDead()       override;                       // 死亡したときに呼ばれる
    void OnFallDead()   override;                       // 落下死・落下ミスしたときに呼ばれる

private: // ステート関数関連
    void TransitionIdleState();                         // 待機ステートへ遷移
    void UpdateIdleState(const float& elapsedTime);     // 待機ステート更新処理
     
    void TransitionWalkState();                         // 歩行ステートへ遷移   
    void UpdateWalkState(const float& elapsedTime);     // 歩行ステート更新処理    
    
    void TransitionDashState();                         // ダッシュステートへ遷移   
    void UpdateDashState(const float& elapsedTime);     // ダッシュステート更新処理    
    
    void TransitionRunState();                          // 走行ステートへ遷移   
    void UpdateRunState(const float& elapsedTime);      // 走行ステート更新処理
 
    void TransitionJumpState();                         // ジャンプステートへ遷移   
    void UpdateJumpState(const float& elapsedTime);     // ジャンプステート更新処理    
    
    void TransitionHipDropState();                      // ヒップドロップステートへ遷移   
    void UpdateHipDropState(const float& elapsedTime);  // ヒップドロップステート更新処理    
    
    void TransitionDeathState();                        // 死亡ステートへ遷移   
    void UpdateDeathState(const float& elapsedTime);    // 死亡ステート更新処理    
    
    void UpdateClearState(const float& elapsedTime);    // クリアステート更新処理
public:
    void TransitionClearState();                        // クリアステートへ遷移   

private: // 変数関連
    State   state                   = State::Idle;          // 現在のステート

    float   dashAcceleration        =  15.0f;               // ダッシュ時のX速度
    float   defaultDashTime         =  0.025f;                // ダッシュ時間初期値
    float   dashTimer               =  defaultDashTime;     // ダッシュタイマー(ダッシュ時の操作制限時間を管理する)
    float   dashFinishScale         =  0.5f;                // ダッシュが終わって別のステート遷移前に速度を減速させる値
    float   dashCoolTime            =  0.2f;
    float   dashCoolTimer           =  dashCoolTime;        // ダッシュを連発されないようクールタイムを設定

    float   runMoveSpeed            =  11.0f;               // 走行時の移動速度(moveSpeedに代入する)
    float   runAcceleration         =  0.5f;                // 走行時の加速力
    float   runFriction             =  0.1f;                // 走行時の摩擦力
    float   runMoveVecX             =  0.0f;                // moveVecXを保存する（ブレーキアニメ―ションのときに使う）

    float   jumpedPositionY         =  0.0f;                // ジャンプ開始時の位置を保存する

    float   hipDropGravity          = -2.0f;                // ヒップドロップ時の重力
    float   needHipDropHeight       =  1.0f;                // ヒップドロップに必要な高さ

    float   defaultBounceSpeedX     =  15.0f;               // バウンスX速度初期値
    float   defaultBounceSpeedY     =  10.0f;               // バウンスY速度初期値
    float   bounceSpeedX            =  defaultBounceSpeedX; // バウンスX速度
    float   bounceSpeedY            =  defaultBounceSpeedY; // バウンスY速度
    float   bounceScaleX            =  0.8f;                // バウンスX速度にかける値
    float   bounceScaleY            =  0.8f;                // バウンスY速度にかける値
    float   bounceInvincibleTimer_  =  0.0f;                // バウンス無敵タイマー(点滅させないように別で追加)

    float   deathGravity_           = -0.5f;                // 死亡ステート時の重力

    float   clearAcceleration_      =  0.7f;                // クリアステート時の加速力
    float   clearMoveSpeed_         =  25.0f;               // クリアステート時の移動速度
    float   clearTimer_             =  0.0f;                // クリアステート時のタイマー

    int     bounceCount             =  0;                   // バウンス回数
    int     bounceLimit             =  2;                   // 最大バウンス回数

    int     clearState_             =  0;                   // クリアステート内のステート
    int     clearJumpCount_         =  0;                   // クリアステートのジャンプカウント

    Effect* effect[2];

};