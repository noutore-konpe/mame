#pragma once
#include "BaseSkill.h"

//�U���Ɋւ���X�L��
namespace PlayerSkill
{
    // �z��
    class Drain : public BaseSkill
    {
    public:
        Drain(Player* player);
        ~Drain() {}

        void Initialize()override;
        void Update(float elapsedTime)override;
        void DrawDebug()override;

        void Overlaping()override;

        //HP�z������ ������v���C���[�̍U�������ɑg�ݍ���
        void Assimilate(float power/*�G�ɗ^�����U���̗�*/);

    private:
        float drainCoefficient;//�U���͂���z��HP�̎Z�o�Ɏg���W��(�d�����邽�тɂ��ꂪ�����Ă���)

        std::unique_ptr<Sprite> card2;
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
        BlackHoleSkill(Player* player);
        ~BlackHoleSkill() {}

        void Overlaping()override;

        void CreateBlackHole();

        void Update(float elapsedTime);

    private:
        float coolTime = 30.0f;
        float coolTimer;
        const float coolTimeDecrease = 3.0f;

        

        DirectX::XMFLOAT3 createPos;

        std::unique_ptr<Sprite> card2;
    };

    class CanTripleAttack : public BaseSkill
    {
    public:
        CanTripleAttack(Player* player);
        ~CanTripleAttack() {}

        void Update(float elapsedTime);
        void Overlaping()override;

    private:
        std::unique_ptr<Sprite> card2;

        //�㏸�U���{��
        const float mulIncreasing = 0.2f;
    };

    class CanCounterAttack : public BaseSkill
    {
    public:
        CanCounterAttack(Player* player);
        ~CanCounterAttack() {}

        void Update(float elapsedTime);
        void Overlaping()override;

    private:
        std::unique_ptr<Sprite> card2;

        //�㏸�U���{��
        const float mulIncreasing = 0.2f;
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

        void Initialize()override;
        void Overlaping()override;
    };

    class PoisonSkill : public BaseSkill
    {
    public:
        PoisonSkill(Player* player);
        ~PoisonSkill() {}

        void Update(float elapsedTime)override;
        void Initialize()override;
        void Overlaping()override;

    private:
        const float damageIncreasing = 10.0f;
        const float effectTimeIncreasing = 5.0f;

        std::unique_ptr<Sprite> card2;
    };

    class RevengeSkill : public BaseSkill
    {
    public:
        RevengeSkill(Player* player);
        ~RevengeSkill() {}

        void Initialize()override;
        void Update(float elapsedTime)override;
        void Overlaping()override;

        //�d�Ԃ��_���[�W�{��
        float revengeMul = 0.3f;
        const float revengeIncreasing = 0.2f;
    private:
        std::unique_ptr<Sprite> card2;
    };
}

