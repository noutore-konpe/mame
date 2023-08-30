#pragma once

#include "Character.h"

class Player : public Character
{
public: // enum�֘A
    // �X�e�[�g
    enum class State
    {
        Idle,   // �ҋ@
        Walk,   // ���s
    };

private: // enum�֘A
    // �A�j���[�V����
    enum class Animation
    {
    };

public:
    Player();
    ~Player() override;

    void Initialize() override;                     // ������
    void Finalize();                                // �I����

    void Begin();                                   // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime) override; // �X�V����
    void End();                                     // ���t���[����ԍŌ�ɌĂ΂��
    
    void Render(const float& elapsedTime, const float& scale) override; // �`�揈��
    
    void DrawDebug() override;  // ImGui�p
};

