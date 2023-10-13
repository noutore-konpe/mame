#include "AbilityManager.h"

#include "../../Taki174/Common.h"

// ������
void AbilityManager::Initialize()
{
    for (Ability*& ability : abilitys_)
    {
        ability->Initialize();
    }
}

// �I����
void AbilityManager::Finalize()
{
    for (Ability*& ability : abilitys_)
    {
        ability->Finalize();
    }
    Clear();
}

// Update�̑O�ɌĂ΂��
void AbilityManager::Begin()
{
    for (Ability*& ability : abilitys_)
    {
        ability->Begin();
    }
}

// �X�V����
void AbilityManager::Update(const float elapsedTime)
{
    // �X�V����
    for (Ability*& ability : abilitys_)
    {
        ability->Update(elapsedTime);
    }

    // �j������
    {
        for (Ability* ability : removes_)
        {
            auto it = std::find(abilitys_.begin(), abilitys_.end(), ability);

            if (it != abilitys_.end())
            {
                abilitys_.erase(it);
            }
            SafeDeletePtr(ability);
        }
        removes_.clear();
    }

}

// Update�̌�ɌĂ΂��
void AbilityManager::End()
{
    for (Ability*& ability : abilitys_)
    {
        ability->End();
    }
}

// �`�揈��
void AbilityManager::Render(const float scale)
{
    for (Ability*& ability : abilitys_)
    {
        ability->Render(scale);
    }
}

// ImGui�p
void AbilityManager::DrawDebug()
{
    for (Ability*& ability : abilitys_)
    {
        ability->DrawDebug();
    }
}

// �o�^
void AbilityManager::Register(Ability* ability)
{
    abilitys_.emplace_back(ability);
}

// �폜
void AbilityManager::Remove(Ability* ability)
{
    // �j�����X�g�ɒǉ�
    removes_.insert(ability);
}

// �S�폜
void AbilityManager::Clear()
{
    for (Ability*& ability : abilitys_)
    {
        SafeDeletePtr(ability);
    }
    abilitys_.clear();
    abilitys_.shrink_to_fit(); // vector�̗]���ȃ��������J������֐�(C++11)
}
