#pragma once
#include "Character.h"
#include "StateMachine.h"
#include "CannonBallManager.h"

enum class EnemyType
{
    Tofu,       // 豆腐
    RedTofu,    // 赤豆腐
};

class Enemy : public Character
{
public:
    Enemy() {}
    virtual ~Enemy() {};

    virtual void Initialize()                       = 0;    // 初期化
    virtual void Finalize()                         = 0;    // 終了化
    virtual void Begin()                            = 0;    // 毎フレーム一番最初に呼ばれる
    virtual void Update(const float& elapsedTime)   = 0;    // 更新処理
    virtual void End()                              = 0;    // 毎フレーム一番最後に呼ばれる
    virtual void Render(const float& elapsedTime);          // 描画処理
    virtual void DrawDebug()                        = 0;    // デバッグ描画

    virtual void OnAttacked()   {};                         // 攻撃したときに呼ばれる
    virtual void OnHitFriend()  {};                         // 味方に当たった時に呼ばれる

    void OnFallDead() override;                             // 落下死・落下ミスしたときに呼ばれる

    void Destroy();                                         // エネミー削除

    void CollisionEnemyVsPlayer();                          // エネミーとプレイヤーの衝突判定処理  

    bool IsInLengthPlayer(const float lengthXLimit);        // プレイヤーが距離内にいるか判定する

public:
    // 移動処理
    void Move(const float vx, const float moveSpeed)
    {
        Character::Move(vx, moveSpeed);
    }

    // 旋回処理
    bool Turn(
        const    float elapsedTime,
        const    float vx,
        NO_CONST float turnSpeed
    );

    // ジャンプ処理
    void Jump(const float jumpSpeed)
    {
        Character::Jump(jumpSpeed);
    }

public:
    // ステートマシン
    StateMachine* GetStateMachine()const { return stateMachine.get(); }

    const bool GetMoveRight() const { return moveRight; }
    void SetMoveRight(const bool r) { moveRight = r; }

    // 目的地の取得・設定
    const float& GetDestination() const { return destination_; }
    void SetDestination(const float& destination) { destination_ = destination; }

    // 移動範囲の半径の取得・設定
    const float& GetMoveRangeRadius() const { return moveRangeRadius_; }
    void SetMoveRangeRadius(const float& moveRange ) { moveRangeRadius_ = moveRange; }    
    
    // 移動範囲の中心Xの取得・設定
    const float& GetMoveRangeCenterX() const { return moveRangeCenterX_; }
    void SetMoveRangeCenterX(const float& moveRangeCenterX ) { moveRangeCenterX_ = moveRangeCenterX; }    
    
    // 索敵距離Xの取得・設定
    const float& GetSerchLengthX() const { return searchLengthX_; }
    void SetSerchLengthX(const float& searchLengthX) { searchLengthX_ = searchLengthX; }       
    // 索敵距離Yの取得・設定
    const float& GetSerchLengthY() const { return searchLengthY_; }
    void SetSerchLengthY(const float& searchLengthY) { searchLengthY_ = searchLengthY; }

    // 移動方向Xの取得・設定
    const float& GetMoveDirectionX() const { return moveDirectionX_; }
    void SetMoveDirectionX(const float& moveDirectionX) { moveDirectionX_ = moveDirectionX; }

    // プレイヤーに当たったかの取得・設定
    const bool GetIsHitPlayer() const { return isHitPlayer_; }
    void SetIsHitPlayer(const bool isHitPlayer) { isHitPlayer_ = isHitPlayer; }

    // 味方に当たったかの取得・設定
    const bool GetIsHitFriend() const { return isHitFriend_; }
    void SetIsHitFriend(const bool isHitFriend) { isHitFriend_ = isHitFriend; }

    // 味方の上に乗っかっているかの取得・設定
    const bool GetIsOnFriend() const { return isOnFriend_; }
    void SetIsOnFriend(const bool isOnFriend) { isOnFriend_ = isOnFriend; }

public: // static変数
    static float renderLengthXLimit_;   // 敵を描画する距離制限
    static float playSELengthXLimit_;   // SEを再生する距離制限

public:    
    std::unique_ptr<StateMachine> stateMachine{ nullptr };  // ステートマシン

    CannonBallManager cannonBallManager = {};               // 大砲の弾マネージャー

    EnemyType enemyType_    = {};                           // 敵の種類(識別用)

protected:
    float destination_      =    0.0f;     // 目的地
    float moveRangeCenterX_ =    0.0f;     // 移動範囲の中心X
    float moveRangeRadius_  =    4.0f;     // 移動範囲の半径
    float searchLengthX_    =    4.0f;     // X軸で索敵できる距離
    float searchLengthY_    =    2.0f;     // Y軸で索敵できる距離
    float moveDirectionX_   =   -1.0f;     // 移動方向X（デフォルトは左向き）

    bool  isHitPlayer_      =   false;     // プレイヤーに当たったか
    bool  isHitFriend_      =   false;     // 味方に当たったか
    bool  isOnFriend_       =   false;     // 味方の上に乗っかっているか

private:
    bool moveRight          =   true; // 右に進むかどうか
};

