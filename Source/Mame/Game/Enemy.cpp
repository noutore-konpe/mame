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


// �Ђ�܂���֐�
void Enemy::Flinch()
{
    // �S�[�����Ȃ�Ђ�܂��Ȃ�
    if (Enemy::TYPE::Golem == type) return;

    //// ���ꒆ�Ȃ�Ђ�܂��Ȃ�
    //if (activeNode_ != nullptr && "EntryStage" == activeNode_->GetName()) return;

    isFlinch_   = true;     // �Ђ�݃t���O�𗧂Ă�
    activeNode_ = nullptr;  // ���݂̎��s�m�[�h�����Z�b�g
    step_       = 0;        // �s���X�e�b�v�����Z�b�g

    // �A���łЂ�񂾍ۂɂЂ�݃A�j���\�V������
    // ���񃊃Z�b�g���邽�߂ɓK���ȃA�j���[�V��������͂��Ă���
    PlayAnimation(0, false, animationSpeed_);

    // �U�����ɂЂ�񂾂Ƃ��̂��߂ɋߋ����U���s���t���O�����낵�Ă���
    EnemyManager& enmManager = EnemyManager::Instance();
    enmManager.SetIsRunningCRAAction(false);

}
