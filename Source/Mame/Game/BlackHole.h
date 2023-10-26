#pragma once

#include "Ability.h"
#include "../Graphics/Effect.h"

class MagicCircle;

class BlackHole : public Ability
{
public:
    BlackHole(AbilityManager* abilityManager);
    ~BlackHole() override;

    void Initialize() override;
    void Finalize() override;
    void Begin() override;
    void Update(const float elapsedTime) override;
    void End() override;
    void Render(const float scale, ID3D11PixelShader* psShader = nullptr) override;
    void DrawDebug() override;

    void PlayEffect();
private:
    static int nameNum_;

private:
    std::unique_ptr<MagicCircle> magicCircle_;

    std::unique_ptr<Effect> blackholeEffect;

    float lifeTimer_    = 5.0f;
    float inhaleLength_ = 5.0f; // ãzÇ¢çûÇﬁãóó£
    float inhaleForce_  = 4.0f; // ãzÇ¢çûÇﬁóÕ
    int effectPlayTime = 0;

};

