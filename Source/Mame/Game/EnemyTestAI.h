#pragma once

#include "BaseCharacterAI.h"

class EnemyTestAI : public BaseCharacterAI
{
public:
    EnemyTestAI();
    //~EnemyTestAI() override;

    void Initialize() override;                     // ������
    //void Finalize() override;                       // �I����
    //void Begin() override;                          // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float elapsedTime) override;  // �X�V����
    //void End() override;                            // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float elapsedTime, const float scale) override; // �`�揈��
    //void DrawDebug() override;                      // �f�o�b�O�`��
};

