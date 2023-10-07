#pragma once
#include "BaseSkill.h"

//�U���Ɋւ���X�L��
namespace PlayerSkill
{
    class Drain : public BaseSkill
    {
    public:
        Drain(Player* player) :
            BaseSkill(player, 
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
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
}

