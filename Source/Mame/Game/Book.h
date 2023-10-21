#pragma once

#include "../../Taki174/Common.h"
#include "Item.h"
#include "ProjectileManager.h"
#include "StateMachine.h"

class Book : public Item
{
public:
    enum class Animation
    {
        Closed,         // 閉じた状態
        Opened,         // 開いた状態
        Open,           // 開く
        TurnThePage,    // ページをめくる
        Close,          // 閉まる
    };

    enum class StateMachineState
    {
        Idle,   // 待機
        Open,   // 開く
        Attack, // 攻撃
        Close,  // 閉じる
    };

public:
    Book();
    ~Book() override;

    void Initialize()                       override; // 初期化
    void Finalize()                         override; // 終了化
    void Begin()                            override; // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime)   override; // 更新処理
    void End()                              override; // 毎フレーム一番最後に呼ばれる
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // 描画処理
    void DrawDebug()                        override; // ImGui用

    // 弾丸と敵との衝突処理
    void CollisionProjectileVsEnemies();

public:
    // XZ回転
    void Turn(
        const float elapsedTime,
        const float vx,
        const float vz,
        const float turnSpeed /*Degree*/
    );

    // 弾生成&発射
    bool LaunchProjectile(const float elapsedTime, const DirectX::XMFLOAT3& vec);

public:// 取得・設定 関連
    void SetTransform();

    // ステートマシン
    StateMachine<State<Book>>* GetStateMachine() const { return stateMachine.get(); }

    // 弾丸
    void SetLaunchTimer(float time) { launchTimer = time; }
    int GetMaxLaunchNum() { return maxLaunchNum; }

    [[nodiscard]] const float GetSearchEnemyRange() const { return searchEnemyRange_; }

private:
    // ----- 制作に必要な情報 -----
    static int totalNum;    // 現在 "Book" が何個生成されてるか知るためのもの
    int num;                // 自分が今何個目か
    // ----- 制作に必要な情報 -----

    // 弾丸
    ProjectileManager projectileManager;
    float launchTimer = 0.0f;   // 発射用タイマー
    float launchTime = 0.5f;    // 発射までの時間      // ☆ ( 後で能力として設定される )
    int maxLaunchNum = 3;       // 一回の発射の最大数  // ☆

    // ステートマシン
    std::unique_ptr<StateMachine<State<Book>>> stateMachine = nullptr;

    DirectX::XMFLOAT3 createPosition{}; // 生成位置
    DirectX::XMFLOAT3 prevPosition{};   // １フレーム前の位置

    //bool isMoveToUp = true;             // 上に移動するか
    float circularMotionRotationZ_  = 0.0f;               // 円運動回転値Z(ラジアン)
    float circularMotionRadius_     = 0.05f;              // 円運動半径(上下する範囲に影響)
    float circularMotionAddRotate_  = ::ToRadian(180.0f); // 円運動回転値に追加する回転(上下速度に影響)

    float searchEnemyRange_ = 15.0f;    // 敵を索敵できる範囲
};