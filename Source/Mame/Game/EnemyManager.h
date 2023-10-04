#pragma once

#include "BaseEnemyAI.h"
#include <vector>
#include <set>

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

    void Initialize();                      // 初期化
    void Finalize();
    void Begin();
    void Update(const float elapsedTime);  // 更新処理
    void End();
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr);  // 描画処理
    void DrawDebug();                       // デバッグ描画

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

    const bool GetIsRunningCRAAction() const { return isRunningCRAAction_; }
    void SetIsRunningCRAAction(const bool isRunningCRAAction) { isRunningCRAAction_ = isRunningCRAAction; }

    const float GetCRAActionCoolTimer() const { return craActionCoolTimer_; }
    void SetCRAActionCoolTimer(const float craActionCoolTime) { craActionCoolTimer_ = craActionCoolTime; }

private:
    std::vector<BaseEnemyAI*> enemies_ = {};
    std::set<BaseEnemyAI*>    removes_ = {};

protected:
    bool    isRunningCRAAction_ = false;    // 誰かが近接攻撃行動を実行中か
    float   craActionCoolTimer_ = 0.0f;     // 近接攻撃行動クールタイマー

};

