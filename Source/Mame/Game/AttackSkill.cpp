#include "AttackSkill.h"
#include "Player.h"
#include "BlackHole.h"

namespace PlayerSkill
{
    void Drain::Initialize()
    {
        BaseSkill::Initialize();
        //probability = 5;//出現確率
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

        //クールタイム減少　最低１０秒
        coolTime -= coolTimeDecrease;
        if (coolTime <= 10.0f)coolTimer = 10.0f;
    }

    void BlackHoleSkill::CreateBlackHole()
    {
        BlackHole* blackhole = new BlackHole(player->GetAbilityManager());
        createPos = player->GetTransform()->GetPosition();
        createPos = createPos + player->GetTransform()->CalcForward() * 2;
        createPos = player->CollidedPosition(createPos);
        blackhole->GetTransform()->SetPosition(createPos);
        blackhole->Initialize();
        player->GetAbilityManager()->Register(blackhole);
    }
    void BlackHoleSkill::Update(float elapsedTime)
    {
        if (overlap <= 0)return;

        coolTimer += elapsedTime;
        if (coolTimer > coolTime)
        {
            coolTimer = 0;
            
            CreateBlackHole();
        }
    }

    void CanTripleAttack::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;
    }
    void CanCounterAttack::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;
    }
    void ChangeHomingSkill::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;
    }
    void PoisonSkill::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;

        player->poisonSlipDamage += damageIncreasing;
    }
}