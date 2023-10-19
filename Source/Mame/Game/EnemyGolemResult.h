#pragma once

#include "Enemy.h"

class EnemyGolemResult : public Enemy
{
public: // �萔
    enum class Animation
    {
        Idle,               // �ҋ@
        Landing,            // ���n
        GetUp,              // �N���オ��
        SwingUpArm,         // �r�̐U��グ
        SwingDownArm,       // �r�̐U�艺��
        SwingGetUp,         // �X�C���O����߂�
        Walk,               // ����
        RoarUp,             // ���K����
        RoarDown,           // ���K
        RoarReturn,         // ���K����̂��ǂ�
        Attack1Tame,        // �U���P�̗���
        Attack1,            // �U���P
        Attack1Return,      // �U������̖߂�
        ComboAttack1,       // �R���{�U���̈ꌂ��
        ComboAttack2,       // �R���{�U���̓񌂖�
        ComboAttack2Return, // �R���{�Q���ږ߂�
        ComboAttack3Up,     // �R���{�R���ڐU��グ
        ComboAttack3Down,   // �R���{�R���ڐU�艺��
        ComboAttack3Return, // �R���{�R���ږ߂�
        Death0,             // ���S0
        Death1,             // ���S1
        Down,               // ���ݓ|��
        DownReturn,         // ���݋N���オ��
        Attack2,            // �U���Q
    };

public:
    EnemyGolemResult();
    ~EnemyGolemResult() override {};

    void Initialize()                                           override; // ������
    void Finalize()                                             override {}; // �I����
    void Begin()                                                override {}; // ���t���[����ԍŏ��ɌĂ΂��
    void Update(const float& elapsedTime)                       override; // �X�V����
    void End()                                                  override {}; // ���t���[����ԍŌ�ɌĂ΂��
    void Render(const float& scale, ID3D11PixelShader* psShader = nullptr)   override; // �`�揈��

    void Render(const float& scale, bool shadow, ID3D11PixelShader* psShader = nullptr) override {};
    void DrawDebug()                                            override; // �f�o�b�O�`��

    void UpdateConstants() override;

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> golemPS;
};

