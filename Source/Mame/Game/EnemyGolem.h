#pragma once

#include "Enemy.h"

class EnemyGolem : public Enemy
{
public:
    EnemyGolem();
    ~EnemyGolem() override;

    void Initialize()                                           override; // ������
    void Finalize()                                             override; // �I����
    void Begin()                                                override; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override; // �X�V����
    void End()                                                  override; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // �`�揈��
    void DrawDebug()                                            override; // �f�o�b�O�`��

    void UpdateConstants() override;
};

