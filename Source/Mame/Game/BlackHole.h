#pragma once

#include "Ability.h"

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

private:
    static int nameNum_;

private:
    std::unique_ptr<MagicCircle> magicCircle_;

    float lifeTimer_    = 5.0f;
    float radius_       = 0.25f;
    float inhaleLength_ = 3.0f; // ãzÇ¢çûÇﬁãóó£
    float inhaleForce_  = 2.0f; // ãzÇ¢çûÇﬁóÕ

};

