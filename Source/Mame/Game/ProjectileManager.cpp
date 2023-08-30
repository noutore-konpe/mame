#include "ProjectileManager.h"

// �R���X�g���N�^
ProjectileManager::ProjectileManager()
{
}

// �f�X�g���N�^
ProjectileManager::~ProjectileManager()
{
    Clear();
}

// ������
void ProjectileManager::Initialize()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->Initialize();
    }
}

// �I����
void ProjectileManager::Finalize()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->Finalize();
    }
}

// Update�̑O�ɌĂ΂��
void ProjectileManager::Begin()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->Begin();
    }
}

// �X�V����
void ProjectileManager::Update(const float& elapsedTime)
{
    // �X�V����
    for (Projectile*& projectile : projectiles)
    {
        projectile->Update(elapsedTime);
    }

    // �j������
    {
        for (Projectile* projectile : removes)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != projectiles.end())
            {
                projectiles.erase(it);
            }

            // �A�C�e���̔j��
            delete projectile;
        }
        // �j�����X�g���N���A
        removes.clear();
    }
}

// Update�̌�ɌĂ΂��
void ProjectileManager::End()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->End();
    }
}

// �`�揈��
void ProjectileManager::Render(const float& elapsedTime, const float& scale)
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->Render(elapsedTime, scale);
    }
}

// ImGui�p
void ProjectileManager::DrawDebug()
{
    for (Projectile*& projectile : projectiles)
    {
        projectile->DrawDebug();
    }
}

// �o�^
void ProjectileManager::Register(Projectile* projectile)
{
    projectiles.emplace_back(projectile);
}

// �폜
void ProjectileManager::Remove(Projectile* projectile)
{
    // �j�����X�g�ɒǉ�
    removes.insert(projectile);
}

// �S�폜
void ProjectileManager::Clear()
{
    for (Projectile*& projectile : projectiles)
    {
        delete projectile;
    }
    projectiles.clear();
    projectiles.shrink_to_fit();  // vector�̗]���ȃ��������J������֐�(C++11)
}
