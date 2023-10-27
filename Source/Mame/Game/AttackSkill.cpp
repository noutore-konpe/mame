#include "AttackSkill.h"
#include "BlackHole.h"
#include "PlayerManager.h"

namespace PlayerSkill
{
    Drain::Drain(Player* player) :
        BaseSkill(player,
            L"./Resources/Image/Card/Drain.png",
            L"./Resources/Image/Icon/iconDrain.png",
            "Drain", BaseSkill::SUPER_RARE) 
    {
        card2 = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), L"./Resources/Image/Card/Drain2.png", "./Resources/Shader/sprite_dissolve_ps.cso");
        card2->GetSpriteDissolve()->SetMaskTextureValue(2);
    }
    void Drain::Initialize()
    {
        BaseSkill::Initialize();
        //probability = 5;//出現確率
        drainCoefficient = 0.3f;
    }

    void Drain::Update(float elapsedTime)
    {
        if (overlap > 0)
        {
            card = card2.get();   
        }
        card2->GetSpriteTransform()->SetPos(card1->GetSpriteTransform()->GetPos());
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

    BlackHoleSkill::BlackHoleSkill(Player* player) :
        BaseSkill(player,
            L"./Resources/Image/Card/BlackHole.png",
            L"./Resources/Image/Icon/iconBlackHole.png",
            "BlackHole",
            BaseSkill::ULTRA_RARE) 
    {
        card2 = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), L"./Resources/Image/Card/BlackHole2.png", "./Resources/Shader/sprite_dissolve_ps.cso");
        card2->GetSpriteDissolve()->SetMaskTextureValue(2);
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
        if (overlap > 0)
        {
            card = card2.get();
        }
        card2->GetSpriteTransform()->SetPos(card1->GetSpriteTransform()->GetPos());

        if (overlap <= 0)return;

        coolTimer += elapsedTime;
        if (coolTimer > coolTime)
        {
            coolTimer = 0;
            
            CreateBlackHole();
        }

        
    }

    CanTripleAttack::CanTripleAttack(Player* player) :
        BaseSkill(player,
            L"./Resources/Image/Card/ThirdAttack.png",
            L"./Resources/Image/Icon/iconThirdAttack.png",
            "Can Triple Attack",
            BaseSkill::COMMON,
            true) 
    {
        card2 = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), L"./Resources/Image/Card/ThirdAttack2.png", "./Resources/Shader/sprite_dissolve_ps.cso");
        card2->GetSpriteDissolve()->SetMaskTextureValue(2);
    }

    void CanTripleAttack::Update(float elapsedTime)
    {
        if (overlap > 0)
        {
            card = card2.get();
        }
        card2->GetSpriteTransform()->SetPos(card1->GetSpriteTransform()->GetPos());
    }

    void CanTripleAttack::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;
    }
    CanCounterAttack::CanCounterAttack(Player* player) :
        BaseSkill(player,
            L"./Resources/Image/Card/Counter.png",
            L"./Resources/Image/Icon/iconCounter.png",
            "Can Counter Attack",
            BaseSkill::COMMON,
            true) 
    {
        card2 = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), L"./Resources/Image/Card/ThirdAttack2.png", "./Resources/Shader/sprite_dissolve_ps.cso");
        card2->GetSpriteDissolve()->SetMaskTextureValue(2);
    }

    void CanCounterAttack::Update(float elapsedTime)
    {
        if (overlap > 0)
        {
            card = card2.get();
        }
        card2->GetSpriteTransform()->SetPos(card1->GetSpriteTransform()->GetPos());
    }

    void CanCounterAttack::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;
    }
    void ChangeHomingSkill::Initialize()
    {
        BaseSkill::Initialize();
        isNotElected = true;
    }
    void ChangeHomingSkill::Overlaping()
    {
        overlap++;

        if (PlayerManager::Instance().GetPoisonSkill()->Active())
        {
            PlayerManager::Instance().SetTamaType(static_cast<int>(PlayerManager::TYPE::All));
        }
        else
        {
            PlayerManager::Instance().SetTamaType(static_cast<int>(PlayerManager::TYPE::Homing));
        }

        if (overlap <= 1)return;
    }
    PoisonSkill::PoisonSkill(Player* player) :
        BaseSkill(player,
            L"./Resources/Image/Card/Poison.png",
            L"./Resources/Image/Icon/iconPoison.png",
            "Poison",
            BaseSkill::RARE) 
    {
        card2 = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), L"./Resources/Image/Card/Poison2.png", "./Resources/Shader/sprite_dissolve_ps.cso");
        card2->GetSpriteDissolve()->SetMaskTextureValue(2);
    }
    void PoisonSkill::Update(float elapsedTime)
    {
        if (overlap > 0)
        {
            card = card2.get();
        }
        card2->GetSpriteTransform()->SetPos(card1->GetSpriteTransform()->GetPos());
    }
    void PoisonSkill::Initialize()
    {
        BaseSkill::Initialize();
        isNotElected = true;
    }
    void PoisonSkill::Overlaping()
    {
        overlap++;
        if (PlayerManager::Instance().GetHomingSkill()->Active())
        {
            PlayerManager::Instance().SetTamaType(static_cast<int>(PlayerManager::TYPE::All));
        }
        else
        {
            PlayerManager::Instance().SetTamaType(static_cast<int>(PlayerManager::TYPE::Doku));
        }

        if (overlap <= 1)return;

        player->poisonSlipDamage += damageIncreasing;
        player->poisonEffectTime += effectTimeIncreasing;
    }

    RevengeSkill::RevengeSkill(Player* player) :
        BaseSkill(player,
            L"./Resources/Image/Card/Knockback.png",
            L"./Resources/Image/Icon/iconKnockback.png",
            "KnockBack",
            BaseSkill::RARE) 
    {
        card2 = std::make_unique<Sprite>(Graphics::Instance().GetDevice(), L"./Resources/Image/Card/Knockback2.png", "./Resources/Shader/sprite_dissolve_ps.cso");
        card2->GetSpriteDissolve()->SetMaskTextureValue(2);
    }

    void RevengeSkill::Initialize()
    {
        BaseSkill::Initialize();
        revengeMul = 0.3f;
    }

    void RevengeSkill::Update(float elapsedTime)
    {
        if (overlap > 0)
        {
            card = card2.get();
        }
        card2->GetSpriteTransform()->SetPos(card1->GetSpriteTransform()->GetPos());
    }

    void RevengeSkill::Overlaping()
    {
        overlap++;
        if (overlap <= 1)return;

        revengeMul += revengeIncreasing;
    }
}