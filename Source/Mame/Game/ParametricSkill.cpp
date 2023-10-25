#include "ParametricSkill.h"
#include "ItemManager.h"
#include "Book.h"
#include "PlayerManager.h"

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
        //各種本のスキルを抽選対象
        if (overlap == 0)
        {
            auto& playerManager = PlayerManager::Instance();
            playerManager.GetBulletRateUpSkill()->isNotElected = false;
            playerManager.GetBulletSizeUpSkill()->isNotElected = false;
            playerManager.GetHomingSkill()->isNotElected = false;
            playerManager.GetPoisonSkill()->isNotElected = false;
        }

        overlap++;
        Book* book = new Book();
        book->Initialize();
        books.emplace_back(book);
        ItemManager::Instance().Register((book));
    }

    void MaxHitPointUp::Overlaping()
    {
        overlap++;
        player->AddMaxHealth(maxHealthIncreasing);
        player->ApplyHeal(maxHealthIncreasing);
    }

    void DefenseUp::Overlaping()
    {
        overlap++;
        player->AddDefense(defenseIncreasing);
    }
    void BulletRateUp::Initialize()
    {
        BaseSkill::Initialize();
        reloadTime = initReloadTime;
        rate = initShotRate;
        isNotElected = true;
    }
    void BulletRateUp::Overlaping()
    {
        overlap++;
        auto* bookSkill = PlayerManager::Instance().GetBookIncreaseSkill();
        for (auto& book : bookSkill->GetBooks())
        {
            float reload = initReloadTime - overlap * reloadIncreasing;
            if (reload < 0.3f)reload = 0.5f;
            book->SetReloadTime(reload);
            reloadTime = reload;

            float rate = initShotRate - overlap * rateIncreasing;
            if (rate < 0.1f)rate = 0.1f;
            book->SetLaunchTime(rate);
            this->rate = rate;
        }
    }

    void BulletSizeUp::Initialize()
    {
        BaseSkill::Initialize();
        scale = initScale;
        radius = initRadius;
        isNotElected = true;
    }

    void BulletSizeUp::Overlaping()
    {
        overlap++;
        auto* bookSkill = PlayerManager::Instance().GetBookIncreaseSkill();
        for (auto& book : bookSkill->GetBooks())
        {
            float sca = initScale + overlap * scaleIncreasing;
            if (sca > 3.0f)sca = 3.0f;
            book->SetBulletScale(sca);
            scale = sca;

            float r = initRadius + overlap * radiusIncreasing;
            if (r > 1.0f)r = 1.0f;
            book->SetBulletRadius(r);
            radius = r;
        }
    }
}
