#pragma once
#include "BaseSkill.h"

class Book;

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
                "BookIncrease", BaseSkill::COMMON,
                false,
                4) {}
        ~BookIncrease() {}

        void Overlaping()override;

        std::vector<Book*> GetBooks() { return books; }

    private:
        std::vector<Book*> books;
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
        float maxHealthIncreasing = 100.0f;//�㏸�l
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

    // �e�ۂ̑傫���A�b�v
    class BulletSizeUp : public BaseSkill
    {
    public:
        BulletSizeUp(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Bigbullet.png",
                L"./Resources/Image/Icon/iconBigbullet.png",
                "BulletSizeUp", BaseSkill::UNCOMMON) {}
        ~BulletSizeUp() {}

        void Initialize()override;
        void Overlaping()override;

        const float GetBulletScale()const { return scale; }
        const float GetBulletRadius()const { return radius; }

    private:
        const float scaleIncreasing = 0.2f;//�㏸�l
        const float radiusIncreasing = 0.02f;//�㏸�l

        const float initScale = 1.0f;
        const float initRadius = 0.1f;

        float scale = 1.0f;
        float radius = 0.1f;
    };

    // �e�ۘA�ˑ��x�A�b�v
    class BulletRateUp : public BaseSkill
    {
    public:
        BulletRateUp(Player* player) :
            BaseSkill(player,
                L"./Resources/Image/Card/Reload.png",
                L"./Resources/Image/Icon/iconReload.png",
                "BulletRateUp", BaseSkill::UNCOMMON) {}
        ~BulletRateUp() {}

        void Initialize()override;
        void Overlaping()override;

        const float GetReloadTime()const { return reloadTime; }
        const float GetBulletRate() const { return rate; }
    private:
        const float initReloadTime = 3.0f;
        const float reloadIncreasing = 0.3f;//�㏸�l
        float reloadTime = 3.0f;

        const float initShotRate = 0.5f;
        const float rateIncreasing = 0.05f;
        float rate = 5.0f;
    };
}

