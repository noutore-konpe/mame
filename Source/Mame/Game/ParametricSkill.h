#pragma once
#include "BaseSkill.h"

//�p�����[�^�̏㏸�֌W�̃X�L��
namespace PlayerSkill
{
    //�ړ����x�㏸
    class MoveSpeedUp : public BaseSkill
    {
    public:
        MoveSpeedUp(Player* player) : 
            BaseSkill(player,
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
                "MoveSpeedUp") {}
        ~MoveSpeedUp() {}

        void Overlaping()override;

    private:
        float speedIncreasing = 0.5f;//�㏸�l
    };

    //��b�U���͏㏸
    class AttackPowerUp : public BaseSkill
    {
    public:
        AttackPowerUp(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
                "AttackPowerUp") {}
        ~AttackPowerUp() {}

        void Overlaping()override;

    private:
        float attackPowerIncreasing = 4.0f;//�㏸�l
    };

    //�U�����x�㏸
    class AttackSpeedUp : public BaseSkill
    {
    public:
        AttackSpeedUp(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
                "AttackSpeedUp") {}
        ~AttackSpeedUp() {}

        void Overlaping()override;

    private:
        float attackSpeedIncreasing = 0.1f;//�㏸�l
    };

    //�{���A�b�v
    class BookIncrease : public BaseSkill
    {
    public:
        BookIncrease(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
                "BookIncrease") {}
        ~BookIncrease() {}

        void Overlaping()override;
    };
}

