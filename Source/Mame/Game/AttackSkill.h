#pragma once
#include "BaseSkill.h"

//攻撃に関するスキル
namespace PlayerSkill
{
    class Drain : public BaseSkill
    {
    public:
        Drain(Player* player) :
            BaseSkill(player, 
                L"./Resources/Image/Card/sampleCard.png",
                L"./Resources/Image/Icon/sampleIcon.png",
                "Drain") {}
        ~Drain() {}

        void Initialize()override;
        void Update(float elapsedTime)override;
        void Render()override;
        void DrawDebug()override;

        void Overlaping()override;

        //HP吸収処理 これをプレイヤーの攻撃処理に組み込む
        void Assimilate(float power/*敵に与えた攻撃の力*/);

    private:
        float drainCoefficient;//攻撃力から吸収HPの算出に使う係数(重複するたびにこれが増えていく)
    };
}

