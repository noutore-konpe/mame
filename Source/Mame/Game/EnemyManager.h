#pragma once

#include "Enemy.h"
#include <vector>
#include <set>

// エネミーマネージャー
class EnemyManager
{
private:
    EnemyManager() {}
    ~EnemyManager() {}

public:
    // 唯一のインスタンス取得
    static EnemyManager& Instance()
    {
        static EnemyManager instance;
        return instance;
    }

    void Initialize();                      // 初期化
    void Finalize();                        // 終了化
    void Begin();                           // 毎フレーム一番最初に呼ばれる
    void Update(const float& elapsedTime);  // 更新処理
    void End();                             // 毎フレーム一番最後に呼ばれる
    void Render(const float& elapsedTime);  // 描画処理
    void DrawDebug();                       // デバッグ描画

    void AllKill();

    void CollisionEnemyVsEnemy();           // エネミー同士の衝突判定処理
public:
    // エネミー登録
    void Register(Enemy* enemy);

    // エネミー削除
    void Remove(Enemy* enemy);

    // エネミー全削除
    void Clear();

public:
    // エネミー数取得
    const int GetEnemyCount() const { return static_cast<int>(enemies.size()); }

    // エネミー取得
    Enemy* GetEnemy(int index) { return enemies.at(index); }

private:
    std::vector<Enemy*> enemies = {};
    std::set<Enemy*>    removes = {};
};

