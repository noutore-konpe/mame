#pragma once

#include "BaseEnemyAI.h"

class EnemyTestAI : public BaseEnemyAI
{
public:
    EnemyTestAI();
    //~EnemyTestAI() override;

    void Initialize() override;                     // ������
    //void Finalize() override;                       // �I����
    void Update(const float& elapsedTime) override;  // �X�V����
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr) override; // �`�揈��
    //void DrawDebug() override;                      // �f�o�b�O�`��

private:
    static int nameNum_;

};

