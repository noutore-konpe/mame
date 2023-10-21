#include "AttackSkill.h"
#include "Player.h"
#include "BlackHole.h"

namespace PlayerSkill
{
    void Drain::Initialize()
    {
        //probability = 5;//èoåªämó¶
        drainCoefficient = 0.3f;
    }

    void Drain::Update(float elapsedTime)
    {

    }

    void Drain::DrawDebug()
    {
        if (ImGui::BeginMenu("Drain"))
        {
            ImGui::SliderFloat("Drain Coefficient",&drainCoefficient,0.0f,1.0f);

            ImGui::InputInt("Overlap", &overlap);

            if (ImGui::Button("Overlaping"))
            {
                Overlaping();
            }

            ImGui::EndMenu();
        }
    }

    void Drain::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;
        drainCoefficient += 0.15f;
    }

    void Drain::Assimilate(float power)
    {
        if (overlap <= 0)return;
        float drainAmount = power * drainCoefficient;
        player->ApplyHeal(drainAmount);
    }

    void BlackHoleSkill::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;
    }
    void BlackHoleSkill::CreateBlackHole()
    {
        BlackHole* blackHole = new BlackHole(player->GetAbilityManager());
        blackHole->Initialize();
        player->GetAbilityManager()->Register(blackHole);
    }
    void CanTripleAttack::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;
    }
}