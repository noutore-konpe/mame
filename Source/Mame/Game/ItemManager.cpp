#include "ItemManager.h"

 //������
void ItemManager::Initialize()
{
    for (Item*& item : items)
    {
        item->Initialize();
    }
}

// �I����
void ItemManager::Finalize()
{
    for (Item*& item : items)
    {
        item->Finalize();
    }

    Clear();
}

// Update�̑O�ɌĂ΂��
void ItemManager::Begin()
{
    for (Item*& item : items)
    {
        item->Begin();
    }
}

// �X�V����
void ItemManager::Update(const float& elapsedTime)
{
    // �X�V����
    for (Item*& item : items)
    {
        item->Update(elapsedTime);
    }

    // �j������
    {
        for (Item* item : removes)
        {
            // vector����v�f���폜����ꍇ�̓C�e���[�^�[�ō폜
            std::vector<Item*>::iterator it = std::find(items.begin(), items.end(), item);

            // std::vector�ŊǗ�����Ă���v�f���폜����ɂ�erase()�֐����g�p����
            // (�j�����X�g�̃|�C���^����C�e���[�^�[���������Aerase�֐��ɓn��)
            if (it != items.end())
            {
                items.erase(it);
            }

            // �A�C�e���̔j��
            delete item;
        }
        // �j�����X�g���N���A
        removes.clear();
    }
}

// Update�̌�ɌĂ΂��
void ItemManager::End()
{
    for (Item*& item : items)
    {
        item->End();
    }
}

// �`�揈��
void ItemManager::Render(const float& elapsedTime, const float& scale)
{
    for (Item*& item : items)
    {
        item->Render(elapsedTime, scale);
    }
}

// ImGui�p
void ItemManager::DrawDebug()
{
    for (Item*& item : items)
    {
        item->DrawDebug();
    }
}

// �o�^
void ItemManager::Register(Item* item)
{
    items.emplace_back(item);
}

// �폜
void ItemManager::Remove(Item* item)
{
    // �j�����X�g�ɒǉ�
    removes.insert(item);
}

// �S�폜
void ItemManager::Clear()
{
    for (Item*& item : items)
    {
        delete item;
    }
    items.clear();
    items.shrink_to_fit();  // vector�̗]���ȃ��������J������֐�(C++11)
}
