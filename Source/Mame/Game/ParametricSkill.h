#pragma once
#include "BaseSkill.h"

namespace PlayerSkill
{
    class MoveSpeedUp : public BaseSkill
    {
    public:
        MoveSpeedUp(Player* player) : BaseSkill(player) {}
        ~MoveSpeedUp() {}

        void Initialize()override;
        void Overlaping()override;

    private:
        const float SpeedIncreasing = 0.3f;
    };
}

