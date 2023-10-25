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
                L"./Resources/Image/Card/Drain.png",
                L"./Resources/Image/Icon/iconDrain.png",
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
                L"./Resources/Image/Card/BlackHole.png",
                L"./Resources/Image/Icon/iconBlackHole.png",
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
                L"./Resources/Image/Card/ThirdAttack.png",
                L"./Resources/Image/Icon/iconThirdAttack.png",
                "Can Triple Attack",
                BaseSkill::COMMON,
                true) {}
        ~CanTripleAttack() {}

        void Overlaping()override;
    };

    class CanCounterAttack : public BaseSkill
    {
    public:
        CanCounterAttack(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Counter.png",
                L"./Resources/Image/Icon/iconCounter.png",
                "Can Counter Attack",
                BaseSkill::COMMON,
                true) {}
        ~CanCounterAttack() {}

        void Overlaping()override;
    };

    class ChangeHomingSkill : public BaseSkill
    {
    public:
        ChangeHomingSkill(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Homing.png",
                L"./Resources/Image/Icon/iconHoming.png",
                "Homing",
                BaseSkill::SUPER_RARE,
                true) {}
        ~ChangeHomingSkill() {}

        void Overlaping()override;
    };

    class PoisonSkill : public BaseSkill
    {
    public:
        PoisonSkill(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Poison.png",
                L"./Resources/Image/Icon/iconPoison.png",
                "Poison",
                BaseSkill::RARE) {}
        ~PoisonSkill() {}

        void Overlaping()override;

    private:
        const float damageIncreasing = 3.0f;
        const float effectTimeIncreasing = 5.0f;
    };

    class RevengeSkill : public BaseSkill
    {
    public:
        RevengeSkill(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Knockback.png",
                L"./Resources/Image/Icon/iconKnockback.png",
                "Poison", 
                BaseSkill::RARE,
                true) {}
        ~RevengeSkill() {}

        void Overlaping()override;
    };
}

