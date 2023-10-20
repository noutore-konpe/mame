#include "Enemy.h"
#include "EnemyManager.h"
#include "WaveManager.h"
#include "PlayerManager.h"

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


// �Ђ�܂���֐�
void Enemy::Flinch()
{
    // �S�[�����Ȃ�Ђ�܂��Ȃ�
    if (Enemy::TYPE::Golem == type) return;

    //// ���ꒆ�Ȃ�Ђ�܂��Ȃ�
    //if (activeNode_ != nullptr && "EntryStage" == activeNode_->GetName()) return;

    isFlinch_ = true;     // �Ђ�݃t���O�𗧂Ă�

    // �A���łЂ�񂾍ۂɂЂ�݃A�j���\�V������
    // ���񃊃Z�b�g���邽�߂ɓK���ȃA�j���[�V��������͂��Ă���
    PlayAnimation(0, false, animationSpeed_);
}

// ������΂��֐�
void Enemy::BlowOff()
{
    // �S�[�����Ȃ琁����΂��Ȃ�
    if (Enemy::TYPE::Golem == type) return;

    //// ���ꒆ�Ȃ琁����΂��Ȃ�
    //if (activeNode_ != nullptr && "EntryStage" == activeNode_->GetName()) return;

    isBlowOff_ = true;     // ������уt���O�𗧂Ă�

    // �A���Ő�����񂾍ۂɂЂ�݃A�j���\�V������
    // ���񃊃Z�b�g���邽�߂ɓK���ȃA�j���[�V��������͂��Ă���
    PlayAnimation(0, false, animationSpeed_);
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

void Enemy::AttackCollisionOnPlayer(const float damage)
{
    if (EnemyManager::AttackCollisionPlayerToEnemy(this))
    {
        PlayerManager::Instance().GetPlayer()->ApplyDamage(damage,this);
    }
}
