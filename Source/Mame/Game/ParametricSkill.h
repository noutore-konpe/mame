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
                L"./Resources/Image/Card/Fleetsteed.png",
                L"./Resources/Image/Icon/iconFleetSpeed.png",
                "MoveSpeedUp",BaseSkill::COMMON) {}
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
                L"./Resources/Image/Card/Power.png",
                L"./Resources/Image/Icon/iconPower.png",
                "AttackPowerUp", BaseSkill::COMMON) {}
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
                L"./Resources/Image/Card/AttackSpeed.png",
                L"./Resources/Image/Icon/iconAttackSpeed.png",
                "AttackSpeedUp", BaseSkill::UNCOMMON) {}
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
                L"./Resources/Image/Card/Grimoire.png",
                L"./Resources/Image/Icon/iconGrimoire.png",
                "BookIncrease", BaseSkill::RARE) {}
        ~BookIncrease() {}

        void Overlaping()override;
    };

    //�̗͏���A�b�v
    class MaxHitPointUp : public BaseSkill
    {
    public:
        MaxHitPointUp(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Life.png",
                L"./Resources/Image/Icon/iconLife.png",
                "BookIncrease", BaseSkill::COMMON) {}
        ~MaxHitPointUp() {}

        void Overlaping()override;

    private:
        float maxHealthIncreasing = 1.0f;//�㏸�l
    };

    // �h���
    class DefenseUp : public BaseSkill
    {
    public:
        DefenseUp(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Protect.png",
                L"./Resources/Image/Icon/iconProtect.png",
                "DefenseUp", BaseSkill::UNCOMMON) {}
        ~DefenseUp() {}

        void Overlaping()override;

    private:
        float defenseIncreasing = 2.0f;//�㏸�l
    };
}

