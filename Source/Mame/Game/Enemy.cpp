#include "Enemy.h"
#include "EnemyManager.h"
#include "WaveManager.h"

Enemy::~Enemy()
{
    // �E�F�[�u�ŊǗ�����Ă����G�Ȃ�c��̃E�F�[�u�G�l�~�[�J�E���g���P���炷
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

// ������΂����̕ۑ�
void Enemy::SaveBlowOffInfo(
    const DirectX::XMFLOAT3& blowOffVec,
    const BLOW_OFF_FORCE_LEVEL& blowOffForceLevel)
{
    blowOffVec_        = blowOffVec;
    blowOffForceLevel_ = blowOffForceLevel;
}

void Enemy::Destroy()
{
    // ����������
    EnemyManager::Instance().Remove(this);
}

// ��������q�m�[�h�����݂��邩��������
const bool Enemy::IsExistChildNode(const std::string& findNodeName)
{
    std::vector<NodeBase*>* Nodes = behaviorTree_->GetRoot()->GetChildren();

    for (size_t i = 0; i < Nodes->size(); ++i)
    {
        // �T���Ă���m�[�h�Ɩ��O����v������true��Ԃ�
        if (findNodeName == Nodes->at(i)->GetName())
        {
            return true;
        }
    }

    return false;
}
