#include "AbilityManager.h"

// ������
void AbilityManager::Initialize()
{
    for (Ability*& ability : abilitys)
    {
        ability->Initialize();
    }
}

// �I����
void AbilityManager::Finalize()
{
    for (Ability*& ability : abilitys)
    {
        ability->Finalize();
    }

    Clear();
}

// Update�̑O�ɌĂ΂��
void AbilityManager::Begin()
{
    for (Ability*& ability : abilitys)
    {
        ability->Begin();
    }
}

// �X�V����
void AbilityManager::Update(const float& elapsedTime)
{
    // �X�V����
    for (Ability*& ability : abilitys)
    {
        ability->Update(elapsedTime);
    }

    // �j������
    {
        for (Ability*& ability : abilitys)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            std::vector<Ability*>::iterator it = std::find(abilitys.begin(), abilitys.end(), ability);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != abilitys.end())
            {
                abilitys.erase(it);
            }

            // �A�r���e�B�̔j��
            delete ability;
        }
        // �j�����X�g���N���A
        removes.clear();
    }
}

// Update�̌�ɌĂ΂��
void AbilityManager::End()
{
    for (Ability*& ability : abilitys)
    {
        ability->End();
    }
}

// �`�揈��
void AbilityManager::Render(const float& scale)
{
    for (Ability*& ability : abilitys)
    {
        ability->Render(scale);
    }
}

// ImGui�p
void AbilityManager::DrawDebug()
{
    for (Ability*& ability : abilitys)
    {
        ability->DrawDebug();
    }
}

// �o�^
void AbilityManager::Register(Ability* ability)
{
    abilitys.emplace_back(ability);
}

// �폜
void AbilityManager::Remove(Ability* ability)
{
    // �j�����X�g�ɒǉ�
    removes.insert(ability);
}

// �S�폜
void AbilityManager::Clear()
{
    for (Ability*& ability : abilitys)
    {
        delete ability;
    }
    abilitys.clear();
    abilitys.shrink_to_fit(); // vector�̗]���ȃ��������J������֐�(C++11)
}
