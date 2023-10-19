#include "Enemy.h"
#include "EnemyManager.h"
#include "WaveManager.h"

Enemy::~Enemy()
{
    // ウェーブで管理されていた敵なら残りのウェーブエネミーカウントを１減らす
    if (true == isWaveEnemy_)
    {
        WaveManager::Instance().ReduceRemainingEnemyCounter();
    }
}

void Enemy::Initialize()
{
    Character::Initialize();
}

void Enemy::Update(const float& elapsedTime)
{
    Character::Update(elapsedTime);
}

void Enemy::Render(const float& scale, ID3D11PixelShader* psShader)
{
    Character::Render(scale, psShader);
}


// ひるませる関数
void Enemy::Flinch()
{
    // ゴーレムならひるませない
    if (Enemy::TYPE::Golem == type) return;

    //// 入場中ならひるませない
    //if (activeNode_ != nullptr && "EntryStage" == activeNode_->GetName()) return;

    isFlinch_   = true;     // ひるみフラグを立てる
    activeNode_ = nullptr;  // 現在の実行ノードをリセット
    step_       = 0;        // 行動ステップをリセット

    // 連続でひるんだ際にひるみアニメ―ションを
    // 毎回リセットするために適当なアニメーションを入力しておく
    PlayAnimation(0, false, animationSpeed_);

    // 攻撃中にひるんだときのために近距離攻撃行動フラグを下ろしておく
    EnemyManager& enmManager = EnemyManager::Instance();
    enmManager.SetIsRunningCRAAction(false);

}
