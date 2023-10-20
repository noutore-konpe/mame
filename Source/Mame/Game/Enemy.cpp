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

// 吹っ飛ばし情報の保存
void Enemy::SaveBlowOffInfo(
    const DirectX::XMFLOAT3& blowOffVec,
    const BLOW_OFF_FORCE_LEVEL& blowOffForceLevel)
{
    blowOffVec_        = blowOffVec;
    blowOffForceLevel_ = blowOffForceLevel;
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
