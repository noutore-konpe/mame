#include "ProjectileManager.h"

#include "../../Taki174/Common.h"
#include "../Resource/sprite.h"

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
    for (Projectile*& projectile : projectiles_)
    {
        projectile->Initialize();
    }
}

// �I����
void ProjectileManager::Finalize()
{
    for (Projectile*& projectile : projectiles_)
    {
        projectile->Finalize();
    }
}

// Update�̑O�ɌĂ΂��
void ProjectileManager::Begin()
{
    for (Projectile*& projectile : projectiles_)
    {
        projectile->Begin();
    }
}

// �X�V����
void ProjectileManager::Update(const float elapsedTime)
{
    // �X�V����
    for (Projectile*& projectile : projectiles_)
    {
        projectile->Update(elapsedTime);
    }

    // �j������
    {
        for (Projectile* projectile : removes_)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            auto it = std::find(projectiles_.begin(), projectiles_.end(), projectile);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != projectiles_.end())
            {
                projectiles_.erase(it);
            }

            // �A�C�e���̔j��
            SafeDeletePtr(projectile);
        }
        // �j�����X�g���N���A
        removes_.clear();
    }

}

// Update�̌�ɌĂ΂��
void ProjectileManager::End()
{
    for (Projectile*& projectile : projectiles_)
    {
        projectile->End();
    }
}

// �`�揈��
void ProjectileManager::Render(const float scale, ID3D11PixelShader* psShader)
{
    for (Projectile*& projectile : projectiles_)
    {
        // �J�����O�Ȃ�`�悵�Ȃ�
        bool isInCamera = false;
        Sprite::ConvertToScreenPos(projectile->GetTransform()->GetPosition(), &isInCamera);
        if (false == isInCamera) { continue; }

        projectile->Render(scale, psShader);
    }
}

// ImGui�p
void ProjectileManager::DrawDebug()
{
    for (Projectile*& projectile : projectiles_)
    {
        projectile->DrawDebug();
    }
}

// �o�^
void ProjectileManager::Register(Projectile* projectile)
{
    projectiles_.emplace_back(projectile);
}

// �폜
void ProjectileManager::Remove(Projectile* projectile)
{
    // �j�����X�g�ɒǉ�
    removes_.insert(projectile);
}

// �S�폜
void ProjectileManager::Clear()
{
    for (Projectile*& projectile : projectiles_)
    {
        SafeDeletePtr(projectile);
    }
    projectiles_.clear();
    projectiles_.shrink_to_fit();  // vector�̗]���ȃ��������J������֐�(C++11)
}
