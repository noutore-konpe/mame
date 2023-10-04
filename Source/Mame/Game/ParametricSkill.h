#pragma once
#include "BaseSkill.h"

//パラメータの上昇関係のスキル
namespace PlayerSkill
{
    //移動速度上昇
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
        const float speedIncreasing = 0.3f;//上昇値
    };

    //基礎攻撃力上昇
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
        const float attackPowerIncreasing = 2.0f;//上昇値
    };

    //攻撃速度上昇
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
        const float attackSpeedIncreasing = 0.1f;//上昇値
    };

    //本数アップ
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

