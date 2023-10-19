#pragma once

#include <vector>
#include <set>

#include "Enemy.h"
#include "ProjectileManager.h"

class EnemyManager
{
private:
    EnemyManager()  {}
    ~EnemyManager() {}

public:
    // 唯一のインスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Initialize();
    void Finalize();
    void Begin();
    void Update(const float elapsedTime);
    void End();
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr);
    void DrawDebug();

    void RenderShadow(const float scale, ID3D11PixelShader* psShader = nullptr);

    void CollisionEnemyVsEnemy(const float elapsedTime);

    // 敵とステージとの衝突処理
    void CollisionEnemyVsStage(const float elapsedTime);

    //攻撃判定を取得する関数
    static bool AttackCollisionPlayerToEnemy(Enemy* my);

    //ACはActive Check コライダーのisActiveがtrueになっている物のみ判定する関数
    static bool ACAttackCollisionPlayerToEnemy(Enemy* my);

public:
    // エネミー登録
    void Register(Enemy* enemy);

    // エネミー削除
    void Remove(Enemy* enemy);

    // エネミー全削除
    void Clear();

public:
    // エネミー数取得
    const size_t GetEnemyCount() const { return enemies_.size(); }
    void GetSpecifyEnemy(const Enemy::TYPE& type, std::vector<Enemy*>& e) const
    {
        for (Enemy* enemy : enemies_)
        {
            if (enemy->GetType() == type)
            {
                e.emplace_back(enemy);
            }
        }
    }

    std::vector<Enemy*>& GetEnemies() { return enemies_; }

    // エネミー取得
    Enemy* GetEnemy(const size_t index) { return enemies_.at(index); }
    ProjectileManager* GetProjectileManager() { return &projectileManager_; }

    const bool GetIsRunningCRAAction() const { return isRunningCRAAction_; }
    void SetIsRunningCRAAction(const bool isRunningCRAAction) { isRunningCRAAction_ = isRunningCRAAction; }

    const float GetCRAActionCoolTimer() const { return craActionCoolTimer_; }
    void SetCRAActionCoolTimer(const float craActionCoolTime) { craActionCoolTimer_ = craActionCoolTime; }

private:
    std::vector<Enemy*> enemies_ = {};
    std::set<Enemy*>    removes_ = {};
    std::set<Enemy*>    generates_ = {};

    ProjectileManager projectileManager_ = {};

    float   craActionCoolTimer_ = 0.0f;     // 近接攻撃行動クールタイマー
    bool    isRunningCRAAction_ = false;    // 誰かが近接攻撃行動を実行中か

};

