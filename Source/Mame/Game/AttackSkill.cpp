#include "AttackSkill.h"

namespace PlayerSkill
{
    void Drain::Initialize()
    {
        probability = 5;//èoåªämó¶
        drainCoefficient = 0.1f;
    }

    void Drain::Update(float elapsedTime)
    {

    }

    void Drain::Assimilate(float power)
    {
        float drainAmount = power * drainCoefficient;

    }
}