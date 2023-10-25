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
                L"./Resources/Image/Card/Cidouup.png",
                L"./Resources/Image/Icon/idouup.png",
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
                L"./Resources/Image/Card/Ckougekiup.png",
                L"./Resources/Image/Icon/pawaup.png",
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
                L"./Resources/Image/Card/Ckougekisokuup.png",
                L"./Resources/Image/Icon/kougekisokuup.png",
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
                L"./Resources/Image/Card/Chon.png",
                L"./Resources/Image/Icon/hon.png",
                "BookIncrease", BaseSkill::RARE) {}
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
                L"./Resources/Image/Card/Chpup.png",
                L"./Resources/Image/Icon/hpup.png",
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
                L"./Resources/Image/Card/Cbougyo.png",
                L"./Resources/Image/Icon/bougyo.png",
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
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
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
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
                "BulletRateUp", BaseSkill::UNCOMMON) {}
        ~BulletRateUp() {}

        void Initialize()override;
        void Overlaping()override;

        const float GetBulletRate()const { return bulletRate; }

    private:
        const float initLaunchTime = 0.5f;

        const float rateIncreasing = 0.05f;//�㏸�l

        float bulletRate = 0.5f;
    };
}

