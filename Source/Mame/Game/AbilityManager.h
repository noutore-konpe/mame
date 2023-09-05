#pragma once

#include "Ability.h"
#include <vector>
#include <set>

class AbilityManager
{
private:
    AbilityManager() {}
    ~AbilityManager() {}

public:
    // �B��̃C���X�^���X�擾
    static AbilityManager& Instance()
    {
        static AbilityManager instance;
        return instance;
    }

    void Initialize();                      // ������
    void Finalize();                        // �I����
    void Begin();                           // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime);  // �X�V����
    void End();                             // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale);        // �`�揈��
    void DrawDebug();                       // ImGui�p

public:
    void Register(Ability* ability);    // �o�^
    void Remove(Ability* ability);      // �폜
    void Clear();                       // �S�폜

public: // �擾�E�ݒ�@�֘A
    const int GetAbilityCount() const { return static_cast<int>(abilitys.size()); } // �A�r���e�B���擾

    Ability* GetAbility(int index) { return abilitys.at(index); } // �A�r���e�B�擾

private:
    std::vector<Ability*>  abilitys = {};
    std::set<Ability*>     removes = {};
};

