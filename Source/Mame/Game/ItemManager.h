#pragma once

#include "Item.h"
#include <vector>
#include <set>

class ItemManager
{
private:
    ItemManager() {}
    ~ItemManager() {}

public:
    // �B��̃C���X�^���X�擾
    static ItemManager& Instance()
    {
        static ItemManager instance;
        return instance;
    }

    void Initialize();                                          // ������
    void Finalize();                                            // �I����
    void Begin();                                               // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);                      // �X�V����
    void End();                                                 // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& elapsedTime, const float& scale);  // �`�揈��
    void DrawDebug();                                           // ImGui�p

public:
    void Register(Item* item);  // �o�^
    void Remove(Item* item);    // �폜
    void Clear();               // �S�폜

public: // �擾�E�ݒ�@�֘A
    const int GetItemCount() const { return static_cast<int>(items.size()); } // �A�C�e�����擾

    Item* GetItem(int index) { return items.at(index); } // �A�C�e���擾

private:
    std::vector<Item*>  items   = {};
    std::set<Item*>     removes = {};
};

