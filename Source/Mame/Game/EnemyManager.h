#pragma once

#include <vector>
#include <set>

#include "BaseEnemyAI.h"
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

    void CollisionEnemyVsEnemy(const float elapsedTime);

public:
    // エネミー登録
    void Register(BaseEnemyAI* enemy);

    // エネミー削除
    void Remove(BaseEnemyAI* enemy);

    // エネミー全削除
    void Clear();

public:
    // エネミー数取得
    const size_t GetEnemyCount() const { return enemies_.size(); }

    // エネミー取得
    BaseEnemyAI* GetEnemy(const size_t index) { return enemies_.at(index); }
    ProjectileManager* GetProjectileManager() { return &projectileManager_; }

    const bool GetIsRunningCRAAction() const { return isRunningCRAAction_; }
    void SetIsRunningCRAAction(const bool isRunningCRAAction) { isRunningCRAAction_ = isRunningCRAAction; }

    const float GetCRAActionCoolTimer() const { return craActionCoolTimer_; }
    void SetCRAActionCoolTimer(const float craActionCoolTime) { craActionCoolTimer_ = craActionCoolTime; }

private:
    std::vector<BaseEnemyAI*> enemies_ = {};
    std::set<BaseEnemyAI*>    removes_ = {};

    ProjectileManager projectileManager_ = {};

    float   craActionCoolTimer_ = 0.0f;     // 近接攻撃行動クールタイマー
    bool    isRunningCRAAction_ = false;    // 誰かが近接攻撃行動を実行中か

};

