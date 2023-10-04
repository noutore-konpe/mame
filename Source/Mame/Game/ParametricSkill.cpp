#include "ParametricSkill.h"
#include "Player.h"
#include "ItemManager.h"
#include "Book.h"

namespace PlayerSkill
{   
    void MoveSpeedUp::Overlaping()
    {
        overlap++;
        player->AddMaxSpeed(speedIncreasing);
    }

    void AttackPowerUp::Overlaping()
    {
        overlap++;
        player->AddBaseAttackPower(attackPowerIncreasing);
    }

    void AttackSpeedUp::Overlaping()
    {
        overlap++;
        player->AddAttackSpeed(attackSpeedIncreasing);
    }

    void BookIncrease::Overlaping()
    {
        overlap++;
        ItemManager::Instance().Register(new Book());
    }
}
