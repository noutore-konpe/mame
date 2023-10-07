#pragma once
#include "BaseSkill.h"

//UŒ‚‚ÉŠÖ‚·‚éƒXƒLƒ‹
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

        //HP‹zûˆ— ‚±‚ê‚ğƒvƒŒƒCƒ„[‚ÌUŒ‚ˆ—‚É‘g‚İ‚Ş
        void Assimilate(float power/*“G‚É—^‚¦‚½UŒ‚‚Ì—Í*/);

    private:
        float drainCoefficient;//UŒ‚—Í‚©‚ç‹zûHP‚ÌZo‚Ég‚¤ŒW”(d•¡‚·‚é‚½‚Ñ‚É‚±‚ê‚ª‘‚¦‚Ä‚¢‚­)
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

