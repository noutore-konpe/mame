#pragma once
#include "BaseSkill.h"

//攻撃に関するスキル
namespace PlayerSkill
{
    // 吸収
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

        //HP吸収処理 これをプレイヤーの攻撃処理に組み込む
        void Assimilate(float power/*敵に与えた攻撃の力*/);

    private:
        float drainCoefficient;//攻撃力から吸収HPの算出に使う係数(重複するたびにこれが増えていく)
    };

    //爆破属性付与（没）
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

    //ブラックホール
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

