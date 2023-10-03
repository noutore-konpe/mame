#include "EnemyManager.h"
#include "../../Taki174/Common.h"

void EnemyManager::Initialize()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Initialize();
    }
}

void EnemyManager::Finalize()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Finalize();
    }
    Clear();
}

void EnemyManager::Begin()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Begin();
    }
}

// �X�V����
void EnemyManager::Update(const float elapsedTime)
{
    // �X�V
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Update(elapsedTime);
    }

    // �j������
    {
        for (BaseEnemyAI* enemy : removes_)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            std::vector<BaseEnemyAI*>::iterator it = std::find(enemies_.begin(), enemies_.end(), enemy);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != enemies_.end())
            {
                enemies_.erase(it);
            }

            // �G�̔j������
            SafeDeletePtr(enemy);
        }
        // �j�����X�g���N���A
        removes_.clear();
    }


    // �G�l�~�[���m�̏Փ˔��菈��
    //CollisionEnemyVsEnemy();
}

void EnemyManager::End()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->End();
    }
}


// �`�揈��
void EnemyManager::Render(const float scale, ID3D11PixelShader* psShader)
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->Render(scale, psShader);
    }
}


// �G�l�~�[�o�^
void EnemyManager::Register(BaseEnemyAI* enemy)
{
    enemies_.emplace_back(enemy);
}


// �G�l�~�[�폜
void EnemyManager::Remove(BaseEnemyAI* enemy)
{
    // �j�����X�g�ɒǉ�
    removes_.insert(enemy);
}


// �G�l�~�[�S�폜
void EnemyManager::Clear()
{
    for (BaseEnemyAI*& enemy : enemies_)
    {
        SafeDeletePtr(enemy);
    }
    enemies_.clear();
    enemies_.shrink_to_fit();    // vector�̗]���ȃ����������
}


// �f�o�b�O
void EnemyManager::DrawDebug()
{
#ifdef USE_IMGUI
    for (BaseEnemyAI*& enemy : enemies_)
    {
        enemy->DrawDebug();
    }
#endif
}
