#include "Enemy.h"
#include "EnemyManager.h"
#include "WaveManager.h"
#include "PlayerManager.h"

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

    isFlinch_ = true;     // ひるみフラグを立てる

    // 連続でひるんだ際にひるみアニメ―ションを
    // 毎回リセットするために適当なアニメーションを入力しておく
    PlayAnimation(0, false, animationSpeed_);
}

// 吹っ飛ばす関数
void Enemy::BlowOff()
{
    // ゴーレムなら吹っ飛ばさない
    if (Enemy::TYPE::Golem == type) return;

    //// 入場中なら吹っ飛ばさない
    //if (activeNode_ != nullptr && "EntryStage" == activeNode_->GetName()) return;

    isBlowOff_ = true;     // 吹っ飛びフラグを立てる

    // 連続で吹っ飛んだ際にひるみアニメ―ションを
    // 毎回リセットするために適当なアニメーションを入力しておく
    PlayAnimation(0, false, animationSpeed_);
}

void Enemy::Destroy()
{
    // 自分を消去
    EnemyManager::Instance().Remove(this);
}

// 検索する子ノードが存在するか検索する
const bool Enemy::IsExistChildNode(const std::string& findNodeName)
{
    std::vector<NodeBase*>* Nodes = behaviorTree_->GetRoot()->GetChildren();

    for (size_t i = 0; i < Nodes->size(); ++i)
    {
        // 探しているノードと名前が一致したらtrueを返す
        if (findNodeName == Nodes->at(i)->GetName())
        {
            return true;
        }
    }

    return false;
}

void Enemy::AttackCollisionOnPlayer(const float damage)
{
    if (EnemyManager::AttackCollisionPlayerToEnemy(this))
    {
        PlayerManager::Instance().GetPlayer()->ApplyDamage(damage,this);
    }
}
