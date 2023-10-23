#pragma once
#include "BaseSkill.h"

//�U���Ɋւ���X�L��
namespace PlayerSkill
{
    // �z��
    class Drain : public BaseSkill
    {
    public:
        Drain(Player* player) :
            BaseSkill(player, 
                L"./Resources/Image/Card/Ckyusyu.png",
                L"./Resources/Image/Icon/kyusyu.png",
                "Drain",BaseSkill::SUPER_RARE) {}
        ~Drain() {}

        void Initialize()override;
        void Update(float elapsedTime)override;
        void DrawDebug()override;

        void Overlaping()override;

        //HP�z������ ������v���C���[�̍U�������ɑg�ݍ���
        void Assimilate(float power/*�G�ɗ^�����U���̗�*/);

    private:
        float drainCoefficient;//�U���͂���z��HP�̎Z�o�Ɏg���W��(�d�����邽�тɂ��ꂪ�����Ă���)
    };

    //���j�����t�^�i�v�j
    class BlastEnchantment : public BaseSkill
    {
    public:
        BlastEnchantment(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
                "Blast Enchantment",
                BaseSkill::ULTRA_RARE) {}
        ~BlastEnchantment() {}
    };

    //�u���b�N�z�[��
    class BlackHoleSkill : public BaseSkill
    {
    public:
        BlackHoleSkill(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Cblack.png",
                L"./Resources/Image/Icon/black.png",
                "BlackHole",
                BaseSkill::ULTRA_RARE) {}
        ~BlackHoleSkill() {}

        void Overlaping()override;

        void CreateBlackHole();

        void Update(float elapsedTime);

    private:
        float coolTime = 30.0f;
        float coolTimer;
        const float coolTimeDecrease = 3.0f;

        DirectX::XMFLOAT3 createPos;
    };

    class CanTripleAttack : public BaseSkill
    {
    public:
        CanTripleAttack(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
                "Can Triple Attack",
                BaseSkill::SUPER_RARE,
                true) {}
        ~CanTripleAttack() {}

        void Overlaping()override;
    };
}

