#pragma once
#include "BaseSkill.h"

//UŒ‚‚ÉŠÖ‚·‚éƒXƒLƒ‹
namespace PlayerSkill
{
    // ‹zû
    class Drain : public BaseSkill
    {
    public:
        Drain(Player* player);
        ~Drain() {}

        void Initialize()override;
        void Update(float elapsedTime)override;
        void DrawDebug()override;

        void Overlaping()override;

        //HP‹zûˆ— ‚±‚ê‚ğƒvƒŒƒCƒ„[‚ÌUŒ‚ˆ—‚É‘g‚İ‚Ş
        void Assimilate(float power/*“G‚É—^‚¦‚½UŒ‚‚Ì—Í*/);

    private:
        float drainCoefficient;//UŒ‚—Í‚©‚ç‹zûHP‚ÌZo‚Ég‚¤ŒW”(d•¡‚·‚é‚½‚Ñ‚É‚±‚ê‚ª‘‚¦‚Ä‚¢‚­)

        std::unique_ptr<Sprite> card2;
    };

    //”š”j‘®«•t—^i–vj
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

    //ƒuƒ‰ƒbƒNƒz[ƒ‹
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

        //ã¸UŒ‚”{—¦
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

        //ã¸UŒ‚”{—¦
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

        //d•Ô‚µƒ_ƒ[ƒW”{—¦
        float revengeMul = 0.3f;
        const float revengeIncreasing = 0.2f;
    private:
        std::unique_ptr<Sprite> card2;
    };
}

